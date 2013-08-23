#include "GameImpl.h"

#include "../Config.h"
#include "../DLLMain.h"
#include "../NewHackUtil.h"

#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>

#include <BW/MenuPosition.h>
#include <BW/Dialog.h>
#include <BW/OrderTypes.h>
#include <Util/clamp.h>

#include "../../../Debug.h"

namespace BWAPI
{
  //------------------------------------------- LOAD AUTO MENU DATA ------------------------------------------
  void GameImpl::loadAutoMenuData()
  {
    //this function is called when starcraft loads and at the end of each match.
    //the function loads the parameters for the auto-menu feature such as auto_menu, map, race, enemy_race, enemy_count, and game_type    
    this->autoMenuMode = LoadConfigString("auto_menu", "auto_menu", "OFF");
#ifdef _DEBUG
    this->autoMenuPause = LoadConfigString("auto_menu", "pause_dbg", "OFF");
#endif
    this->autoMenuRestartGame = LoadConfigString("auto_menu", "auto_restart", "OFF");
    this->autoMenuGameName    = LoadConfigString("auto_menu", "game");

    // Load map string
    std::string cfgMap = LoadConfigString("auto_menu", "map", "");
    std::replace(cfgMap.begin(), cfgMap.end(), '/', '\\');

    // If the auto-menu map field was changed
    if ( this->lastAutoMapString != cfgMap )
    {
      this->lastAutoMapString = cfgMap;

      // Get just the directory
      this->autoMenuMapPath.clear();
      size_t tmp = cfgMap.find_last_of("\\/\n");
      if ( tmp != std::string::npos )
        this->autoMenuMapPath = cfgMap.substr(0, tmp);
      this->autoMenuMapPath += "\\";
      
      // Iterate files in directory
      WIN32_FIND_DATA finder = { 0 };
      HANDLE hFind = FindFirstFile(cfgMap.c_str(), &finder);
      
      if ( hFind != INVALID_HANDLE_VALUE )
      {
        do
        {
          if ( !(finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )  // Check if found is not a directory
          {
            // Convert to string and add to autoMapPool if the type is valid
            std::string finderStr = std::string(finder.cFileName);
            if ( getFileType((this->autoMenuMapPath + finderStr).c_str()) )
            {
              this->autoMapPool.push_back( finderStr );
            }
          }
        } while ( FindNextFile(hFind, &finder) );
        FindClose(hFind);
      } // handle exists
      
      this->lastAutoMapEntry = 0;
    } // if map was changed^

    // Get map iteration config
    std::string newMapIteration = LoadConfigString("auto_menu", "mapiteration", "RANDOM");
    if ( this->autoMapIteration != newMapIteration )
    {
      this->autoMapIteration = newMapIteration;
      this->lastAutoMapEntry = 0;
    }

    autoMenuLanMode       = LoadConfigString("auto_menu", "lan_mode", "Local Area Network (UDP)");
    autoMenuRace          = LoadConfigString("auto_menu", "race", "RANDOM");
    autoMenuEnemyRace[0]  = LoadConfigString("auto_menu", "enemy_race", "RANDOM");
    for ( unsigned int i = 1; i < 8; ++i )
    {
      std::stringstream sskey;
      sskey << "enemy_race_" << i;
      autoMenuEnemyRace[i] = LoadConfigString("auto_menu", sskey.str().c_str(), "DEFAULT");
      if ( autoMenuEnemyRace[i] == "DEFAULT" )
        autoMenuEnemyRace[i] = autoMenuEnemyRace[0];
    }

    autoMenuEnemyCount  = clamp<int>(LoadConfigInt("auto_menu", "enemy_count", 1), 0, 7);
    autoMenuGameType    = LoadConfigString("auto_menu", "game_type", "MELEE");
    autoMenuSaveReplay  = LoadConfigString("auto_menu", "save_replay");

    autoMenuMinPlayerCount = LoadConfigInt("auto_menu", "wait_for_min_players", 2);
    autoMenuMaxPlayerCount = LoadConfigInt("auto_menu", "wait_for_max_players", 8);
    autoMenuWaitPlayerTime = LoadConfigInt("auto_menu", "wait_for_time", 30000);

    this->chooseNewRandomMap();
  }
  void GameImpl::chooseNewRandomMap()
  {
    if ( !this->autoMapPool.empty() )
    {
      // Obtain a random map file
      srand(GetTickCount());

      int chosenEntry = 0;
      if ( this->autoMapIteration == "RANDOM" )
      {
        chosenEntry = rand() % this->autoMapPool.size();
      }
      else if ( this->autoMapIteration == "SEQUENCE" )
      {
        if ( this->lastAutoMapEntry >= this->autoMapPool.size() )
          this->lastAutoMapEntry = 0;
        chosenEntry = this->lastAutoMapEntry++;
      }
      std::string chosen = this->autoMapPool[chosenEntry];
      this->lastMapGen   = this->autoMenuMapPath + chosen;
    }
  }
  //--------------------------------------------- GET LOBBY STUFF --------------------------------------------
  unsigned int getLobbyPlayerCount()
  {
    unsigned int rval = 0;
    for ( unsigned int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA::Players[i].nType == PlayerTypes::Player )
        ++rval;
    }
    return rval;
  }
  unsigned int getLobbyPlayerReadyCount()
  {
    unsigned int rval = 0;
    for ( unsigned int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA::Players[i].nType == PlayerTypes::Player && BW::BWDATA::PlayerDownloadStatus[i] >= 100 )
        ++rval;
    }
    return rval;
  }
  unsigned int getLobbyOpenCount()
  {
    unsigned int rval = 0;
    for ( unsigned int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA::Players[i].nType == PlayerTypes::EitherPreferHuman )
        ++rval;
    }
    return rval;
  }
  DWORD createdTimer;
  DWORD waitJoinTimer;
  DWORD waitSelRaceTimer;
  DWORD waitRestartTimer;
  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    //this function is called each frame while starcraft is in the main menu system (not in-game).
    this->inGame        = false;

    events.push_back(Event::MenuFrame());
    this->server.update();

    // Don't attempt auto-menu if we run into 50 error message boxes
    if ( this->autoMapTryCount > 50 )
      return;

    // Return if autoMenu is not enabled
    if ( this->autoMenuMode == "" || this->autoMenuMode == "OFF" )
      return;

