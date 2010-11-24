#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "../../starcraftver.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <ddraw.h>

#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/BulletImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Flag.h>
#include <BWAPI.h>

#include <BW/Unit.h>
#include <BW/Order.h>
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>
#include <BW/Dialog.h>
#include <BW/Path.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"
#include "WMode.h"
#include "Resolution.h"
#include "NewHackUtil.h"

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"
#include "BWtoBWAPI.h"
/*
  This files holds all functions of the GameImpl class that are not part of the Game interface.
 */

#define commandIterator(lst,cmd) {\
  u8 uCount = 0;\
  BWAPI::UnitImpl *units[13];\
  foreach(UnitImpl *u, lst)\
  {\
    units[uCount] = u;\
    ++uCount;\
    if ( uCount == 12 )\
    {\
      BW::Orders::Select sel = BW::Orders::Select(uCount, units);\
      QueueGameCommand((PBYTE)&sel, sel.size);\
      QueueGameCommand((PBYTE)&BW::Orders::cmd(), sizeof(BW::Orders::cmd));\
      uCount = 0;\
    }\
  }\
  if ( uCount )\
  {\
    BW::Orders::Select sel = BW::Orders::Select(uCount, units);\
    QueueGameCommand((PBYTE)&sel, sel.size);\
    QueueGameCommand((PBYTE)&BW::Orders::cmd(), sizeof(BW::Orders::cmd));\
    uCount = 0;\
  }\
  lst.clear();\
}

namespace BWAPI
{
  Game* Broodwar;
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
      : onStartCalled(false)
      , unitsOnTileData(0, 0)
      , enabled(true)
      , client(NULL)
      , startedClient(false)
      , inGame(false)
      , frameCount(-1)
      , endTick(0)
      , hasLatCom(true)
      , pathDebug(false)
      , unitDebug(false)
      , grid(false)
      , wantSelectionUpdate(false)
      , noGUI(false)
      , calledMatchEnd(false)
  {
    BWAPI::Broodwar = static_cast<Game*>(this);

    /* initialize the unit types */
    BW::UnitType::initialize();
    BWtoBWAPI_init();

    try
    {
      memset(savedUnitSelection, 0, sizeof(savedUnitSelection));

      /* iterate through players and create PlayerImpl for each */
      for (int i = 0; i < PLAYER_COUNT; i++)
        players[i] = new PlayerImpl((u8)i);

      /* iterate through units and create UnitImpl for each */
      for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[i],
                                    (u16)i);

      /* iterate through bullets and create BulletImpl for each */
      for (int i = 0; i < BULLET_ARRAY_MAX_LENGTH; i++)
        bulletArray[i] = new BulletImpl(&BW::BWDATA_BulletNodeTable->bullet[i],
                                        (u16)i);


      /* iterate through unit types and create UnitType for each */
      for (int i = 0; i < UNIT_TYPE_COUNT; i++)
        unitTypes.insert(BW::UnitType((u16)i));
    }
    catch (GeneralException& exception)
    {
      BWAPIError("Exception caught inside Game constructor: %s", exception.getMessage().c_str());
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    /* destroy all UnitImpl */
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    /* destroy all PlayerImpl */
    for (int i = 0; i < PLAYER_COUNT; i++)
      delete players[i];

    /* destroy all log handles */
    delete this->commandLog;
    delete this->newUnitLog;
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void GameImpl::update()
  {
    //this function is called every frame from a hook attached in DllMain.cpp
    this->inGame = true;

    //clear all shapes
    for (unsigned int i = 0; i < this->shapes.size(); ++i)
      delete this->shapes[i];
    this->shapes.clear();

#ifdef _DEBUG
    // menu dialog test update code
    if ( myDlg )
      myDlg->update();
#endif

    // Compute frame rate
    accumulatedFrames++;
    DWORD currentTickCount = GetTickCount();
    if ( currentTickCount >= lastTickCount + 1000 )
    {
      fps               = accumulatedFrames;
      averageFPS        = averageFPS*0.7+fps*0.3;
      lastTickCount     = currentTickCount;
      accumulatedFrames = 0;
    }

    try
    {
      //the first time update() is called, we also call onGameStart to initialize some things
      if ( !onStartCalled )
      {
        this->onGameStart();
        Util::Logger::globalLog->log("calling onGameStart");
      }
      
      if ( !this->enabled )
        return;

      if ( !this->calledMatchEnd && frameCount > 1)
      {
        bool win     = false;
        bool allDone = false;
        if ( this->BWAPIPlayer )
        {
          if ( this->BWAPIPlayer->isVictorious() )
          {
            win     = true;
            allDone = true;
          }
          if ( this->BWAPIPlayer->isDefeated() )
          {
            win     = false;
            allDone = true;
          }
            
        }
        else
        {
          allDone = true;
          foreach(Player* p, this->playerSet)
          {
            if ( ((PlayerImpl*)p)->getIndex() >= 8 )
              continue;
            if ( !p->isDefeated() && !p->isVictorious() && !p->leftGame() )
              allDone = false;
          }
        }
        if ( allDone)
        {
          this->calledMatchEnd = true;
          events.push_back(Event::MatchFrame());
          events.push_back(Event::MatchEnd(win));
          Util::Logger::globalLog->log("creating MatchEnd(%s) event", win ? "true" : "false");
          processEvents();
          server.update();
          events.clear();
          this->inGame = false;
          events.push_back(Event::MenuFrame());
          processEvents();
          server.update();
          events.clear();
        }
      }


      //don't have any more MatchFrame events after MatchEnd until MatchStart is called.
      if ( this->calledMatchEnd ) return;

      // Update unit selection
      if ( wantSelectionUpdate && memcmp(savedUnitSelection, BW::BWDATA_ClientSelectionGroup, sizeof(savedUnitSelection)) != 0 )
      {
        wantSelectionUpdate = false;
        memcpy(savedUnitSelection, BW::BWDATA_ClientSelectionGroup, sizeof(savedUnitSelection));
        refreshSelectionStates();
      }

      //update players and check to see if they have just left the game.
      for (int i = 0; i < PLAYER_COUNT; i++)
      {
        bool prevLeftGame = this->players[i]->leftGame();
        this->players[i]->updateData();
        if (!prevLeftGame && this->players[i]->leftGame())
          events.push_back(Event::PlayerLeft((Player*)this->players[i]));
      }
      //update properties of Unit and Bullet objects
      this->updateUnits();
      this->updateBullets();

      //iterate through the list of intercepted messages
      foreach(std::string i, sentMessages)
        BroodwarImpl.onSendText(i.c_str());

      //clear all intercepted messages
      this->sentMessages.clear();
    }
    catch (GeneralException& exception)
    {
      BWAPIError("Exception caught inside Game::update: %s", exception.getMessage().c_str());
    }

    //on the first frame we check to see if the client process has connected.
    //if not, then we load the AI dll specified in bwapi.ini
    if ( !this->startedClient )
    {
      externalModuleConnected = false;
      char *pszModuleName = "<Nothing>";
      if (server.isConnected()) //check to see if the server is connected to the client
      {
        Util::Logger::globalLog->logCritical("Client connected, not loading AI module.");
        this->client = new AIModule();
        printf("BWAPI: Connected to AI Client process");
        pszModuleName = "<Client Connection>";
        externalModuleConnected = true;
      }
      else // if not, load the AI module DLL
      {
        char szDllPath[MAX_PATH];
        char szKeyName[MAX_PATH];
        hMod = NULL;

        SStrCopy(szKeyName, "ai_dll", MAX_PATH);
        if ( gdwProcNum > 0 )
        {
          char tst[16];
          sprintf_s(tst, 16, "_%u", gdwProcNum);
          SStrNCat(szKeyName, tst, MAX_PATH);
        }

        if ( isDebug() )
          SStrNCat(szKeyName, "_dbg", MAX_PATH);

        GetPrivateProfileString("ai", szKeyName, "NULL", szDllPath, MAX_PATH, szConfigPath);
        if ( SStrCmpI(szDllPath, "NULL", MAX_PATH) == 0)
        {
          BWAPIError("Could not find %s under ai in \"%s\".", szKeyName, szConfigPath);
        }
        else
        {
          Util::Logger::globalLog->logCritical("Loading AI DLL from: %s", szDllPath);
          hMod = LoadLibrary(szDllPath);
        }
        if ( !hMod )
        {
          //if hMod is a null pointer, there there was a problem when trying to load the AI Module
          Util::Logger::globalLog->logCritical("ERROR: Failed to load the AI Module");
          this->client = new AIModule();
          Broodwar->enableFlag(Flag::CompleteMapInformation);
          Broodwar->enableFlag(Flag::UserInput);
          printf("%cERROR: Failed to load the AI Module.", 6);
          externalModuleConnected = false;
        }
        else
        {
          Util::Logger::globalLog->logCritical("Loaded AI Module");
          Util::Logger::globalLog->logCritical("Importing by Virtual Function Table from AI DLL");

          typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);
          Util::Logger::globalLog->logCritical("Creating an Object of AIModule");

          PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, TEXT("newAIModule"));
          if ( newAIModule )
          {
            this->client = newAIModule(this);
            Util::Logger::globalLog->logCritical("Created an Object of AIModule");
            printf("%cLoaded the AI Module: %s", 7, szDllPath);
            externalModuleConnected = true;

            pszModuleName = szDllPath;
            if ( strchr(pszModuleName, '/') )
              pszModuleName = &strrchr(pszModuleName, '/')[1];

            if ( strchr(pszModuleName, '\\') )
              pszModuleName = &strrchr(pszModuleName, '\\')[1];
          }
          else
          {
            Util::Logger::globalLog->logCritical("ERROR: Failed to find the newAIModule function in %s", szDllPath);
            this->client = new AIModule();
            Broodwar->enableFlag(Flag::CompleteMapInformation);
            Broodwar->enableFlag(Flag::UserInput);
            printf("%cERROR: Failed to find the newAIModule function in %s", 6, szDllPath);
            externalModuleConnected = false;
          }
        }
      }
      //push the MatchStart event to the front of the queue so that it is the first event in the queue.
      events.push_front(Event::MatchStart());
      this->startedClient = true;
      sendText("BWAPI r%s %s is now live using \"%s\".", 
                SVN_REV_STR, 
                isDebug() ? "DEBUG" : "RELEASE", 
                pszModuleName );
    }

