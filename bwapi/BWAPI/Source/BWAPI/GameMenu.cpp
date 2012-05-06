#include "GameImpl.h"

#include "Config.h"
#include "DLLMain.h"
#include "NewHackUtil.h"

#include <BW/MenuPosition.h>
#include <BW/PlayerType.h>

#include "../../Debug.h"

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

    char buffer[MAX_PATH];
    std::string cfgMap = LoadConfigString("auto_menu", "map", "");
    strncpy(buffer, cfgMap.c_str(), MAX_PATH);

    for ( int i = strlen(buffer); i; --i )
    {
      if ( buffer[i] == '/' )
        buffer[i] = '\\';
    }
    if ( lastAutoMapString != buffer )
    {
      lastAutoMapString = buffer;

      char szMapPathTemp[MAX_PATH];
      strcpy(szMapPathTemp, buffer);
      char *pszPathEnd = strrchr(szMapPathTemp, '\\');
      if ( pszPathEnd )
        pszPathEnd[1] = 0;
      autoMenuMapPath = std::string(szMapPathTemp);

      autoMapPool.clear();
      if ( autoMenuMapPath.size() > 0 )
      {
        WIN32_FIND_DATA finder = { 0 };

        HANDLE hFind = FindFirstFile(buffer, &finder);
        if ( hFind != INVALID_HANDLE_VALUE )
        {
          BOOL bResult = TRUE;
          while ( bResult )
          {
            // Check if found is not a directory
            if ( !(finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
              std::string finderStr = std::string(finder.cFileName);
              if ( getFileType((autoMenuMapPath + finderStr).c_str()) )
                autoMapPool.push_back( finderStr );
            }
            bResult = FindNextFile(hFind, &finder);
          } // ^ loop
          FindClose(hFind);
        } // handle exists
      } // map path exists
      lastAutoMapEntry = 0;
    }
    std::string newMapIteration = LoadConfigString("auto_menu", "mapiteration", "RANDOM");
    if ( autoMapIteration != newMapIteration )
    {
      autoMapIteration = newMapIteration;
      lastAutoMapEntry = 0;
    }

    autoMenuLanMode       = LoadConfigString("auto_menu", "lan_mode", "Local Area Network (UDP)");
    autoMenuRace          = LoadConfigString("auto_menu", "race", "RANDOM");
    autoMenuEnemyRace[0]  = LoadConfigString("auto_menu", "enemy_race", "RANDOM");
    for ( int i = 1; i < 8; ++i )
    {
      char key[16];
      sprintf(key, "enemy_race_%u", i);
      autoMenuEnemyRace[i] = LoadConfigString("auto_menu", key, "DEFAULT");
      if ( autoMenuEnemyRace[i] == "DEFAULT" )
        autoMenuEnemyRace[i] = autoMenuEnemyRace[0];
    }

    autoMenuEnemyCount = LoadConfigInt("auto_menu", "enemy_count", 1);
    if ( autoMenuEnemyCount > 7 )
      autoMenuEnemyCount = 7;
    autoMenuGameType    = LoadConfigString("auto_menu", "game_type", "MELEE");
    autoMenuSaveReplay  = LoadConfigString("auto_menu", "save_replay");

    autoMenuMinPlayerCount = LoadConfigInt("auto_menu", "wait_for_min_players", 2);
    autoMenuMaxPlayerCount = LoadConfigInt("auto_menu", "wait_for_max_players", 8);
    autoMenuWaitPlayerTime = LoadConfigInt("auto_menu", "wait_for_time", 30000);

    this->chooseNewRandomMap();
  }
  void GameImpl::chooseNewRandomMap()
  {
    if ( this->autoMapPool.size() > 0 )
    {
      // Obtain a random map file
      srand(GetTickCount());

      int chosenEntry = 0;
      if ( autoMapIteration == "RANDOM" )
      {
        chosenEntry = rand() % this->autoMapPool.size();
      }
      else if ( autoMapIteration == "SEQUENCE" )
      {
        if ( lastAutoMapEntry >= this->autoMapPool.size() )
          lastAutoMapEntry = 0;
        chosenEntry = lastAutoMapEntry++;
      }
      std::string chosen = this->autoMapPool[chosenEntry];
      lastMapGen         = this->autoMenuMapPath + chosen;
    }
  }
  //--------------------------------------------- GET LOBBY STUFF --------------------------------------------
  unsigned int getLobbyPlayerCount()
  {
    unsigned int rval = 0;
    for ( unsigned int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA_Players[i].nType == BW::PlayerType::Player  && BW::BWDATA_PlayerDownloadStatus[i] >= 100 )
        ++rval;
    }
    return rval;
  }
  unsigned int getLobbyOpenCount()
  {
    unsigned int rval = 0;
    for ( unsigned int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      if ( BW::BWDATA_Players[i].nType == BW::PlayerType::EitherPreferHuman )
        ++rval;
    }
    return rval;
  }
  DWORD createdTimer;
  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    //this function is called each frame while starcraft is in the main menu system (not in-game).
    this->inGame        = false;

    // Get races so we can catch random
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i )
    {
      Race r = Races::None;
      int _r = _getLobbyRace(i);
      switch ( _r )
      {
      case BW::Race::Zerg:
      case BW::Race::Terran:
      case BW::Race::Protoss:
        r = Race(_r);
        break;
      case BW::Race::Random:
        r = Races::Random;
        break;
      case BW::Race::None:
        break;
      default:
        r = Races::Unknown;
        break;
      }
      if ( _r != BW::Race::None )
        lastKnownRaceBeforeStart[i] = r;
    }

    events.push_back(Event::MenuFrame());
    this->server.update();

    // Don't attempt auto-menu if we run into 50 error message boxes
    if ( autoMapTryCount > 50 )
      return;

    // Return if autoMenu is not enabled
    if ( autoMenuMode == "" || autoMenuMode == "OFF" )
      return;