#ifdef _DEBUG
    // Wait for a debugger if autoMenuPause is enabled, and in DEBUG
    decltype(&IsDebuggerPresent) _IsDebuggerPresent;
    (FARPROC&)_IsDebuggerPresent = HackUtil::GetImport("Kernel32", "IsDebuggerPresent");

    if ( this->autoMenuPause != "OFF" && _IsDebuggerPresent && !_IsDebuggerPresent() )
      return;
#endif

    // Get the menu mode
    int menu = *BW::BWDATA::glGluesMode;

    // Declare a commonly used dialog pointer
    BW::dialog *tempDlg = nullptr;

    // Get some autoMenu properties
    bool isAutoSingle = this->autoMenuMode == "SINGLE_PLAYER";
    bool isCreating   = !this->autoMenuMapPath.empty();
    bool isJoining    = !this->autoMenuGameName.empty();

    // Reset raceSel flag
    if ( menu != BW::GLUE_CHAT )
    {
      this->actRaceSel = false;
      this->actStartedGame = false;
    }

    // Iterate through the menus
    switch ( menu )
    {
    case BW::GLUE_MAIN_MENU:    // Main menu
      if ( BW::FindDialogGlobal("TitleDlg") ) // skip if at title
        break;

      // Choose single or multi
      this->pressKey(BW::FindDialogGlobal("MainMenu")->findIndex(isAutoSingle ? 3 : 4)->getHotkey() );

      // choose original or expansion (we always choose expansion)
      if ( BW::FindDialogGlobal("Delete") )
        this->pressKey( BW::FindDialogGlobal("Delete")->findIndex(7)->getHotkey() );
      break;
    case BW::GLUE_EX_CAMPAIGN:  // Campaign selection menu
    case BW::GLUE_CAMPAIGN:
      // Choose "Custom"
      this->pressKey( BW::FindDialogGlobal("RaceSelection")->findIndex(10)->getHotkey() );
      break;
    case BW::GLUE_CREATE:       // Game creation menu
    case BW::GLUE_CREATE_MULTI:
      // Store the tick count while in this menu, and refer to it in the next
      createdTimer  = GetTickCount();
      tempDlg = BW::FindDialogGlobal("Create");

      if ( !this->lastMapGen.empty() )
      {
        if ( getFileType(this->lastMapGen.c_str()) == 1 )
        {
          // convert to game type
          GameType gt = GameType::getType(this->autoMenuGameType);

          // retrieve gametype dropdown
          BW::dialog *gameTypeDropdown = tempDlg->findIndex(17);
          if ( gt != GameTypes::None && gt != GameTypes::Unknown && (int)gameTypeDropdown->getSelectedValue() != gt )
            gameTypeDropdown->setSelectedByValue(gt);

          // if this is single player
          if ( isAutoSingle )
          {
            // get race
            Race playerRace;
            if ( this->autoMenuRace == "RANDOMTP" )
              playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
            else if ( this->autoMenuRace == "RANDOMTZ" )
              playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
            else if ( this->autoMenuRace == "RANDOMPZ" )
              playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;
            else
              playerRace = Race::getType(this->autoMenuRace);

            // set race dropdown
            if ( playerRace != Races::Unknown && playerRace != Races::None )
              this->_changeRace(0, playerRace);

            for ( unsigned int i = 1; i <= this->autoMenuEnemyCount; ++i )
            {
              Race enemyRace = Race::getType(this->autoMenuEnemyRace[i]);
              if ( this->autoMenuEnemyRace[i] == "RANDOMTP" )
                enemyRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
              else if ( this->autoMenuEnemyRace[i] == "RANDOMTZ" )
                enemyRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
              else if ( this->autoMenuEnemyRace[i] == "RANDOMPZ" )
                enemyRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

              if ( enemyRace != Races::Unknown && enemyRace != Races::None )
                this->_changeRace(i, enemyRace);
            }
            //close remaining slots
            for( int i = this->autoMenuEnemyCount; i < 7; ++i )
            {
              BW::dialog *slot = tempDlg->findIndex((short)(21 + i));
              if ( slot->getSelectedIndex() != 0 )
                slot->setSelectedIndex(0);
            }
          } // if single
        } // if map is playable

        // get the full map path
        std::string mapFilePath = installPath + lastMapGen;
        
        // Get substring containing only the file name
        size_t tmp = mapFilePath.find_last_of("/\\");
        std::string mapFileName(mapFilePath, tmp == std::string::npos ? 0 : tmp+1);

        // Get substring containing only the directory
        std::string mapFileDir(mapFilePath, 0, mapFilePath.size() - mapFileName.size() - 1);
        
        // Apply the altered name to all vector entries
        for ( BW::BlizzVectorEntry<BW::MapVectorEntry> *i = BW::BWDATA::MapListVector->begin; (u32)i != ~(u32)&BW::BWDATA::MapListVector->end && (u32)i != (u32)&BW::BWDATA::MapListVector->begin; i = i->next )
        {
          i->container.bTotalPlayers  = 8;
          i->container.bHumanSlots    = 8;
          for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
            i->container.bPlayerSlotEnabled[p] = 1;
          i->container.bEntryFlags = 0x04;
          SStrCopy(i->container.szEntryName, mapFileName.c_str(), sizeof(i->container.szEntryName));
          SStrCopy(i->container.szFileName,  mapFileName.c_str(), sizeof(i->container.szFileName)); // @TODO verify
          SStrCopy(i->container.szFullPath,  mapFilePath.c_str(), sizeof(i->container.szFullPath));
        }

        // update map folder location
        SStrCopy(BW::BWDATA::CurrentMapFolder, mapFileDir.c_str(), MAX_PATH);
        
        // if we encounter an unknown error when attempting to load the map
        if ( BW::FindDialogGlobal("gluPOk") )
        {
          this->chooseNewRandomMap();
          ++this->autoMapTryCount;
          this->pressKey(BW::FindDialogGlobal("gluPOk")->findIndex(1)->getHotkey());
        }
        this->pressKey( tempDlg->findIndex(12)->getHotkey() );
      } // if lastmapgen
      break;
    case BW::GLUE_CONNECT:
      tempDlg = BW::FindDialogGlobal("ConnSel");

      // Press hotkey if trying to get to BNET
      // or press it after the LAN mode has been selected
      if ( autoMenuMode == "BATTLE_NET" ||
           (tempDlg->findIndex(5)->isVisible() && 
           tempDlg->findIndex(5)->setSelectedByString(autoMenuLanMode.c_str()) )  )
        pressKey( tempDlg->findIndex(9)->getHotkey() );

      waitJoinTimer = 0;
      break;
    case BW::GLUE_GAME_SELECT:  // Games listing
      {
        if ( waitJoinTimer == 0 )
          waitJoinTimer = GetTickCount();

        tempDlg = BW::FindDialogGlobal("GameSel");
        if ( !tempDlg )
          break;

        if ( isJoining &&
             !tempDlg->findIndex(5)->setSelectedByString(autoMenuGameName.c_str()) && 
             waitJoinTimer + 3000 > GetTickCount() )
          break;

        waitJoinTimer = GetTickCount();
        isHost = !(isJoining && tempDlg->findIndex(5)->setSelectedByString(autoMenuGameName.c_str()));

        if ( isCreating && isHost )
          this->pressKey( tempDlg->findIndex(15)->getHotkey() );  // Create Game
        else // is joining
          this->pressKey( tempDlg->findIndex(13)->getHotkey() );  // OK
      }
      break;
    case BW::GLUE_CHAT:
      waitJoinTimer = 0;
      
      if ( !actRaceSel && 
            BW::FindDialogGlobal("Chat") && 
            _currentPlayerId() >= 0 && 
            _currentPlayerId() < 8 &&
            waitSelRaceTimer + 300 < GetTickCount() )
      {
        waitSelRaceTimer = GetTickCount();

        // Determine the current player's race
        Race playerRace;
        if ( this->autoMenuRace == "RANDOMTP" )
          playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
        else if ( this->autoMenuRace == "RANDOMTZ" )
          playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
        else if ( this->autoMenuRace == "RANDOMPZ" )
          playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;
        else
          playerRace = Race::getType(this->autoMenuRace);

        if ( playerRace != Races::Unknown && playerRace != Races::None )
        {
          // Check if the race was selected correctly, and prevent further changing afterwords
          u8 currentRace = BW::BWDATA::Players[_currentPlayerId()].nRace;

          if ( (currentRace == playerRace ||
            (this->autoMenuRace == "RANDOMTP" &&
            ( currentRace == Races::Terran ||
              currentRace == Races::Protoss)) ||
            (this->autoMenuRace == "RANDOMTZ" &&
            ( currentRace == Races::Terran ||
              currentRace == Races::Zerg)) ||
            (this->autoMenuRace == "RANDOMPZ" &&
            ( currentRace == Races::Protoss ||
              currentRace == Races::Zerg))
             ) )
          {
            actRaceSel = true;
          }
          
          // Set the race
          if ( !actRaceSel )
            this->_changeRace(8, playerRace);
        }// if player race is valid
      } // if dialog "chat" exists


      if ( BW::FindDialogGlobal("gluPOk") )
      {
        this->pressKey(BW::FindDialogGlobal("gluPOk")->findIndex(1)->getHotkey());
        actStartedGame = false;
        waitRestartTimer = GetTickCount();
      }

      // Start the game if creating and auto-menu requirements are met
      if (  isCreating && 
            waitRestartTimer + 2000 < GetTickCount() &&
            !actStartedGame && 
            isHost && 
            getLobbyPlayerReadyCount() > 0 && 
            getLobbyPlayerReadyCount() == getLobbyPlayerCount() && 
            (getLobbyPlayerReadyCount() >= this->autoMenuMinPlayerCount || getLobbyOpenCount() == 0) )
      {
        if ( getLobbyPlayerReadyCount() >= this->autoMenuMaxPlayerCount || getLobbyOpenCount() == 0 || GetTickCount() > createdTimer + this->autoMenuWaitPlayerTime )
        {
          if ( !BW::FindDialogGlobal("Chat")->findIndex(7)->isDisabled() )
          {
            actStartedGame = true;
            BW::FindDialogGlobal("Chat")->findIndex(7)->activate();
          }
        } // if lobbyPlayerCount etc
      } // if isCreating etc
      break;
    case BW::GLUE_LOGIN:  // Registry/Character screen
      // Type in "BWAPI" if no characters available
      tempDlg = BW::FindDialogGlobal("gluPEdit");
      if ( tempDlg )
      {
        tempDlg->findIndex(4)->setText("BWAPI");
        this->pressKey( tempDlg->findIndex(1)->getHotkey() );
      }
      else
      {
        this->pressKey( BW::FindDialogGlobal("Login")->findIndex(4)->getHotkey() );
      }
      break;
    case BW::GLUE_SCORE_Z_DEFEAT: 
    case BW::GLUE_SCORE_Z_VICTORY:
    case BW::GLUE_SCORE_T_DEFEAT:
    case BW::GLUE_SCORE_T_VICTORY:
    case BW::GLUE_SCORE_P_DEFEAT:
    case BW::GLUE_SCORE_P_VICTORY:
      if ( autoMenuRestartGame != "" && autoMenuRestartGame != "OFF" )
        this->pressKey( BW::FindDialogGlobal("End")->findIndex(7)->getHotkey() );
      break;
    case BW::GLUE_READY_T:  // Mission Briefing
    case BW::GLUE_READY_Z:
    case BW::GLUE_READY_P:
      this->pressKey( BW::FindDialogGlobal(menu == BW::GLUE_READY_Z ? "ReadyZ" : "TerranRR")->findIndex(13)->getHotkey() );
      break;
    } // menu switch
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::_changeRace(int slot, BWAPI::Race race)
  {
    // Obtain the single player dialog
    BW::dialog *custom = BW::FindDialogGlobal("Create");
    if ( custom )
    {
      slot = clamp(slot, 0, 7);
      // Apply the single player change
      BW::dialog *slotCtrl = custom->findIndex((short)(28 + slot));  // 28 is the CtrlID of the first slot
      if ( slotCtrl && (int)slotCtrl->getSelectedValue() != race )
        slotCtrl->setSelectedByValue(race);
      return;
    }

    // Obtain the multi-player dialog
    custom = BW::FindDialogGlobal("Chat");
    if ( !custom ) // return if not found
      return;

    // Obtain the countdown control
    BW::dialog *countdown = custom->findIndex(24);
    if ( !countdown ) // return if not found
      return;

    // Obtain the countdown control's text
    char *txt = countdown->getText();
    if ( txt && txt[0] && txt[0] < '2' )
      return; // return if the countdown is less than 2
    
    // Send the change race command for multi-player
    QUEUE_COMMAND(BW::Orders::RequestChangeRace, slot, race);
  }
}