    //each frame we add a MatchFrame event to the queue
    events.push_back(Event::MatchFrame());

    //if the AI is a DLL, processEvents() will translate the events into AIModule callbacks.
    processEvents();

    //if the AI is a client process, this will signal the client to process the next frame
    server.update();

    //Before returning control to starcraft, we clear the unit data for units that are no longer accessible
    foreach(UnitImpl* u, evadeUnits)
      u->updateData();

    //We also kill the units that are dying on this frame.
    //We wait until after server.update() and processEvents() to do this so that the AI can
    //access the last frame of unit data during the onUnitDestroy callback.
    foreach(UnitImpl* u, dyingUnits)
    {
      deadUnits.push_back(u);
      int index = u->getIndex();
      unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],(u16)index);
      u->die();
    }

    /* In case we ever want to add a Flag::UnitPermanence cheat flag...
    bool UnitPermanence = false;
    if (!UnitPermanence)
    {
      foreach(UnitImpl* u, evadeUnits)
      {
        deadUnits.push_back(u);
        int index = u->getIndex();
        unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],(u16)index);
        u->die();
      }
    }
    */

    //increment frame count if the game is not paused
    if ( *BW::BWDATA_IsRunning != 0 )
    {
      commandIterator(cmdToSiege,Siege);
      commandIterator(cmdToUnsiege,Unsiege);
      commandIterator(cmdToStop,Stop);
      commandIterator(cmdToHold,HoldPosition);
      commandIterator(cmdToBurrow,Burrow);
      commandIterator(cmdToUnburrow,Unburrow);
      commandIterator(cmdToCloak,Cloak);
      commandIterator(cmdToUncloak,Decloak);

      this->frameCount++;
    }

#ifdef _DEBUG
    setTextSize(0);
    // unitdebug
    if ( unitDebug )
    {
      foreach ( Unit *u, aliveUnits )
      {
        if ( !u )
          continue;
        if ( u->isStuck() )
          printf("A %s is stuck", u->getType().getName().c_str());
      }
      // selected units
      foreach( Unit *_u, selectedUnitSet )
      {
        if ( !_u )
          continue;

        BW::Unit *u = ((UnitImpl*)_u)->getOriginalRawData;
        if ( !u )
          continue;

        drawCircleMap(u->position.x, u->position.y, 2, BWAPI::Colors::Cyan, true);
        drawTextMap(u->position.x + 32, u->position.y - 24, "%s", BW::getMoveStateName(u->movementState));
        drawTextMap(u->position.x + 32, u->position.y - 12, "%s | %s", BWAPI::Order(BWtoBWAPI_Order[u->orderID]).getName().c_str(), BWAPI::Order(BWtoBWAPI_Order[u->secondaryOrderID]).getName().c_str());
        BW::Order *o = u->orderQueueHead;
        for ( int i = 0; o; ++i )
        {
          drawTextMap(u->position.x + 32, u->position.y + 12*i, "%s", BWAPI::Order(BWtoBWAPI_Order[o->orderID]).getName().c_str());
          o = o->nextOrder;
        }
      }


    } // unitdebug

    // grid
    if ( grid )
    {
      BWAPI::Position scrPos = getScreenPosition();

      // draw mtx grid
      for ( int y = scrPos.y()/32-1; y < (scrPos.y()+480)/32+1; ++y )
        for ( int x = scrPos.x()/32-1; x < (scrPos.x()+640)/32+1; ++x )
          for ( int i = 0; i < 32; i += 4 )
          {
            drawLineMap(x*32 + 32, y*32 + i, x*32 + 32, y*32 + i + 2, BWAPI::Colors::Grey);
            drawLineMap(x*32 + i, y*32 + 32, x*32 + i + 2, y*32 + 32, BWAPI::Colors::Grey);
          }
      if ( getReplayFrameCount() )
        drawTextScreen(30, 30, "%d/%d = %u%%", getFrameCount(), getReplayFrameCount(), (u32)(getFrameCount()*100/getReplayFrameCount()));
    } // grid

    // pathdebug
    if ( pathDebug && BW::BWDATA_SAIPathing )
    {
      setTextSize(0);
      BWAPI::Position mouse  = getMousePosition() + getScreenPosition();
      
      int tileY = mouse.y()/32;
      int tileX = mouse.x()/32;
      u16 idx = BW::BWDATA_SAIPathing->mapTileRegionId[tileY][tileX];
      if ( idx & 0x2000 )
      {
        BW::split *t = &BW::BWDATA_SAIPathing->splitTiles[idx & 0x1FFF];
        drawLineMap(tileX*32 + 16, tileY*32 + 16, BW::BWDATA_SAIPathing->regions[t->rgn1].rgnCenterX >> 8, BW::BWDATA_SAIPathing->regions[t->rgn2].rgnCenterY >> 8, BWAPI::Colors::Green);
        drawLineMap(tileX*32 + 16, tileY*32 + 16, BW::BWDATA_SAIPathing->regions[t->rgn2].rgnCenterX >> 8, BW::BWDATA_SAIPathing->regions[t->rgn1].rgnCenterY >> 8, BWAPI::Colors::Red);
        for ( int mTileY = 0; mTileY < 4; ++mTileY )
          for ( int mTileX = 0; mTileX < 4; ++mTileX )
          {
            BWAPI::Color c = BWAPI::Colors::Green;
            u16 rgn = t->rgn2;
            if ( (t->minitileMask >> ( mTileX + mTileY * 4 )) & 1 )
            {
              c = BWAPI::Colors::Red;
              rgn = t->rgn1;
            }
            drawLineMap(tileX * 32 + mTileX * 8,     tileY * 32 + mTileY * 8, tileX * 32 + mTileX * 8 + 8, tileY * 32 + mTileY * 8 + 8, c);
            drawLineMap(tileX * 32 + mTileX * 8 + 8, tileY * 32 + mTileY * 8, tileX * 32 + mTileX * 8,     tileY * 32 + mTileY * 8 + 8, c);
          }
        drawTextMouse(32, 32, "%u", idx & 0x1FFF);
      }

    } // pathdebug