#ifdef _DEBUG
    // Wait for a debugger if autoMenuPause is enabled, and in DEBUG
    BOOL (*_IsDebuggerPresent)();
    *(FARPROC*)&_IsDebuggerPresent = HackUtil::GetImport("Kernel32", "IsDebuggerPresent");

    if ( autoMenuPause != "OFF" && _IsDebuggerPresent && !_IsDebuggerPresent() )
      return;
#endif

    // Get the menu mode
    int menu = *BW::BWDATA_glGluesMode;

    // Declare a commonly used dialog pointer
    BW::dialog *tempDlg = NULL;

    // Get some autoMenu properties
    bool isAutoSingle = autoMenuMode == "SINGLE_PLAYER";
    bool isCreating   = autoMenuMapPath.length() > 0;
    bool isJoining    = autoMenuGameName.length() > 0;

    // Reset raceSel flag
    if ( menu != BW::GLUE_CHAT )
    {
      actRaceSel = false;
      actStartedGame = false;
    }

    // Iterate through the menus
    switch ( menu )
    {
    case BW::GLUE_MAIN_MENU:    // Main menu
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

      if ( this->lastMapGen.size() > 0 )
      {
        if ( getFileType(this->lastMapGen.c_str()) == 1 )
        {
          // convert to game type
          GameType gt = GameTypes::getGameType(this->autoMenuGameType);

          // retrieve gametype dropdown
          BW::dialog *gameTypeDropdown = tempDlg->findIndex(17);
          if ( gt != GameTypes::None && gt != GameTypes::Unknown && (int)gameTypeDropdown->getSelectedValue() != gt )
            gameTypeDropdown->setSelectedByValue(gt);

          // if this is single player
          if ( isAutoSingle )
          {
            // get race
            Race playerRace = Races::getRace(this->autoMenuRace);
            if ( this->autoMenuRace == "RANDOMTP" )
              playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
            else if ( this->autoMenuRace == "RANDOMTZ" )
              playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
            else if ( this->autoMenuRace == "RANDOMPZ" )
              playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

            // set race dropdown
            if ( playerRace != Races::Unknown && playerRace != Races::None )
              this->_changeRace(0, playerRace);

            for ( unsigned int i = 1; i <= this->autoMenuEnemyCount; ++i )
            {
              Race enemyRace = Races::getRace(this->autoMenuEnemyRace[i]);
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
        char mapName[MAX_PATH] = { 0 };
        sprintf_s(mapName, MAX_PATH, "%s%s", szInstallPath, lastMapGen.c_str());

        // get the filename
        char *pszFile = mapName;
        // Go to last backslash
        char *pszTmp  = strrchr(pszFile, '\\');
        if ( pszTmp )
          pszFile = &pszTmp[1];

        // go to last forward slash (after any backslashes)
        pszTmp  = strrchr(pszFile, '/');
        if ( pszTmp )
          pszFile = &pszTmp[1];

        // Apply the altered name to all vector entries
        for ( BW::BlizzVectorEntry<BW::MapVectorEntry> *i = BW::BWDATA_MapListVector->begin; (u32)i != ~(u32)&BW::BWDATA_MapListVector->end && (u32)i != (u32)&BW::BWDATA_MapListVector->begin; i = i->next )
        {
          i->container.bTotalPlayers  = 8;
          i->container.bHumanSlots    = 8;
          for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
            i->container.bPlayerSlotEnabled[p] = 1;
          SStrCopy(i->container.szEntryName, pszFile, 65);
          SStrCopy(i->container.szFileName,  pszFile, MAX_PATH);
          SStrCopy(i->container.szFullPath,  mapName, MAX_PATH);
        }

        // update map folder location
        SStrCopy(BW::BWDATA_CurrentMapFolder, mapName, MAX_PATH);
        // Go to last backslash
        char *pszPos = BW::BWDATA_CurrentMapFolder;
        pszTmp = strrchr(pszPos, '\\');
        if ( pszTmp )
          pszPos = pszTmp;

        // go to last forward slash (after any backslashes)
        pszTmp  = strrchr(pszPos, '/');
        if ( pszTmp )
          pszPos = pszTmp;
        
        // Trim the "file"
        if ( pszPos != BW::BWDATA_CurrentMapFolder )
          pszPos[0] = 0;

        // if we encounter an unknown error when attempting to load the map
        if ( BW::FindDialogGlobal("gluPOk") )
        {
          this->chooseNewRandomMap();
          ++autoMapTryCount;
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

      break;
    case BW::GLUE_GAME_SELECT:  // Games listing
      {
        tempDlg = BW::FindDialogGlobal("GameSel");

        isHost = !(isJoining && tempDlg->findIndex(5)->setSelectedByString(autoMenuGameName.c_str()));

        if ( isCreating && isHost )
          this->pressKey( tempDlg->findIndex(15)->getHotkey() );  // Create Game
        else // is joining
          this->pressKey( tempDlg->findIndex(13)->getHotkey() );  // OK
      }
      break;
    case BW::GLUE_CHAT:
      if ( !actRaceSel && BW::FindDialogGlobal("Chat") && _currentPlayerId() >= 0 && _currentPlayerId() < 8 )
      {
        // Determine the current player's race
        Race playerRace = Races::getRace(this->autoMenuRace);
        if ( this->autoMenuRace == "RANDOMTP" )
          playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
        else if ( this->autoMenuRace == "RANDOMTZ" )
          playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
        else if ( this->autoMenuRace == "RANDOMPZ" )
          playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

        if ( playerRace != Races::Unknown && playerRace != Races::None )
        {
          // Check if the race was selected correctly, and prevent further changing afterwords
          u8 currentRace = BW::BWDATA_Players[_currentPlayerId()].nRace;
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
            actRaceSel = true;

          // Set the race
          if ( !actRaceSel )
            this->_changeRace(8, playerRace);
        }// if player race is valid
      } // if dialog "chat" exists

      // Start the game if creating and auto-menu requirements are met
      if ( isCreating && 
            !actStartedGame && 
            isHost && 
            getLobbyPlayerCount() > 0 && 
            (getLobbyPlayerCount() >= this->autoMenuMinPlayerCount || getLobbyOpenCount() == 0) )
      {
        if ( getLobbyPlayerCount() >= this->autoMenuMaxPlayerCount || getLobbyOpenCount() == 0 || GetTickCount() > createdTimer + this->autoMenuWaitPlayerTime )
        {
          DWORD dwMode = 0;
          SNGetGameInfo(GAMEINFO_MODEFLAG, dwMode);
          if ( !(dwMode & GAMESTATE_STARTED) )
          {
            actStartedGame = true;
            SNetSetGameMode(dwMode | GAMESTATE_STARTED);
            QUEUE_COMMAND(BW::Orders::StartGame);
          } // if game not started
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
  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    // Send the Change Slot command for multi-player
    QUEUE_COMMAND(BW::Orders::ChangeSlot, slot, slotID);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void  GameImpl::_changeRace(int slot, BWAPI::Race race)
  {
    // Obtain the single player dialog
    BW::dialog *custom = BW::FindDialogGlobal("Create");
    if ( custom )
    {
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
    if ( txt && strlen(txt) > 0 && txt[0] < '2' )
      return; // return if the countdown is less than 2
    
    // Send the change race command for multi-player
    QUEUE_COMMAND(BW::Orders::RequestChangeRace, race, slot);
  }
  //-------------------------------------------- GET LOBBY RACE ----------------------------------------------
  int GameImpl::_getLobbyRace(int slot)
  {
    if ( !this->isMultiplayer() )
    {
      BW::dialog *custom = BW::FindDialogGlobal("Create");
      if ( custom )
      {
        // Get single player race
        BW::dialog *slotCtrl = custom->findIndex((short)(28 + slot));  // 28 is the CtrlID of the first slot
        if ( slotCtrl )
          return slotCtrl->getSelectedValue();
      }
    }
    else if ( *BW::BWDATA_glGluesMode == BW::GLUE_CHAT )
    {
      return BW::BWDATA_Players[slot].nRace;
    }
    return BW::Race::None;
  }
}

