#include "AutoMenuManager.h"
#include "../Config.h"
#include "../DLLMain.h"

#include <algorithm>
#include <random>
#include <sstream>
#include "../StringUtil.h"

#include "../BW/MenuPosition.h"
#include "../BW/Dialog.h"
#include "GameImpl.h"
#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>
#include "../BW/Offsets.h"

#include <BWAPI/Input.h>

using namespace BWAPI4;

namespace
{
  std::mt19937 mt{ static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()) };
}

AutoMenuManager::AutoMenuManager()
{
  auto const seedOverride = LoadConfigInt("starcraft", "seed_override", std::numeric_limits<decltype(GameImpl::seedOverride)>::max());
  if (seedOverride != std::numeric_limits<decltype(seedOverride)>::max())
    mt.seed(seedOverride);
  this->reloadConfig();
}

void AutoMenuManager::reloadConfig()
{
  InitPrimaryConfig();

  // Reset menu activation variables
  this->actRaceSel = false;
  this->actStartedGame = false;
  this->autoMapTryCount = 0;

  //this function is called when starcraft loads and at the end of each match.
  //the function loads the parameters for the auto-menu feature such as auto_menu, map, race, enemy_race, enemy_count, and game_type
  this->autoMenuMode = LoadConfigStringUCase("auto_menu", "auto_menu", "OFF");
#ifdef _DEBUG
  this->autoMenuPause = LoadConfigStringUCase("auto_menu", "pause_dbg", "OFF");
#endif
  this->autoMenuRestartGame = LoadConfigStringUCase("auto_menu", "auto_restart", "OFF");
  this->autoMenuGameName = LoadConfigString("auto_menu", "game");
  this->autoMenuCharacterName = LoadConfigString("auto_menu", "character_name", "FIRST").substr(0, 24);

  // Load map string
  std::string cfgMap = LoadConfigString("auto_menu", "map", "");
  std::replace(cfgMap.begin(), cfgMap.end(), '/', '\\');

  // Used to check if map string was changed.
  static std::string lastAutoMapString;
  bool mapChanged = false;

  // If the auto-menu map field was changed

  if (lastAutoMapString != cfgMap)
  {
    lastAutoMapString = cfgMap;
    this->lastAutoMapEntry = 0;
    this->lastMapGen.clear();
    this->autoMapPool.clear();

    // Get just the directory
    this->autoMenuMapPath.clear();
    size_t tmp = cfgMap.find_last_of("\\/\n");
    if (tmp != std::string::npos)
      this->autoMenuMapPath = cfgMap.substr(0, tmp);
    this->autoMenuMapPath += "\\";

    // Iterate files in directory
    WIN32_FIND_DATAA finder = { 0 };
    HANDLE hFind = FindFirstFileA(cfgMap.c_str(), &finder);

    if (hFind != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (!(finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  // Check if found is not a directory
        {
          // Convert to string and add to autoMapPool if the type is valid
          std::string finderStr = std::string(finder.cFileName);
          if (getFileType(this->autoMenuMapPath + finderStr))
          {
            this->autoMapPool.push_back(finderStr);
          }
        }
      } while (FindNextFileA(hFind, &finder));
      FindClose(hFind);
    } // handle exists

    mapChanged = true;
  } // if map was changed^

  // Get map iteration config
  std::string newMapIteration = LoadConfigStringUCase("auto_menu", "mapiteration", "RANDOM");
  if (this->autoMapIteration != newMapIteration)
  {
    this->autoMapIteration = newMapIteration;
    this->lastAutoMapEntry = 0;
    this->lastMapGen.clear();

    mapChanged = true;
  }

  if (mapChanged)
    this->chooseNewRandomMap();

  this->autoMenuLanMode = LoadConfigString("auto_menu", "lan_mode", "Local Area Network (UDP)");
  this->autoMenuRace = LoadConfigStringUCase("auto_menu", "race", "RANDOM");
  std::stringstream raceList(this->autoMenuRace);

  std::string currentrace = this->autoMenuRace.substr(0, this->autoMenuRace.find_first_of(','));

  for (int i = 0; i < (int)gdwProcNum && raceList; ++i)
      std::getline(raceList, currentrace, ',');

  // trim whitespace outside quotations and then the quotations
  Util::trim(currentrace, Util::is_whitespace_or_newline);
  Util::trim(currentrace, [](char c) { return c == '"'; });

  this->autoMenuRace = currentrace;

  this->autoMenuEnemyRace[0] = LoadConfigStringUCase("auto_menu", "enemy_race", "RANDOM");
  for (unsigned int i = 1; i < 8; ++i)
  {
    std::string key = "enemy_race_" + std::to_string(i);
    this->autoMenuEnemyRace[i] = LoadConfigStringUCase("auto_menu", key.c_str(), "DEFAULT");
    if (this->autoMenuEnemyRace[i] == "DEFAULT")
      this->autoMenuEnemyRace[i] = this->autoMenuEnemyRace[0];
  }

  this->autoMenuEnemyCount = LoadConfigInt("auto_menu", "enemy_count", 1);
  this->autoMenuEnemyCount = std::min(std::max(this->autoMenuEnemyCount, 0U), 7U);

  this->autoMenuGameType = LoadConfigStringUCase("auto_menu", "game_type", "MELEE");
  this->autoMenuGameTypeExtra = LoadConfigString("auto_menu", "game_type_extra", "");
  this->autoMenuSaveReplay = LoadConfigString("auto_menu", "save_replay");

  this->autoMenuMinPlayerCount = LoadConfigInt("auto_menu", "wait_for_min_players", 2);
  this->autoMenuMaxPlayerCount = LoadConfigInt("auto_menu", "wait_for_max_players", 8);
  this->autoMenuWaitPlayerTime = LoadConfigInt("auto_menu", "wait_for_time", 30000);
}

void AutoMenuManager::chooseNewRandomMap()
{
  if (!this->autoMapPool.empty())
  {
    std::string chosen;
    if (this->autoMapIteration == "RANDOM")
    {
      // Obtain a random map file
      std::sample(autoMapPool.begin(), autoMapPool.end(), &chosen, 1, mt);
    }
    else if (this->autoMapIteration == "SEQUENCE")
    {
      if (this->lastAutoMapEntry >= this->autoMapPool.size())
        this->lastAutoMapEntry = 0;
      chosen = this->autoMapPool[this->lastAutoMapEntry++];
    }
    this->lastMapGen = this->autoMenuMapPath + chosen;
  }
}

unsigned int getLobbyPlayerCount()
{
  unsigned int rval = 0;
  for (unsigned int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    if (BW::BWDATA::Players[i].nType == PlayerTypes::Player)
      ++rval;
  }
  return rval;
}
unsigned int getLobbyPlayerReadyCount()
{
  unsigned int rval = 0;
  for (unsigned int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    if (BW::BWDATA::Players[i].nType == PlayerTypes::Player && BW::BWDATA::PlayerDownloadStatus[i] >= 100)
      ++rval;
  }
  return rval;
}
unsigned int getLobbyOpenCount()
{
  unsigned int rval = 0;
  for (unsigned int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
  {
    if (BW::BWDATA::Players[i].nType == PlayerTypes::EitherPreferHuman)
      ++rval;
  }
  return rval;
}

void AutoMenuManager::onMenuFrame()
{
  static DWORD createdTimer;
  static DWORD waitJoinTimer;
  static DWORD waitSelRaceTimer;
  static DWORD waitRestartTimer;

  // Don't attempt auto-menu if we run into 50 error message boxes
  if (this->autoMapTryCount > 50)
    return;

  // Return if autoMenu is not enabled
  if (this->autoMenuMode == "" || this->autoMenuMode == "OFF")
    return;

#ifdef _DEBUG
  // Wait for a debugger if autoMenuPause is enabled, and in DEBUG
  if (this->autoMenuPause != "OFF" && !IsDebuggerPresent())
    return;
#endif

  // Get the menu mode
  int menu = BW::BWDATA::glGluesMode;

  // Declare a commonly used dialog pointer
  BW::dialog *tempDlg = nullptr;

  // Get some autoMenu properties
  bool isAutoSingle = this->autoMenuMode == "SINGLE_PLAYER";
  bool isCreating = !this->autoMenuMapPath.empty();
  bool isJoining = !this->autoMenuGameName.empty();

  // Reset raceSel flag
  if (menu != BW::GLUE_CHAT)
  {
    this->actRaceSel = false;
    this->actStartedGame = false;
  }

  // Iterate through the menus
  switch (menu)
  {
  case BW::GLUE_MAIN_MENU:    // Main menu
    if (BW::FindDialogGlobal("TitleDlg")) // skip if at title
      break;

    // Choose single or multi
    pressDialogKey(BW::FindDialogGlobal("MainMenu")->findIndex(isAutoSingle ? 3 : 4));

    // choose original or expansion (we always choose expansion)
    if (BW::FindDialogGlobal("Delete"))
      pressDialogKey(BW::FindDialogGlobal("Delete")->findIndex(7));
    break;
  case BW::GLUE_EX_CAMPAIGN:  // Campaign selection menu
  case BW::GLUE_CAMPAIGN:
    // Choose "Custom"
    pressDialogKey(BW::FindDialogGlobal("RaceSelection")->findIndex(10));
    break;
  case BW::GLUE_CREATE:       // Game creation menu
  case BW::GLUE_CREATE_MULTI:
    // Store the tick count while in this menu, and refer to it in the next
    createdTimer = GetTickCount();
    tempDlg = BW::FindDialogGlobal("Create");

    if (!this->lastMapGen.empty())
    {
      if (getFileType(this->lastMapGen) == 1)
      {
        // convert to game type
        GameType gt = GameType::getType(this->autoMenuGameType);

        // retrieve gametype dropdown
        BW::dialog *gameTypeDropdown = tempDlg->findIndex(17);
        if (!gameTypeDropdown)
          break;

        if (gt != GameTypes::None && gt != GameTypes::Unknown && (int)gameTypeDropdown->getSelectedValue() != gt)
        {
          gameTypeDropdown->setSelectedByValue(gt);
          break;
        }

        // game types with an extra settings dropdown
        if (gt == GameTypes::Top_vs_Bottom ||
            gt == GameTypes::Greed ||
            gt == GameTypes::Slaughter ||
            gt == GameTypes::Team_Melee ||
            gt == GameTypes::Team_Free_For_All ||
            gt == GameTypes::Team_Capture_The_Flag)
        {
          BW::dialog* extraDropdown = tempDlg->findIndex(18);
          if (!extraDropdown || std::string(extraDropdown->getSelectedString()).empty())
            break;
          
          if (!this->autoMenuGameTypeExtra.empty() &&
            extraDropdown->getSelectedString() != this->autoMenuGameTypeExtra)
          {
            extraDropdown->setSelectedByString(this->autoMenuGameTypeExtra);
            break;
          }
        }

        // if this is single player
        if (isAutoSingle)
        {
          // Set player race
          GameImpl::_changeRace(0, GameImpl::getMenuRace(this->autoMenuRace));

          // Set enemy races
          for (unsigned int i = 1; i <= this->autoMenuEnemyCount; ++i)
            GameImpl::_changeRace(i, GameImpl::getMenuRace(this->autoMenuEnemyRace[i]));

          //close remaining slots
          for (int i = this->autoMenuEnemyCount; i < 7; ++i)
          {
            BW::dialog *slot = tempDlg->findIndex((short)(21 + i));
            if (slot->getSelectedIndex() != 0)
              slot->setSelectedIndex(0);
          }
        } // if single
      } // if map is playable

      // if we encounter an unknown error when attempting to load the map
      if (BW::FindDialogGlobal("gluPOk"))
      {
        this->chooseNewRandomMap();
        ++this->autoMapTryCount;
        pressDialogKey(BW::FindDialogGlobal("gluPOk")->findIndex(1));
      }

      pressDialogKey(tempDlg->findIndex(12));
    } // if lastmapgen
    break;
  case BW::GLUE_CONNECT:
    tempDlg = BW::FindDialogGlobal("ConnSel");

    // Press hotkey if trying to get to BNET
    // or press it after the LAN mode has been selected
    if (this->autoMenuMode == "BATTLE_NET" ||
      (tempDlg->findIndex(5)->isVisible() &&
      tempDlg->findIndex(5)->setSelectedByString(this->autoMenuLanMode)))
      pressDialogKey(tempDlg->findIndex(9));

    waitJoinTimer = 0;
    break;
  case BW::GLUE_GAME_SELECT:  // Games listing
  {
    if (waitJoinTimer == 0)
      waitJoinTimer = GetTickCount();

    tempDlg = BW::FindDialogGlobal("GameSel");
    if (!tempDlg)
      break;

    bool gameToJoinExists = tempDlg->findIndex(5)->setSelectedByString(this->autoMenuGameName) ||
      (this->autoMenuGameName == "JOIN_FIRST" && tempDlg->findIndex(5)->getListCount() > 0);

    if (isJoining && !gameToJoinExists &&
      waitJoinTimer + (3000 * (BroodwarImpl.getInstanceNumber() + 1)) > GetTickCount())
      break; //wait for game to be hosted

    waitJoinTimer = GetTickCount();

    if (gameToJoinExists)
    {
      this->lastMapGen.clear();
      pressDialogKey(tempDlg->findIndex(13));  // OK
    }
    else if (isCreating)
    {
      pressDialogKey(tempDlg->findIndex(15));  // Create Game
    }
  }
  break;
  case BW::GLUE_CHAT:
    waitJoinTimer = 0;

    if (!actRaceSel &&
      BW::FindDialogGlobal("Chat") &&
      GameImpl::_currentPlayerId() >= 0 &&
      GameImpl::_currentPlayerId() < 8 &&
      waitSelRaceTimer + 300 < GetTickCount())
    {
      waitSelRaceTimer = GetTickCount();

      // Determine the current player's race
      Race playerRace = GameImpl::getMenuRace(this->autoMenuRace);
      if (playerRace != Races::Unknown && playerRace != Races::None)
      {
        // Check if the race was selected correctly, and prevent further changing afterwords
        u8 currentRace = BW::BWDATA::Players[GameImpl::_currentPlayerId()].nRace;

        if ((currentRace == playerRace ||
          (this->autoMenuRace == "RANDOMTP" &&
          (currentRace == Races::Terran ||
          currentRace == Races::Protoss)) ||
          (this->autoMenuRace == "RANDOMTZ" &&
          (currentRace == Races::Terran ||
          currentRace == Races::Zerg)) ||
          (this->autoMenuRace == "RANDOMPZ" &&
          (currentRace == Races::Protoss ||
          currentRace == Races::Zerg))
          ))
        {
          actRaceSel = true;
        }

        // Set the race
        if (!actRaceSel)
          GameImpl::_changeRace(8, playerRace);
      }// if player race is valid
    } // if dialog "chat" exists


    if (BW::FindDialogGlobal("gluPOk"))
    {
      pressDialogKey(BW::FindDialogGlobal("gluPOk")->findIndex(1));
      actStartedGame = false;
      waitRestartTimer = GetTickCount();
    }

    // Start the game if creating and auto-menu requirements are met
    if (isCreating &&
      waitRestartTimer + 2000 < GetTickCount() &&
      !actStartedGame &&
      getLobbyPlayerReadyCount() > 0 &&
      getLobbyPlayerReadyCount() == getLobbyPlayerCount() &&
      (getLobbyPlayerReadyCount() >= this->autoMenuMinPlayerCount || getLobbyOpenCount() == 0))
    {
      if (getLobbyPlayerReadyCount() >= this->autoMenuMaxPlayerCount || getLobbyOpenCount() == 0 || GetTickCount() > createdTimer + this->autoMenuWaitPlayerTime)
      {
        if (!BW::FindDialogGlobal("Chat")->findIndex(7)->isDisabled())
        {
          actStartedGame = true;
          BW::FindDialogGlobal("Chat")->findIndex(7)->activate();
        }
      } // if lobbyPlayerCount etc
    } // if isCreating etc
    break;
  case BW::GLUE_LOGIN:  // Registry/Character screen
  {
    if (this->autoMenuCharacterName == "WAIT")
      break;

    std::string name = this->autoMenuCharacterName;
    if (name == "FIRST")
      name = "bwapi"; // this name will be used if there are no existing characters
    else if (name.empty())
      name = "empty";

    BW::dialog* newIdPopup = BW::FindDialogGlobal("gluPEdit");
    if (newIdPopup)
    {
      newIdPopup->findIndex(4)->setText(&name[0]); //it'll copy the string
      BroodwarImpl.pressKey(K_RETURN); // popup Ok
      BroodwarImpl.pressKey(K_RETURN); // main Ok
    }
    else if (this->autoMenuCharacterName != "FIRST")
    {
      //if we're here, there are some existing characters
      BW::dialog* characterList = BW::FindDialogGlobal("Login")->findIndex(8);

      if (characterList->getListCount() == 0)
        break; //wait for list to exist/be populated
      if (characterList->setSelectedByString(this->autoMenuCharacterName))
        BroodwarImpl.pressKey(K_RETURN); // main Ok
      else
        pressDialogKey(BW::FindDialogGlobal("Login")->findIndex(6)); // New ID
    }
    else
      BroodwarImpl.pressKey(K_RETURN); // main Ok
    break;
  }
  case BW::GLUE_SCORE_Z_DEFEAT:
  case BW::GLUE_SCORE_Z_VICTORY:
  case BW::GLUE_SCORE_T_DEFEAT:
  case BW::GLUE_SCORE_T_VICTORY:
  case BW::GLUE_SCORE_P_DEFEAT:
  case BW::GLUE_SCORE_P_VICTORY:
    if (this->autoMenuRestartGame != "" && this->autoMenuRestartGame != "OFF")
      pressDialogKey(BW::FindDialogGlobal("End")->findIndex(7));
    break;
  case BW::GLUE_READY_T:  // Mission Briefing
  case BW::GLUE_READY_Z:
  case BW::GLUE_READY_P:
    pressDialogKey(BW::FindDialogGlobal(menu == BW::GLUE_READY_Z ? "ReadyZ" : "TerranRR")->findIndex(13));
    break;
  } // menu switch
}

void AutoMenuManager::pressDialogKey(BW::dialog *pDlg)
{
  if (pDlg)
    BroodwarImpl.pressKey(pDlg->getHotkey());
}

const char* AutoMenuManager::interceptFindFirstFile(const char* lpFileName)
{
  if (!autoMenuMapPath.empty() &&
    autoMenuMode != ""       &&
    autoMenuMode != "OFF"    &&
    !lastMapGen.empty() &&
    strstr(lpFileName, "*.*"))
  {
    lpFileName = lastMapGen.c_str();

    // Get the directory that the map is in
    std::string directoryPath = std::filesystem::path(installPath() + lastMapGen).parent_path().string();

    // update map folder location
    SStrCopy(BW::BWDATA::CurrentMapFolder.data(), directoryPath.c_str(), MAX_PATH);
  }
  return lpFileName;
}