#endif
    //finally return control to starcraft
  }
  //------------------------------------------- LOAD AUTO MENU DATA ------------------------------------------
  void GameImpl::loadAutoMenuData()
  {
    //this function is called when starcraft loads and at the end of each match.
    //the function loads the parameters for the auto-menu feature such as auto_menu, map, race, enemy_race, enemy_count, and game_type
    char buffer[MAX_PATH];
    GetPrivateProfileString("auto_menu", "auto_menu", "OFF", buffer, MAX_PATH, szConfigPath);
    this->autoMenuMode = std::string( strupr(buffer) );

    GetPrivateProfileString("auto_menu", "auto_restart", "OFF", buffer, MAX_PATH, szConfigPath);
    this->autoMenuRestartGame = std::string( strupr(buffer) );

    if ( autoMenuMode != "OFF" && autoMenuMode != "" )
    {
      GetPrivateProfileString("auto_menu", "map", "", buffer, MAX_PATH, szConfigPath);

      //split path into path and filename
      char* mapPathAndNameI         = buffer;
      char* mapPathAndNameLastSlash = buffer;
      while(mapPathAndNameI[0] != '\0')
      {
        if (mapPathAndNameI[0] == '\\' || mapPathAndNameI[0] == '/')
          mapPathAndNameLastSlash = mapPathAndNameI + 1;
        mapPathAndNameI++;
      }
      autoMenuMapName = std::string(mapPathAndNameLastSlash);
      mapPathAndNameLastSlash[0] = '\0';
      autoMenuMapPath = std::string(buffer);
    }
    GetPrivateProfileString("auto_menu", "lan_mode", "Local Area Network (UDP)", buffer, MAX_PATH, szConfigPath);
    autoMenuLanMode = std::string(buffer);
    GetPrivateProfileString("auto_menu", "race", "RANDOM", buffer, MAX_PATH, szConfigPath);
    autoMenuRace = std::string(strupr(buffer));
    GetPrivateProfileString("auto_menu", "enemy_race", "RANDOM", buffer, MAX_PATH, szConfigPath);
    autoMenuEnemyRace[0] = std::string(strupr(buffer));
    for ( int i = 1; i < 8; ++i )
    {
      char key[16];
      sprintf(key, "enemy_race_%u", i);
      GetPrivateProfileString("auto_menu", key, "DEFAULT", buffer, MAX_PATH, szConfigPath);
      autoMenuEnemyRace[i] = std::string(strupr(buffer));
      if ( autoMenuEnemyRace[i] == "DEFAULT" )
        autoMenuEnemyRace[i] = autoMenuEnemyRace[0];
    }

    autoMenuEnemyCount = GetPrivateProfileInt("auto_menu", "enemy_count", 1, szConfigPath);
    if      ( autoMenuEnemyCount > 7 ) autoMenuEnemyCount = 7;
    GetPrivateProfileString("auto_menu", "game_type", "MELEE", buffer, MAX_PATH, szConfigPath);
    autoMenuGameType = std::string(buffer);
    GetPrivateProfileString("auto_menu", "save_replay", "", buffer, MAX_PATH, szConfigPath);
    autoMenuSaveReplay = std::string(buffer);
  }
  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    //this function is called each frame while starcraft is in the main menu system (not in-game).
    this->inGame = false;
    events.push_back(Event::MenuFrame());
    this->server.update();
    int menu = *BW::BWDATA_glGluesMode;
    BW::dialog *tempDlg;
    if ( autoMenuMode == "SINGLE_PLAYER" )
    {
      switch ( menu )
      {
//main menu
      case 0: 
        if ( !actMainMenu )
        {
          actMainMenu = true;
          if ( !BW::FindDialogGlobal("MainMenu")->findIndex(3)->activate() )
            actMainMenu = false;
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();

        actRegistry = false;
        break;
//single player play custom / load replay selection screen
      case 22:
        actRegistry = false;
        SStrCopy(BW::BWDATA_menuMapRelativePath, autoMenuMapPath.c_str(), MAX_PATH);
        SStrCopy(BW::BWDATA_menuMapFileName, autoMenuMapName.c_str(), MAX_PATH);
        if ( !actRaceSel )
        {
          actRaceSel = true;
          if ( !BW::FindDialogGlobal("RaceSelection")->findIndex(10)->activate() )
            actRaceSel = false;
        }
        actCreate = false;
        break;
//create single/multi player game screen
      case 11: 
        actRaceSel = false;
        //the first time we enter the create game screen, it won't set the map correctly
        //so we need to cancel out and re-enter
        tempDlg = BW::FindDialogGlobal("Create");
        if ( *BW::BWDATA_menuStuff != -1 ) //Starcraft sets this to -1 after the first time we enter the create game screen
          this->pressKey( tempDlg->findIndex(13)->getHotkey() );
          //tempDlg->findIndex(13)->doEvent(14, 2);    // This is too efficient and will cause whatever trick being used to fail (infinite loop)
        else
        {
          GameType gt = GameTypes::getGameType(this->autoMenuGameType);
          if ( gt != GameTypes::None && gt != GameTypes::Unknown )
            tempDlg->findIndex(17)->setSelectedByValue(gt.getID());

          Race playerRace = Races::getRace(this->autoMenuRace);
          if ( this->autoMenuRace == "RANDOMTP" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
          else if ( this->autoMenuRace == "RANDOMTZ" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
          else if ( this->autoMenuRace == "RANDOMPZ" )
            playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

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
            tempDlg->findIndex((short)(21 + i))->setSelectedIndex(0);

          this->pressKey( tempDlg->findIndex(12)->getHotkey() );
        }
        break;
      }
    }
    else if (autoMenuMode == "LAN")
    {
      switch ( menu )
      {
//main menu
      case 0: 
        if ( !actMainMenu )
        {
          actMainMenu = true;
          if ( !BW::FindDialogGlobal("MainMenu")->findIndex(4)->activate() )
            actMainMenu = false;
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();

        actConnSel = false;
        break;
// Select connection
      case 2:
        actMainMenu = false;
        BW::dialog *connDlg = BW::FindDialogGlobal("ConnSel");

        if ( connDlg->findIndex(5)->isVisible() && 
             connDlg->findIndex(5)->setSelectedByString(autoMenuLanMode.c_str()) )
          pressKey( connDlg->findIndex(9)->getHotkey() );

        actRegistry = false;
        break;
      }

      if (autoMenuMapName.length() > 0)
      {
        switch ( menu )
        {
//lan games lobby
        case 10: 
          actRegistry = false;
          SStrCopy(BW::BWDATA_menuMapRelativePath, autoMenuMapPath.c_str(), MAX_PATH);
          SStrCopy(BW::BWDATA_menuMapFileName, autoMenuMapName.c_str(), MAX_PATH);
          if ( !actGameSel )
          {
            actGameSel = true;
            if ( !BW::FindDialogGlobal("GameSel")->findIndex(15)->activate() )
              actGameSel = false;
          }
          actCreate = false;
          break;
//create single/multi player game screen
        case 11: 
          actGameSel = false;
          //the first time we enter the create game screen, it won't set the map correctly
          //so we need to cancel out and re-enter
          tempDlg = BW::FindDialogGlobal("Create");
          if (*BW::BWDATA_menuStuff != -1) // Starcraft sets this to -1 after the first time we enter the create game screen
            this->pressKey( tempDlg->findIndex(13)->getHotkey() );
          else
          {
            GameType gt = GameTypes::getGameType(this->autoMenuGameType);
            if (gt != GameTypes::None && gt != GameTypes::Unknown)
              tempDlg->findIndex(17)->setSelectedByValue(gt.getID());

            this->pressKey( tempDlg->findIndex(12)->getHotkey() );
          }
          break;
// in lobby
          
        case 3:
          actCreate = false;
          Race playerRace = Races::getRace(this->autoMenuRace);
          if ( this->autoMenuRace == "RANDOMTP" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
          else if ( this->autoMenuRace == "RANDOMTZ" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
          else if ( this->autoMenuRace == "RANDOMPZ" )
            playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

          if ( playerRace != Races::Unknown && playerRace != Races::None )
            this->_changeRace(0, playerRace);

          break;
        }
      }
      else // wait for other computer to make game
      {
        switch ( menu )
        {
//lan games lobby
        case 10: 
          actRegistry = false;
          this->pressKey( BW::FindDialogGlobal("GameSel")->findIndex(13)->getHotkey() );
          //BW::FindDialogGlobal("GameSel")->findIndex(13)->activate();  // might bug
          break;
//multiplayer game ready screen
        case 3: 
          Race playerRace = Races::getRace(this->autoMenuRace);
          if ( this->autoMenuRace == "RANDOMTP" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
          else if ( this->autoMenuRace == "RANDOMTZ" )
            playerRace = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
          else if ( this->autoMenuRace == "RANDOMPZ" )
            playerRace = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;

          if ( playerRace != Races::Unknown && playerRace != Races::None )
            this->_changeRace(1, playerRace);

          break;          
        }
      }
    }
    else if (autoMenuMode == "BATTLE_NET")
    {
      switch ( menu )
      {
//main menu
      case 0: 
        if ( !actMainMenu )
        {
          actMainMenu = true;
          if ( !BW::FindDialogGlobal("MainMenu")->findIndex(4)->activate() )
            actMainMenu = false;
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();
        actConnSel = false;
        break;
//multiplayer select connection screen
      case 2: 
        actMainMenu = false;
        /*if ( !actConnSel )      // Broken
        {
          actConnSel = true;
          if ( !BW::FindDialogGlobal("ConnSel")->findIndex(9)->activate() )
            actConnSel = false;
        }*/
        this->pressKey( BW::FindDialogGlobal("ConnSel")->findIndex(9)->getHotkey() );
        break;
      }
    }


// Common
    if ( autoMenuMode != "" && autoMenuMode != "OFF" )
    {
      switch ( menu )
      {
//registry screen
      case 5: 
        actMainMenu = false;
        tempDlg = BW::FindDialogGlobal("gluPEdit");
        if ( tempDlg )
        {
          tempDlg->findIndex(4)->setText("BWAPI");
          tempDlg->findIndex(1)->activate();
        }
        else if ( !actRegistry )
        {
          actRegistry = true;
          if ( !BW::FindDialogGlobal("Login")->findIndex(4)->activate() )
            actRegistry = false;
        }
        actRaceSel = false;
        break;
// Score screen
      case 14: 
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
        actCreate = false;
        if ( !actEnd )
        {
          actEnd = true;
          if ( menu != 15 )//Only save replay if this is NOT the end of replay screen
          {
            if ( autoMenuSaveReplay != "" )
            {
              char szReplayPath[MAX_PATH];
              SStrCopy(szReplayPath, szInstallPath, MAX_PATH);
              SStrNCat(szReplayPath, "maps\\replays\\LastReplay.rep", MAX_PATH);
              CopyFile(szReplayPath, autoMenuSaveReplay.c_str(), false);
            }
          }
          if (autoMenuRestartGame != "" && autoMenuRestartGame != "OFF")
          {
            if ( !BW::FindDialogGlobal("End")->findIndex(7)->activate() )
              actEnd = false;
          }
        }
        break;
// Mission Briefings
      case 7:
      case 8:
      case 9:
        if ( !actBriefing )
        {
          actBriefing = true;
          if ( !BW::FindDialogGlobal("TerranRR")->findIndex(13)->activate() &&
               !BW::FindDialogGlobal("ReadyZ")->findIndex(13)->activate() )
           actBriefing = false;
        }
        break;
      }
    }
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
      this->unitArray[i]->setSelected(false);

    selectedUnitSet.clear();
    for (int i = 0; i < *BW::BWDATA_ClientSelectionCount && i < 12; ++i)
    {
      if ( BW::BWDATA_ClientSelectionGroup[i] )
      {
        BWAPI::UnitImpl *u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA_ClientSelectionGroup[i]);
        u->setSelected(true);
        selectedUnitSet.insert(u);
      }
    }
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::_isBattleNet()
  {
    return *BW::BWDATA_GameModule == 'TENB';
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::_isSinglePlayer() const
  {
    return *BW::BWDATA_GameModule == 0 || *BW::BWDATA_GameModule == -1;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::_isInGame() const
  {
    return *BW::BWDATA_InGame != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::_isReplay() const
  {
    return *BW::BWDATA_InReplay != 0;
  }

  //------------------------------------------------ MOUSE/KEY INPUT -----------------------------------------
  void GameImpl::pressKey(int key)
  {
    PostMessage(SDrawGetFrameWindow(), WM_KEYDOWN, (WPARAM)key, NULL);
    PostMessage(SDrawGetFrameWindow(), WM_KEYUP, (WPARAM)key, NULL);
  }
  void GameImpl::mouseDown(int x, int y)
  {
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONDOWN, NULL, (LPARAM)MAKELONG(x,y));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONUP, NULL, (LPARAM)MAKELONG(x,y));
  }

  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    QueueGameCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void  GameImpl::_changeRace(int slot, BWAPI::Race race)
  {
    BW::dialog *custom = BW::FindDialogGlobal("Create");
    if ( custom )
    {
      BW::dialog *slotCtrl = custom->findIndex((short)(28 + slot));  // 28 is the CtrlID of the first slot
      if ( slotCtrl )
        slotCtrl->setSelectedByValue(race.getID());
    }
    else
    {
      custom = BW::FindDialogGlobal("Chat");
      if ( custom )
      {
        BW::dialog *countdown = custom->findIndex(24);
        if ( countdown )
        {
          char *txt = countdown->getText();
          if ( txt && strlen(txt) > 0 )
          {
            if ( txt[0] >= '2' )
              QueueGameCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<u8>(race.getID()), (u8)slot), 3);
          }
          else
          {
            QueueGameCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<u8>(race.getID()), (u8)slot), 3);
          }
        } // countdown
      }
    }
  }
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command* command)
  {
    //executes latency compensation code and added it to the buffer
    command->execute(0);
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
  }
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    /** This function is called at the start of every match */
    actMainMenu = false;
    actRegistry = false;
    actCreate   = false;
    actConnSel  = false;
    actGameSel  = false;
    actRaceSel  = false;
    actEnd      = false;
    actBriefing = false;

    /* initialize the variables */
    frameCount      = 0;
    textSize        = 1;
    onStartCalled   = true;
    BWAPIPlayer     = NULL;
    bulletCount     = 0;
    calledMatchEnd  = false;

    srand(GetTickCount());

    /* set all the flags to the default of disabled */
    for (int i = 0; i < Flag::Max; i++)
      this->flags[i] = false;

    /* load the map data */
    map.load();
    this->savedMapHash = Map::getMapHash();

    BWAPIPlayer = NULL;
    enemyPlayer = NULL;

    if (*(BW::BWDATA_InReplay)) /* set replay flags */
    {
      for (int i = 0; i < Flag::Max; i++)
        this->flags[i] = true;
    }
    else
    {
      /* Get the current player */
      if ( this->players[*BW::BWDATA_g_LocalHumanID] )
      {
        this->BWAPIPlayer = this->players[*BW::BWDATA_g_LocalHumanID];
      }
      else
      {
        this->commandLog->log("Error: Could not locate BWAPI player.");
        return;
      }

      /* find the opponent player */
      for (int i = 0; i < 8; i++)
        if ((this->players[i]->getType() == BW::PlayerType::Computer ||
             this->players[i]->getType() == BW::PlayerType::Player ||
             this->players[i]->getType() == BW::PlayerType::EitherPreferComputer) &&
            this->BWAPIPlayer->isEnemy(this->players[i]))
          this->enemyPlayer = this->players[i];
    }

    /* Clear our sets */
    this->startLocations.clear();
    this->playerSet.clear();
    this->forces.clear();

    /* get the set of start locations */
    BW::Position *StartLocs = BW::BWDATA_startPositions;
    BW::UnitType SLocType   = BW::UnitType(BW::UnitID::Start_Location);
    for ( int i = 0; i < PLAYABLE_PLAYER_COUNT ; ++i)
    {
      if ( (StartLocs[i].x != 0 || StartLocs[i].y != 0)/* && this->players[i]->isParticipating()*/ )
        startLocations.insert(BWAPI::TilePosition( (StartLocs[i].x - SLocType.dimensionLeft()) / TILE_SIZE,
                                                   (StartLocs[i].y - SLocType.dimensionUp()  ) / TILE_SIZE) );
    }

    // Get Player Objects
    this->server.clearAll();
    for (int i = 0; i < PLAYABLE_PLAYER_COUNT; ++i)
    {
      if ( this->players[i] && 
           BW::BWDATA_Players[i].nType != BW::PlayerType::None &&
           BW::BWDATA_Players[i].nType <  BW::PlayerType::Closed )
      {
        players[i]->setID(server.getPlayerID(players[i]));
        this->playerSet.insert(this->players[i]);
      }
    }

    if ( this->players[11] )
      this->playerSet.insert(this->players[11]);

    // Get Force Objects, assign Force<->Player relations
    for ( int f = 0; f < 5; ++f )
    {
      ForceImpl *newForce;
      if ( f == 0 )
      {
        newForce = new ForceImpl(std::string(""));
        newForce->players.insert(this->players[11]);
        this->players[11]->force = newForce;
      }
      else
      {
        newForce = new ForceImpl( std::string(BW::BWDATA_ForceNames[f-1].name) );
      }
      
      this->forces.insert( (Force*)newForce );
      for ( int p = 0; p < PLAYABLE_PLAYER_COUNT; ++p )
      {
        if ( this->players[p] && BW::BWDATA_Players[p].nTeam == f )
        {
          this->players[p]->force = newForce;
          if ( BW::BWDATA_Players[p].nType != BW::PlayerType::None &&
               BW::BWDATA_Players[p].nType <  BW::PlayerType::Closed )
            newForce->players.insert(this->players[p]);
        }
      }
    }

    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Translates a storm ID to a player Index */
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
      if ( BW::BWDATA_Players[i].dwStormId == dwStormId )
        return i;
    }
    return -1;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const char* text)
  {
    /* analyze the string */
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");

    /* fix for bad referencing */
    while (parsed.size() < 5)
      parsed.push_back("");

    /* commands list */
    if (parsed[0] == "/leave")
    {
      this->leaveGame();
    }
    else if (parsed[0] == "/speed")
    {
      if (parsed[1] != "")
        setLocalSpeed(atoi(parsed[1].c_str()));
      else
        setLocalSpeed();
    }
    else if (parsed[0] == "/cheats")
    {
      sendText("power overwhelming");
      sendText("operation cwal");
      sendText("the gathering");
      sendText("medieval man");
      sendText("black sheep wall");
      sendText("food for thought");
      sendText("modify the phase variance");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
    }
    else if (parsed[0] == "/restart")
    {
      restartGame();
    }
    else if (parsed[0] == "/nogui")
    {
      setGUI(noGUI);
    }
#ifdef _DEBUG
    else if (parsed[0] == "/jump")
    {
      if ( !this->isMultiplayer() )
      {
        *BW::BWDATA_gwGameMode    = 3;
        *BW::BWDATA_Ophelia       = 1;
        *BW::BWDATA_GameState     = 0;
        *BW::BWDATA_gwNextGameMode  = 4;
      }
    }
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d", getLatency());
      printf("New latency?: %u frames; %ums", getLatencyFrames(), getLatencyTime());
    }
    else if (parsed[0] == "/dlgdebug")
    {
      if ( !myDlg )
      {
        // Create the dialog window
        myDlg = BW::CreateDialogWindow("Test Dialog", 100, 100, 300, 200);

        // Add additional controls to the window
        BW::dialog *test = new BW::dialog(BW::ctrls::cLIST, 1, "testing123", 12, 16, myDlg->width() - 24, myDlg->height() - 56);
        myDlg->addControl(test);
        test->setFlags(CTRL_PLAIN | CTRL_FONT_SMALLEST | CTRL_BTN_NO_SOUND);
        myDlg->setFlags(CTRL_UPDATE | CTRL_DLG_ACTIVE);

        // Initialize the dialog
        myDlg->initialize();
    
        // Add entries to the combo/list box
        test->addListEntry("Test");
        test->addListEntry("Test2");
        test->addListEntry("Test3");
        test->addListEntry("Test4");
        test->addListEntry("Test5");
        test->addListEntry("Test6");
        test->addListEntry("Test7");
        test->addListEntry("Test8");
        test->addListEntry("Test9");
        test->addListEntry("Test10");
        test->addListEntry("Test11");
        test->addListEntry("Test12");
        test->addListEntry("Test13");
        test->addListEntry("Test14");
        test->setSelectedIndex(test->getListCount()-1);
      }
    }
    else if (parsed[0] == "/pathdebug")
    {
      pathDebug = !pathDebug;
      printf("pathdebug %s", pathDebug ? "ENABLED" : "DISABLED");
    }
    else if (parsed[0] == "/unitdebug")
    {
      unitDebug = !unitDebug;
      printf("unitdebug %s", unitDebug ? "ENABLED" : "DISABLED");
    }
    else if (parsed[0] == "/grid")
    {
      grid = !grid;
      printf("Matrix grid %s.", grid ? "enabled" : "disabled");
    }
    else if (parsed[0] == "/hud")
    {
      hideHUD = !hideHUD;
      printf("Now %s the HUD.", hideHUD ? "hiding" : "showing");
    }
    else if (parsed[0] == "/test")
    {
      printf("Done");
      SetResolution(1024, 768);
    }
#endif
    else if (parsed[0] == "/wmode")
    {
      ToggleWMode(640, 480);
    }
    else
    {
      return false;
    }
    return true;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    //this is called at the end of every match
    if ( !this->onStartCalled )
      return;

#ifdef _DEBUG
    if ( myDlg )
    {
      delete myDlg;
      myDlg = NULL;
    }
#endif

    if ( !this->calledMatchEnd )
    {
      events.push_back(Event::MatchFrame());
      events.push_back(Event::MatchEnd(false));
      Util::Logger::globalLog->log("creating MatchEnd(leave) event");
      processEvents();
      server.update();
      events.clear();
      this->inGame = false;
      events.push_back(Event::MenuFrame());
      processEvents();
      server.update();
      events.clear();
    }

    if ( this->client )
    {
      delete this->client;
      this->client = NULL;
    }
    //clear all sets
    cmdToSiege.clear();
    cmdToUnsiege.clear();
    cmdToStop.clear();
    cmdToHold.clear();
    cmdToBurrow.clear();
    cmdToUnburrow.clear();
    cmdToCloak.clear();
    cmdToUncloak.clear();

    aliveUnits.clear();
    dyingUnits.clear();
    discoverUnits.clear();
    accessibleUnits.clear();
    evadeUnits.clear();
    lastEvadedUnits.clear();
    selectedUnitSet.clear();
    emptySet.clear();
    startLocations.clear();
    unitTypes.clear();
    forces.clear();
    playerSet.clear();
    minerals.clear();
    geysers.clear();
    neutralUnits.clear();
    bullets.clear();
    pylons.clear();
    staticMinerals.clear();
    staticGeysers.clear();
    staticNeutralUnits.clear();

    memset(savedUnitSelection, 0, sizeof(savedUnitSelection));
    wantSelectionUpdate = false;

    //clear latency buffer
    this->commandBuffer.clear();

    //remove AI Module from memory (object was already deleted)
    if ( hMod )
    {
      FreeLibrary(hMod);
      hMod = NULL;
    }
    Util::Logger::globalLog->logCritical("Unloaded AI Module");

    this->invalidIndices.clear();
    this->startedClient = false;

    //delete all dead units
    foreach (UnitImpl* d, this->deadUnits)
      delete d;
    this->deadUnits.clear();

    //delete all shapes
    for (unsigned int i = 0; i < this->shapes.size(); ++i)
      delete this->shapes[i];
    this->shapes.clear();

    for(int i = 0 ; i < PLAYER_COUNT; ++i)
      if ( this->players[i] )
        this->players[i]->onGameEnd();

    //reset game speeds and text size
    this->setLocalSpeed();
    this->setTextSize();

    //reset all Unit objects in the unit array
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
    {
      if ( !unitArray[i] )
        continue;
      unitArray[i]->userSelected      = false;
      unitArray[i]->isAlive           = false;
      unitArray[i]->wasAlive          = false;
      unitArray[i]->wasAccessible     = false;
      unitArray[i]->wasVisible        = false;
      unitArray[i]->staticInformation = false;
      unitArray[i]->nukeDetected      = false;
      unitArray[i]->lastType          = UnitTypes::Unknown;
      unitArray[i]->lastPlayer        = NULL;
      unitArray[i]->setID(-1);
    }
    this->cheatFlags  = 0;
    this->bulletCount = 0;
    this->frameCount  = -1;
    this->endTick     = GetTickCount();

    //reload auto menu data (in case the AI set the location of the next map/replay)
    this->loadAutoMenuData();

    //clear everything in the server
    this->server.clearAll();

    // Reset menu activation variables
    actMainMenu = false;
    actRegistry = false;
    actCreate   = false;
    actConnSel  = false;
    actGameSel  = false;
    actRaceSel  = false;
    actEnd      = false;
    actBriefing = false;

    setGUI();
    onStartCalled = false;
  }
  //------------------------------------------------ GET UNIT FROM INDEX -------------------------------------
  UnitImpl* GameImpl::getUnitFromIndex(int index)
  {
    int i = (index & 0x7FF);
    return this->unitArray[i];
  }
  //----------------------------------------------- GET BULLET FROM INDEX ------------------------------------
  BulletImpl* GameImpl::getBulletFromIndex(int index)
  {
    int i = (index & 0x7F);
    return this->bulletArray[i];
  }
  //------------------------------------------ copy Map To Shared Memory -------------------------------------
  void GameImpl::copyMapToSharedMemory()
  {
    map.copyToSharedMemory();
  }
  bool inline isAlive(UnitImpl* i, bool isHidden = false)
  {
    //this function determines if a unit in one of the alive unit lists is actually "alive" according to BWAPI
    //this function is only used in computeUnitExistence and shouldn't be called from any other function

    if ( i->getOriginalRawData->orderID == BW::OrderID::Die )
      return false;

    u16 uId = i->getOriginalRawData->unitType.id;
    UnitType _getType = BWAPI::UnitType(uId);
    if ( uId == BW::UnitID::Resource_MineralPatch1 ||
         uId == BW::UnitID::Resource_MineralPatch2 ||
         uId == BW::UnitID::Resource_MineralPatch3)
    {
      _getType = UnitTypes::Resource_Mineral_Field;
    }

    if ( !i->getOriginalRawData->sprite )
      return false;

    if ( isHidden ) //usually means: is inside another unit?
    {
      if (_getType == UnitTypes::Unknown)
        return false;//skip subunits if they are in this list

      if (_getType == UnitTypes::Protoss_Scarab ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
        return false;
    }

    if ( !_getType.isInvincible() && i->getOriginalRawData->hitPoints <= 0 )
    {
      Broodwar->printf("Unit %s is dead, 0 HP", _getType.getName().c_str());
      return false;
    }

    return true;
  }
  //------------------------------------------ Compute Unit Existence ----------------------------------------
  void GameImpl::computeUnitExistence()
  {
    foreach(UnitImpl* u, aliveUnits) //all alive units are dying until proven alive
    {
      u->wasAlive = true;
      u->isAlive  = false;
    }
    lastEvadedUnits = evadeUnits;//save last evaded units for updating shared memory (Server.cpp)

    //set the wasAccessible and wasVisible fields
    foreach(UnitImpl* u, accessibleUnits)
      u->wasAccessible = true;
    foreach(UnitImpl* u, evadeUnits)
      u->wasAccessible = false;

    //fill dyingUnits set with all aliveUnits and then clear the aliveUnits set.
    dyingUnits = aliveUnits;
    aliveUnits.clear();
    //Now we will add alive units to the aliveUnits set and remove them from the dyingUnits set based on the Broodwar unit lists:

    //compute alive and dying units
    for(UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_VisibleUnit_First); u; u = u->getNext())
    {
      if (isAlive(u))
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    for(UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_HiddenUnit_First); u; u = u->getNext())
    {
      if (isAlive(u,true))
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    for(UnitImpl* u = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_ScannerSweep_First); u; u = u->getNext())
    {
      if (isAlive(u))
      {
        u->isAlive = true;
        aliveUnits.insert(u);
        dyingUnits.erase(u);
        u->updateInternalData();
      }
    }
    //set the exists field to false for all dying units (though we don't update/clear their data yet)
    foreach(UnitImpl* u, dyingUnits)
      u->self->exists = false;
  }
  //------------------------------------------ Compute Client Sets -------------------------------------------
  void GameImpl::computePrimaryUnitSets()
  {
    //this frame computes the set of accessible units and visible units.
    accessibleUnits.clear();

    //discoverUnits is the set of units that have entered the accessibleUnits set this frame
    discoverUnits.clear();

    //evadeUnits is the set of units that have left the accessibleUnits set this frame
    evadeUnits.clear();

    //computes sets, also generating UnitCreate, UnitDiscover, UnitShow, UnitDestroy, UnitEvade, and UnitHide callbacks
    foreach(UnitImpl* u, aliveUnits)
    {
      if (u->canAccess())
      {
        if ( !u->wasAlive )
          events.push_back(Event::UnitCreate(u));
        if ( !u->wasAccessible )
        {
          discoverUnits.push_back(u);
          events.push_back(Event::UnitDiscover(u));
        }
        if (u->isVisible())
        {
          if ( !u->wasVisible )
            events.push_back(Event::UnitShow(u));
          u->wasVisible = true;
        }
        if (!u->isVisible())
        {
          if ( u->wasVisible )
            events.push_back(Event::UnitHide(u));
          u->wasVisible = false;
        }
        accessibleUnits.insert(u);
      }
      else
      {
        if (u->wasAccessible)
        {
          if (u->wasVisible)
          {
            u->wasVisible = false;
            events.push_back(Event::UnitHide(u));
          }
          evadeUnits.push_back(u);
          events.push_back(Event::UnitEvade(u));
        }
      }
    }
    foreach(UnitImpl* u, dyingUnits)
    {
      if (u->wasAccessible)
      {
        if (u->wasVisible)
        {
          u->wasVisible = false;
          events.push_back(Event::UnitHide(u));
        }
        evadeUnits.push_back(u);
        events.push_back(Event::UnitEvade(u));
        events.push_back(Event::UnitDestroy(u));
      }
    }
  }
  void GameImpl::extractUnitData()
  {
    //this function extracts all current unit information from Broodwar memory for all the accessible units
    //and also generates the NukeDetect event when needed
    nukeDots.clear();
    foreach (UnitImpl *i, aliveUnits)
    {
      i->connectedUnits.clear();
      i->loadedUnits.clear();
      int airWeaponCooldown = i->getOriginalRawData->airWeaponCooldown;
      if ( i->getOriginalRawData->subUnit )
        airWeaponCooldown = i->getOriginalRawData->subUnit->airWeaponCooldown;
      int groundWeaponCooldown = i->getOriginalRawData->groundWeaponCooldown;
      if ( i->getOriginalRawData->subUnit )
        groundWeaponCooldown = i->getOriginalRawData->subUnit->groundWeaponCooldown;
      if ( i->getOriginalRawData->unitType == BW::UnitID::Protoss_Reaver || i->getOriginalRawData->unitType == BW::UnitID::Protoss_Hero_Warbringer )
        groundWeaponCooldown = i->getOriginalRawData->mainOrderTimer;

      i->startingAttack           = airWeaponCooldown > i->lastAirWeaponCooldown || groundWeaponCooldown > i->lastGroundWeaponCooldown;
      i->lastAirWeaponCooldown    = airWeaponCooldown;
      i->lastGroundWeaponCooldown = groundWeaponCooldown;
      if (i->canAccess())
      {
        if (i->getID() == -1)
          i->setID(server.getUnitID(i));
        i->updateData();
      }
      if ( i->getOriginalRawData->unitType == BW::UnitID::Terran_Ghost)
      {
        if (i->getOriginalRawData->orderID == BW::OrderID::NukePaint)
          i->nukePosition = Position(i->getOriginalRawData->orderTargetPos.x, i->getOriginalRawData->orderTargetPos.y);
        if (i->getOriginalRawData->orderID != BW::OrderID::NukeTrack)
          i->nukeDetected = false;
        else
        {
          Position target=i->nukePosition;
          if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x()/32,target.y()/32))
            nukeDots.insert(target);
          if ( !i->nukeDetected )
          {
            i->nukeDetected = true;
            if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x()/32,target.y()/32))
              events.push_back(Event::NukeDetect(target));
            else
              events.push_back(Event::NukeDetect(Positions::Unknown));
          }
        }
      }
    }
  }
  void GameImpl::augmentUnitData()
  {
    //this function modifies the extracted unit data for build unit, loaded units, larva, and interceptors
    foreach(UnitImpl* i, accessibleUnits)
    {
      UnitImpl* orderTargetUnit = UnitImpl::BWUnitToBWAPIUnit(i->getOriginalRawData->orderTargetUnit);
      if ( orderTargetUnit && orderTargetUnit->exists() && i->getOrder() == Orders::ConstructingBuilding )
      {
        UnitImpl* j             = orderTargetUnit;
        i->self->buildUnit      = server.getUnitID((Unit*)j);
        i->self->isConstructing = true;
        i->self->isIdle         = false;
        i->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID((Unit*)i);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      else if ( i->getAddon() && !i->getAddon()->isCompleted() )
      {
        UnitImpl* j             = (UnitImpl*)i->getAddon();
        i->self->buildUnit      = server.getUnitID((Unit*)j);
        i->self->isConstructing = true;
        i->self->isIdle         = false;
        i->self->buildType      = j->self->type;
        j->self->buildUnit      = server.getUnitID((Unit*)i);
        j->self->isConstructing = true;
        j->self->isIdle         = false;
        j->self->buildType      = j->self->type;
      }
      if ( i->getTransport() )
        ((UnitImpl*)i->getTransport())->loadedUnits.insert((Unit*)i);

      if ( i->getHatchery() )
      {
        UnitImpl* hatchery = (UnitImpl*)i->getHatchery();
        hatchery->connectedUnits.insert((Unit*)i);
        if (hatchery->connectedUnits.size() >= 3)
          hatchery->self->remainingTrainTime = 0;
      }
      if ( i->getCarrier() )
        ((UnitImpl*)i->getCarrier())->connectedUnits.insert((Unit*)i);

    }
  }
  void GameImpl::applyLatencyCompensation()
  {
    //apply latency compensation
    while ((int)(this->commandBuffer.size()) > this->getLatency()+15)
    {
      for (unsigned int i = 0; i < this->commandBuffer[0].size(); i++)
        delete this->commandBuffer[0][i];
      this->commandBuffer.erase(this->commandBuffer.begin());
    }
    this->commandBuffer.push_back(std::vector<Command *>());
    for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
      for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
        this->commandBuffer[i][j]->execute(this->commandBuffer.size()-1-i);
  }
  void GameImpl::computeSecondaryUnitSets()
  {
    // This function computes units on tile, player units, neutral units, minerals, geysers, pylons, and static unit sets
    // Also generates the UnitMorph and UnitRenegade callbacks
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();

    foreach(Player* p, playerSet)
    {
      for(int i=0;i<230;i++)
      {
        ((PlayerImpl*)p)->self->allUnitCount[i]       = 0;
        ((PlayerImpl*)p)->self->visibleUnitCount[i]   = 0;
        ((PlayerImpl*)p)->self->completedUnitCount[i] = 0;
      }
    }

    foreach(UnitImpl* u, discoverUnits)
    {
      ((PlayerImpl*)u->getPlayer())->units.insert(u);
      if (u->getPlayer()->isNeutral())
      {
        neutralUnits.insert(u);
        if (u->getType() == UnitTypes::Resource_Mineral_Field)
          minerals.insert(u);
        else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
          geysers.insert(u);
      }
      else
      {
        if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
          pylons.insert(u);
      }
    }
    foreach(UnitImpl* u, evadeUnits)
    {
      ((PlayerImpl*)u->getPlayer())->units.erase(u);
      if (u->getPlayer()->isNeutral())
      {
        neutralUnits.erase(u);
        if (u->getType() == UnitTypes::Resource_Mineral_Field)
          minerals.erase(u);
        else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
          geysers.erase(u);
      }
      else if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
      {        
        pylons.erase(u);
      }
    }
    foreach(UnitImpl* i, accessibleUnits)
    {
      if ( i->getType().isBuilding() && !i->isLifted() )
      {
        int tx = i->getTilePosition().x();
        int ty = i->getTilePosition().y();
        for(int x = tx; x < tx + i->getType().tileWidth(); ++x)
          for(int y = ty; y < ty + i->getType().tileHeight(); ++y)
            unitsOnTileData[x][y].insert(i);
      }
      else
      {
        int startX = (i->_getPosition.x() - i->_getType.dimensionLeft()) / TILE_SIZE;
        int endX   = (i->_getPosition.x() + i->_getType.dimensionRight() + TILE_SIZE - 1) / TILE_SIZE; // Division - round up
        int startY = (i->_getPosition.y() - i->_getType.dimensionUp())   / TILE_SIZE;
        int endY   = (i->_getPosition.y() + i->_getType.dimensionDown()  + TILE_SIZE - 1) / TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            unitsOnTileData[x][y].insert(i);
      }
      if (i->lastType != i->_getType && i->lastType != UnitTypes::Unknown && i->_getType != UnitTypes::Unknown)
      {
        events.push_back(Event::UnitMorph(i));
        if (i->lastType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.erase(i);
          geysers.erase(i);
        }
        if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
        {
          neutralUnits.insert(i);
          geysers.insert(i);
        }
      }
      if (i->lastPlayer != i->_getPlayer && i->lastPlayer && i->_getPlayer )
      {
        events.push_back(Event::UnitRenegade(i));
        ((PlayerImpl*)i->lastPlayer)->units.erase(i);
        ((PlayerImpl*)i->_getPlayer)->units.insert(i);
      }
      ((PlayerImpl*)i->_getPlayer)->self->allUnitCount[i->_getType.getID()]++;
      if (i->isVisible())
        ((PlayerImpl*)i->_getPlayer)->self->visibleUnitCount[i->_getType.getID()]++;
      if (i->isCompleted())
        ((PlayerImpl*)i->_getPlayer)->self->completedUnitCount[i->_getType.getID()]++;
      i->lastPlayer = i->_getPlayer;
      i->lastType   = i->_getType;
    }

    if (this->staticNeutralUnits.empty()) //if we haven't saved the set of static units, save them now
    {
      foreach (UnitImpl* i, accessibleUnits)
      {
        if (i->_getPlayer->isNeutral())
        {
          i->saveInitialInformation();
          this->staticNeutralUnits.insert(i);
          if (i->_getType == UnitTypes::Resource_Mineral_Field)
            this->staticMinerals.insert(i);
          else if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
          {
            this->staticGeysers.insert(i);
          }
        }
      }
    }
  }
  //---------------------------------------------- UPDATE UNITS ----------------------------------------------
  void GameImpl::updateUnits()
  {
    computeUnitExistence();
    computePrimaryUnitSets();
    extractUnitData();
    augmentUnitData();
    applyLatencyCompensation();
    computeSecondaryUnitSets();
  }
  void GameImpl::processEvents()
  {
    //This function translates events into AIModule callbacks
    if ( !client )
      return;
    if (server.isConnected())
      return;
    foreach(Event e, events)
    {
      EventType::Enum et = e.type;
      switch (et)
      {
      case EventType::MatchStart:
        client->onStart();
        break;
      case EventType::MatchEnd:
        client->onEnd(e.isWinner);
        break;
      case EventType::MatchFrame:
        client->onFrame();
        break;
      case EventType::MenuFrame:
        break;
      case EventType::SendText:
        client->onSendText(e.text);
        break;
      case EventType::ReceiveText:
        client->onReceiveText(e.player, e.text);
        break;
      case EventType::PlayerLeft:
        client->onPlayerLeft(e.player);
        break;
      case EventType::NukeDetect:
        client->onNukeDetect(e.position);
        break;
      case EventType::UnitDiscover:
        client->onUnitDiscover(e.unit);
        break;
      case EventType::UnitEvade:
        client->onUnitEvade(e.unit);
        break;
      case EventType::UnitCreate:
        client->onUnitCreate(e.unit);
        break;
      case EventType::UnitDestroy:
        client->onUnitDestroy(e.unit);
        break;
      case EventType::UnitMorph:
        client->onUnitMorph(e.unit);
        break;
      case EventType::UnitShow:
        client->onUnitShow(e.unit);
        break;
      case EventType::UnitHide:
        client->onUnitHide(e.unit);
        break;
      case EventType::UnitRenegade:
        client->onUnitRenegade(e.unit);
        break;
      case EventType::SaveGame:
        client->onSaveGame(e.text);
        break;
      default:
        break;
      }
    }
  }
  //--------------------------------------------- UPDATE BULLETS ---------------------------------------------
  void GameImpl::updateBullets()
  {
    //update bullet information
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
      this->bulletArray[i]->setExists(false);
    std::set<Bullet*> lastBullets = bullets;
    bullets.clear();
    for(BW::Bullet* curritem = *BW::BWDATA_BulletNodeTable_FirstElement; curritem; curritem = curritem->nextBullet)
    {
      BulletImpl* b = BulletImpl::BWBulletToBWAPIBullet(curritem);
      b->setExists(true);
      b->updateData();
      if (b->exists())
        this->bullets.insert(b);
      lastBullets.erase(b);
    }
    foreach(BulletImpl* b, lastBullets)
      b->updateData();
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
      this->bulletArray[i]->saveExists();
  }
  //----------------------------------------------------- DRAW -----------------------------------------------
  bool GameImpl::inScreen(int ctype, int x, int y)
  {
    int screen_x1 = x;
    int screen_y1 = y;
    switch ( ctype )
    {
    case 2: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      break;
    case 3: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      break;
    }
    if (screen_x1 < 0   || 
        screen_y1 < 0   ||
        screen_x1 > BW::BWDATA_GameScreenBuffer->wid || 
        screen_y1 > BW::BWDATA_GameScreenBuffer->ht)
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    switch ( ctype )
    {
    case 2: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
      break;
    case 3: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      break;
    }
    BW::rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > scrLimit.Xmax  && screen_x2 > scrLimit.Xmax) ||
        (screen_y1 > scrLimit.Ymax && screen_y2 > scrLimit.Ymax))
      return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    int screen_x3 = x3;
    int screen_y3 = y3;
    switch ( ctype )
    {
    case 2: // if we're using map coordinates, subtract the position of the screen to convert the coordinates into screen coordinates
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
      screen_x3 -= *(BW::BWDATA_ScreenX);
      screen_y3 -= *(BW::BWDATA_ScreenY);
      break;
    case 3: // if we're using mouse coordinates, add the position of the mouse to convert the coordinates into screen coordinates
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      screen_x3 += BW::BWDATA_Mouse->x;
      screen_y3 += BW::BWDATA_Mouse->y;
      break;
    }
    BW::rect scrLimit = { 0, 0, BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht };
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > scrLimit.Xmax && screen_x2 > scrLimit.Xmax && screen_x3 > scrLimit.Xmax) ||
        (screen_y1 > scrLimit.Ymax && screen_y2 > scrLimit.Ymax && screen_y3 > scrLimit.Ymax))
      return false;
    return true;
  }
//--------------------------------------------------- ON SAVE ------------------------------------------------
  void GameImpl::onSaveGame(char *name)
  {
    /* called when the game is being saved */
    events.push_back(Event::SaveGame(std::string(name)));
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  void GameImpl::onSendText(const char* text)
  {
    if ( !text )
      return;

    if ( !parseText(text) && isFlagEnabled(BWAPI::Flag::UserInput) )
    {
      if ( externalModuleConnected )
        events.push_back(Event::SendText(std::string(text)));
      else
        sendText("%s", text);
    }
  }
  //---------------------------------------------- ON RECV TEXT ----------------------------------------------
  void GameImpl::onReceiveText(int playerId, std::string text)
  {
    /* Do onReceiveText */
    int realId = stormIdToPlayerId(playerId);
    if ( realId != -1 && 
         (!this->BWAPIPlayer ||
          realId != this->BWAPIPlayer->getIndex() ) &&
         this->isFlagEnabled(BWAPI::Flag::UserInput) )
      events.push_back(Event::ReceiveText(this->players[realId], text));
  }
  //------------------------------------------- CENTER ON SELECTED -------------------------------------------
  void GameImpl::moveToSelected()
  {
    int count = this->selectedUnitSet.size();
    int x = 0;
    int y = 0;
    foreach(BWAPI::UnitImpl *u, this->selectedUnitSet)
    {
      x += u->getPosition().x();
      y += u->getPosition().y();
    }
    x /= count;
    y /= count;
    x -= BW::BWDATA_GameScreenBuffer->wid / 2;
    y -= BW::BWDATA_GameScreenBuffer->ht  / 2 - 40;
    if ( x < 0 )
      x = 0;
    if ( y < 0 )
      y = 0;
    this->setScreenPosition(x, y);
  }

  //--------------------------------------------- EXECUTE COMMAND --------------------------------------------
  void GameImpl::executeCommand(UnitCommand command, bool addCommandToLatComBuffer)
  {
    if      (command.type == UnitCommandTypes::Attack_Move)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      Unit* target = command.target;
      switch ( command.unit->getType().getID() )
      {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CarrierAttack1), sizeof(BW::Orders::Attack));
        break;
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::ReaverAttack1), sizeof(BW::Orders::Attack));
        break;
      default:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Attack1), sizeof(BW::Orders::Attack));
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      TilePosition target(command.x,command.y);
      UnitType type1(command.extra);
      BW::UnitType type((u16)command.extra);
      if ( command.unit->getType() == BWAPI::UnitTypes::Zerg_Nydus_Canal && type == BW::UnitID::Zerg_NydusCanal )
        QueueGameCommand((PBYTE)&BW::Orders::MakeNydusExit(BW::TilePosition((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::MakeNydusExit));
      else if (!type.isAddon())
        QueueGameCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeBuilding));
      else
        QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeAddon));
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      TilePosition target(command.unit->getTilePosition().x() + 4, command.unit->getTilePosition().y() + 1);
      UnitType type1(command.extra);
      target.makeValid();
      QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), (u16)type1.getID()), sizeof(BW::Orders::MakeAddon));
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      UnitType type1(command.extra);
      BW::UnitType type((u16)type1.getID());
      switch ( command.unit->getType().getID() )
      {
      case BW::UnitID::Zerg_Larva:
      case BW::UnitID::Zerg_Mutalisk:
      case BW::UnitID::Zerg_Hydralisk:
        QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(type), 3);
        break;
      case BW::UnitID::Zerg_Hatchery:
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Spire:
      case BW::UnitID::Zerg_CreepColony:
        QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph(type), 3);
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::TrainFighter(), 1);
        break;
      default:
        QueueGameCommand((PBYTE)&BW::Orders::TrainUnit(type), 3);
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      UnitType type(command.extra);
      if(type.isBuilding())
        QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph((u16)type.getID()), sizeof(BW::Orders::BuildingMorph));
      else
        QueueGameCommand((PBYTE)&BW::Orders::UnitMorph((u16)type.getID()), sizeof(BW::Orders::UnitMorph));
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      TechType tech(command.extra);
      QueueGameCommand((PBYTE)&BW::Orders::Invent(BW::TechType((u8)tech.getID())), sizeof(BW::Orders::Invent));
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      UpgradeType upgrade(command.extra);
      QueueGameCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType((u8)upgrade.getID())), sizeof(BW::Orders::Upgrade));
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      Unit* target = command.target;
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Move), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      QueueGameCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      switch ( command.unit->getType().getID() )
      {
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::ReaverStop(), sizeof(BW::Orders::ReaverStop));
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        QueueGameCommand((PBYTE)&BW::Orders::CarrierStop(), sizeof(BW::Orders::CarrierStop));
        break;
      default:
        QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Harvest1), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      QueueGameCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)command.x, (u16)command.y), BW::UnitType((u16)command.unit->getType().getID())), sizeof(BW::Orders::Land));
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      BWAPI::UnitType thisType = command.unit->getType();
      if ( thisType == UnitTypes::Terran_Bunker )
      {
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::PickupBunker), sizeof(BW::Orders::Attack));
      }
      else if ( thisType == UnitTypes::Terran_Dropship || 
                thisType == UnitTypes::Protoss_Shuttle || 
                thisType == UnitTypes::Zerg_Overlord   ||
                thisType == UnitTypes::Hero_Yggdrasill )
      {
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::PickupTransport), sizeof(BW::Orders::Attack));
      }
      else if ( command.target->getType() == UnitTypes::Terran_Bunker   ||
                command.target->getType() == UnitTypes::Terran_Dropship ||
                command.target->getType() == UnitTypes::Protoss_Shuttle ||
                command.target->getType() == UnitTypes::Zerg_Overlord   ||
                command.target->getType() == UnitTypes::Hero_Yggdrasill )
      {
        QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)command.target), sizeof(BW::Orders::RightClick));
      }
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      QueueGameCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)command.target), sizeof(BW::Orders::UnloadUnit));
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if ( command.unit->getType()==UnitTypes::Terran_Bunker)
        QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
      else
        QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)command.unit->getPosition().x(), (u16)command.unit->getPosition().y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      if ( command.unit->getType()==UnitTypes::Terran_Bunker)
        QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
      else
        QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)command.x, (u16)command.y), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      QueueGameCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)command.x, (u16)command.y)), sizeof(BW::Orders::RightClick));
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)command.target), sizeof(BW::Orders::RightClick));
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((s8)command.extra), sizeof(BW::Orders::CancelTrain));
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((s8)command.extra), sizeof(BW::Orders::CancelTrain));
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (command.unit->getType().isBuilding())
        QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
      else
        QueueGameCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      TechType tech(command.extra);
      switch (tech.getID())
      {
        case BW::TechID::Stimpacks:
          QueueGameCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
          break;
        case BW::TechID::TankSiegeMode:
          if (command.unit->isSieged())
          {
            QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
          }
          else
          {
            QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
          }
          break;
        case BW::TechID::PersonnelCloaking:
        case BW::TechID::CloakingField:
          if(command.unit->isCloaked())
          {
            QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
          }
          else
          {
            QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
          }
          break;
        case BW::TechID::Burrowing:
          if(command.unit->isBurrowed())
          {
            QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
          }
          else
          {
            QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
          }
          break;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      TechType tech(command.extra);
      Position position(command.x,command.y);
      u8 order = BW::OrderID::None;
      switch (tech.getID())
      {
        case BW::TechID::DarkSwarm:
          order = BW::OrderID::DarkSwarm;
          break;
        case BW::TechID::DisruptionWeb:
          order = BW::OrderID::CastDisruptionWeb;
          break;
        case BW::TechID::EMPShockwave:
          order = BW::OrderID::EmpShockwave;
          break;
        case BW::TechID::Ensnare:
          order = BW::OrderID::Ensnare;
          break;
        case BW::TechID::NuclearStrike:
          order = BW::OrderID::NukePaint;
          break;
        case BW::TechID::Plague:
          order = BW::OrderID::Plague;
          break;
        case BW::TechID::PsionicStorm:
          order = BW::OrderID::PsiStorm;
          break;
        case BW::TechID::Recall:
          order = BW::OrderID::Teleport;
          break;
        case BW::TechID::ScannerSweep:
          order = BW::OrderID::PlaceScanner;
          break;
        case BW::TechID::SpiderMines:
          order = BW::OrderID::PlaceMine;
          break;
        case BW::TechID::StasisField:
          order = BW::OrderID::StasisField;
          break;
      }
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), order), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      TechType tech(command.extra);
      if (tech==TechTypes::Archon_Warp)
        QueueGameCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
      else if (tech==TechTypes::Dark_Archon_Meld)
        QueueGameCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
      else
      {
        u8 order;
        switch (tech.getID())
        {
          case BW::TechID::Consume:
            order = BW::OrderID::Consume;
            break;
          case BW::TechID::DefensiveMatrix:
            order = BW::OrderID::DefensiveMatrix;
            break;
          case BW::TechID::Feedback:
            order = BW::OrderID::CastFeedback;
            break;
          case BW::TechID::Hallucination:
            order = BW::OrderID::Hallucination1;
            break;
          case BW::TechID::Healing:
            order = BW::OrderID::MedicHeal1;
            break;
          case BW::TechID::Infestation:
            order = BW::OrderID::InfestMine2;
            break;
          case BW::TechID::Irradiate:
            order = BW::OrderID::Irradiate;
            break;
          case BW::TechID::Lockdown:
            order = BW::OrderID::MagnaPulse;
            break;
          case BW::TechID::Maelstorm:
            order = BW::OrderID::CastMaelstrom;
            break;
          case BW::TechID::MindControl:
            order = BW::OrderID::CastMindControl;
            break;
          case BW::TechID::OpticalFlare:
            order = BW::OrderID::CastOpticalFlare;
            break;
          case BW::TechID::Parasite:
            order = BW::OrderID::CastParasite;
            break;
          case BW::TechID::Restoration:
            order = BW::OrderID::Restoration;
            break;
          case BW::TechID::SpawnBroodlings:
            order = BW::OrderID::SummonBroodlings;
            break;
          case BW::TechID::YamatoGun:
            order = BW::OrderID::FireYamatoGun1;
            break;
          default:
            order = BW::OrderID::None;
        }
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, order), sizeof(BW::Orders::Attack));
      }
    }
    if (addCommandToLatComBuffer)
      BroodwarImpl.addToCommandBuffer(new Command(command));
  }
}