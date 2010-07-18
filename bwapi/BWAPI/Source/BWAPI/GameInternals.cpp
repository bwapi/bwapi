#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

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

#include "BWAPI/AIModule.h"
#include "DLLMain.h"

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"
#include "BWtoBWAPI.h"

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
      , inUpdate(false)
      , inGame(false)
      , calledOnEnd(false)
      , frameCount(0)
      , BUFFER_SIZE(1024)
  {
    BWAPI::Broodwar = static_cast<Game*>(this);

    /* initialize the unit types */
    BW::UnitType::initialize();
    BWtoBWAPI_init();

    try
    {
      /* create log handles */
      if (logging)
      {
        this->commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
        this->newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);
      }
      else
      {
        this->commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::DontLog);
        this->newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::DontLog);
      }
      loadAutoMenuData();


      /* iterate through players and create PlayerImpl for each */
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        players[i] = new PlayerImpl((u8)i);

      /* iterate through units and create UnitImpl for each */
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[i],
                                    (u16)i);

      /* iterate through bullets and create BulletImpl for each */
      for (int i = 0; i < BW::BULLET_ARRAY_MAX_LENGTH; i++)
        bulletArray[i] = new BulletImpl(&BW::BWDATA_BulletNodeTable->bullet[i],
                                        (u16)i);


      /* iterate through unit types and create UnitType for each */
      for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        unitTypes.insert(BW::UnitType((u16)i));
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error.txt", "a+");
      fprintf(f, "Exception caught inside Game constructor: %s\n", exception.getMessage().c_str());
      fclose(f);
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {

    /* destroy all UnitImpl */
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    /* destroy all PlayerImpl */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
      delete players[i];

    /* destroy all log handles */
    delete this->commandLog;
    delete this->newUnitLog;
  }

  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void GameImpl::IssueCommand(PBYTE pbBuffer, u32 iSize)
  {
    __asm
    {
      mov ecx, pbBuffer
      mov edx, iSize
    }
    NewIssueCommand();
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void GameImpl::update()
  {
    this->inGame = true;

    if ( myDlg )
      myDlg->update();
    
    if ( canvas )
    {
      u8 *data = canvas->getSourceBuffer()->data;
      if ( data )
        memset(data, 0, 640*480);

      for( int i = 0; i < (int)shapes.size(); i++ )
        shapes[i]->draw();

      canvas->update();
    }

    BW::dialog *endDialog = BW::FindDialogGlobal("LMission");
    if ( !endDialog )
      endDialog = BW::FindDialogGlobal("WMission");
    if ( endDialog )
      endDialog->findIndex(-2)->activate();

    accumulatedFrames++;
    DWORD currentTickCount = GetTickCount();
    if ( currentTickCount >= lastTickCount + 1000 )
    {
      fps               = accumulatedFrames;
      averageFPS        = averageFPS*0.7+fps*0.3;
      lastTickCount     = currentTickCount;
      accumulatedFrames = 0;
    }

    actMainMenu = false;
    actRegistry = false;
    actCreate   = false;
    actConnSel  = false;
    actGameSel  = false;
    actRaceSel  = false;
    actEnd      = false;
    actBriefing = false;

    try
    {
      this->inUpdate = true;
      if (!this->isOnStartCalled())
        this->onGameStart();
      
      if (!this->enabled)
        return;
      if (this->client != NULL && this->calledOnEnd == false)
      {
        if (this->BWAPIPlayer != NULL)
        {
          if (this->BWAPIPlayer->isVictorious())
          {
            this->client->onEnd(true);
            events.push_back(Event::MatchEnd(true));
            this->calledOnEnd = true;
          }
          if (this->BWAPIPlayer->isDefeated())
          {
            this->client->onEnd(false);
            events.push_back(Event::MatchEnd(false));
            this->calledOnEnd = true;
          }
        }
        else
        {
          bool allDone = true;
          foreach(Player* p, this->playerSet)
          {
            if (((PlayerImpl*)p)->getIndex() >= 8) continue;
            if (!p->isDefeated() && !p->isVictorious() && !p->leftGame())
              allDone=false;
          }
          if (allDone)
          {
            this->client->onEnd(false);
            events.push_back(Event::MatchEnd(false));
            this->calledOnEnd = true;
          }
        }
      }

      refreshSelectionStates();

      for (int i = 0; i < BW::PLAYER_COUNT; i++)
      {
        bool prevLeftGame=this->players[i]->leftGame();
        this->players[i]->updateData();
        if (!prevLeftGame && this->players[i]->leftGame())
        {
          if (this->client!=NULL)
          {
            this->client->onPlayerLeft((Player*)this->players[i]);
            events.push_back(Event::PlayerLeft((Player*)this->players[i]));
          }
        }
      }

      std::list<UnitImpl*> unitList;
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      {
        this->getUnitFromIndex(i)->connectedUnits.clear();
        this->getUnitFromIndex(i)->loadedUnits.clear();
      }
      std::set<BWAPI::UnitImpl*> dyingUnits = units;
      for(UnitImpl* i = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_VisibleUnit_First); i!=NULL ; i = i->getNext())
      {
        if (i->getOriginalRawData->orderID == BW::OrderID::Die) continue;
        unitList.push_back(i);
        dyingUnits.erase(i);
      }
      for(UnitImpl* i = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_HiddenUnit_First); i!=NULL ; i = i->getNext())
      {
        if (i->getOriginalRawData->orderID == BW::OrderID::Die) continue;
        UnitType _getType = BWAPI::UnitType(i->getOriginalRawData->unitID.id);
        bool _isCompleted = i->getOriginalRawData->status.getBit(BW::StatusFlags::Completed);

        if (_getType==UnitTypes::Unknown) continue;//skip subunits if they are in this list
        if (!_isCompleted) continue;
        if (_getType==UnitTypes::Protoss_Scarab ||
            _getType==UnitTypes::Terran_Vulture_Spider_Mine) continue;
        unitList.push_back(i);
        dyingUnits.erase(i);
      }
      for(UnitImpl* i = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_ScannerSweep_First); i!=NULL ; i = i->getNext())
      {
        if (i->getOriginalRawData->orderID == BW::OrderID::Die) continue;
        unitList.push_back(i);
        dyingUnits.erase(i);
      }
      for each(UnitImpl* u in dyingUnits)
      {
        //Broodwar->printf("unit %x died",u);
        onUnitDestroy(u);
        units.erase(u);
      }

      this->unitsToBeAdded.clear();
      std::list<Position> detectedNukes;
      foreach (UnitImpl* i, unitList)
      {
        i->_exists = true;
        if (i->getID()==-1)
          i->setID(server.getUnitID(i));
        i->connectedUnits.clear();
        i->loadedUnits.clear();
      }
      foreach (UnitImpl* i, unitList)
      {
        i->updateData();
        if (this->units.find(i) == this->units.end())
        {
          this->units.insert(i);
          this->unitsToBeAdded.insert(i);
        }
      }
      foreach (UnitImpl* i, units)
      {
        if (i->exists()==false) continue;
        UnitImpl* orderTargetUnit = UnitImpl::BWUnitToBWAPIUnit(i->getOriginalRawData->orderTargetUnit);
        if (orderTargetUnit != NULL && orderTargetUnit->exists() && i->getOrder() == Orders::ConstructingBuilding)
        {
          UnitImpl* j = orderTargetUnit;
          i->self->buildUnit = server.getUnitID((Unit*)j);
          i->self->isConstructing = true;
          i->self->isIdle = false;
          i->self->buildType = j->self->type;
          j->self->buildUnit = server.getUnitID((Unit*)i);
          j->self->isConstructing = true;
          j->self->isIdle = false;
          j->self->buildType = j->self->type;
        }
        else if (i->getAddon()!=NULL && i->getAddon()->isCompleted()==false)
        {
          UnitImpl* j = (UnitImpl*)i->getAddon();
          i->self->buildUnit = server.getUnitID((Unit*)j);
          i->self->isConstructing = true;
          i->self->isIdle = false;
          i->self->buildType = j->self->type;
          j->self->buildUnit = server.getUnitID((Unit*)i);
          j->self->isConstructing = true;
          j->self->isIdle = false;
          j->self->buildType = j->self->type;
        }
        if (i->getTransport()!=NULL)
          ((UnitImpl*)i->getTransport())->loadedUnits.insert((Unit*)i);

        if (i->getHatchery() != NULL)
        {
          UnitImpl* hatchery = (UnitImpl*)i->getHatchery();
          hatchery->connectedUnits.insert((Unit*)i);
          if (hatchery->connectedUnits.size()>=3)
            hatchery->self->remainingTrainTime = 0;
        }

        if (i->getCarrier() != NULL)
          ((UnitImpl*)i->getCarrier())->connectedUnits.insert((Unit*)i);

        if (i->getType() == UnitTypes::Terran_Nuclear_Missile &&
            i->nukeDetected == false &&
            i->getOriginalRawData->connectedUnit->unitID == BW::UnitID::Terran_Ghost)
        {
          i->nukeDetected = true;
          BW::Position bwtarget = i->getOriginalRawData->orderTargetPos;
          Position target(bwtarget.x, bwtarget.y);
          if (this->client)
          {
            if (this->isFlagEnabled(Flag::CompleteMapInformation) || this->isVisible(target.x() / 32, target.y() / 32))
              detectedNukes.push_back(target);
            else
              detectedNukes.push_back(Positions::Unknown);
          }
        }
      }

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

      this->updateUnits();
      this->updateBullets();
      foreach(Position i, detectedNukes)
      {
        this->client->onNukeDetect(i);
        events.push_back(Event::NukeDetect(i));
      }

      for (unsigned int i = 0; i < this->shapes.size(); i++)
        delete this->shapes[i];
      this->shapes.clear();
      this->inUpdate = false;

      foreach(std::string i, sentMessages)
      {
        bool send = !BroodwarImpl.onSendText(i.c_str());
        if (send)
          BroodwarImpl.sendText(i.c_str());
      }
      this->sentMessages.clear();

    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "a+");
      fprintf(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
      fclose(f);
    }

    if (this->startedClient == false)
    {
      sendText("BWAPI revision %s is now live.", SVN_REV_STR);
      if (server.isConnected())
      {
        Util::Logger::globalLog->logCritical("Client connected, not loading AI module.");
        this->client = new AIModule();
        printf("BWAPI: Connected to AI Client process");
      }
      else
      {
        TCHAR szDllPath[MAX_PATH];
        GetPrivateProfileStringA("ai", "ai_dll", "NULL", szDllPath, MAX_PATH, "bwapi-data\\bwapi.ini");
        if (_strcmpi(szDllPath, "NULL") == 0)
        {
          printf("\x06 Could not find ai_dll under ai in \"bwapi-data\\bwapi.ini\".");
          FILE* f = fopen("bwapi-error.txt", "a+");
          fprintf(f, "Could not find ai_dll under ai in \"bwapi-data\\bwapi.ini\".\n");
          fclose(f);
        }

        Util::Logger::globalLog->logCritical("Loading AI DLL from: %s", szDllPath);
        hMod = LoadLibrary(szDllPath);
        if (hMod == NULL)
        {
          Util::Logger::globalLog->logCritical("ERROR: Failed to load the AI Module");
          this->client = new AIModule();
          Broodwar->enableFlag(Flag::CompleteMapInformation);
          Broodwar->enableFlag(Flag::UserInput);
          printf("Error: Failed to load the AI Module");
        }
        else
        {
          Util::Logger::globalLog->logCritical("Loaded AI Module");
          Util::Logger::globalLog->logCritical("Importing by Virtual Function Table from AI DLL");

          typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);
          Util::Logger::globalLog->logCritical("Creating an Object of AIModule");

          PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, TEXT("newAIModule"));
          this->client = newAIModule(this);
          Util::Logger::globalLog->logCritical("Created an Object of AIModule");
          printf("BWAPI: Loaded the AI Module: %s", szDllPath);
        }
      }
      events.push_back(Event::MatchStart());
      this->client->onStart();
      this->startedClient = true;
    }

    events.push_back(Event::MatchFrame());
    this->server.update();
    this->client->onFrame();

    this->loadSelected();
    if (!this->isPaused())
      this->frameCount++;
  }
  //------------------------------------------- LOAD AUTO MENU DATA ------------------------------------------
  void GameImpl::loadAutoMenuData()
  {
    char buffer[MAX_PATH];
    GetPrivateProfileStringA("config", "auto_menu", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");
    this->autoMenuMode = std::string(buffer);

    if (autoMenuMode!="OFF" && autoMenuMode!="off" && autoMenuMode!="")
    {
      GetPrivateProfileStringA("config", "map", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");

      //split path into path and filename
      char* mapPathAndNameI=buffer;
      char* mapPathAndNameLastSlash=buffer;
      while(mapPathAndNameI[0]!='\0')
      {
        if (mapPathAndNameI[0]=='\\' || mapPathAndNameI[0]=='/')
          mapPathAndNameLastSlash=mapPathAndNameI+1;
        mapPathAndNameI++;
      }
      autoMenuMapName=std::string(mapPathAndNameLastSlash);
      mapPathAndNameLastSlash[0]='\0';
      autoMenuMapPath=std::string(buffer);
    }
    GetPrivateProfileStringA("config", "race", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");
    autoMenuRace = std::string(buffer);
    GetPrivateProfileStringA("config", "enemy_race", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");
    autoMenuEnemyRace = std::string(buffer);
    GetPrivateProfileStringA("config", "enemy_count", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");
    autoMenuEnemyCount = std::string(buffer);
    GetPrivateProfileStringA("config", "game_type", "NULL", buffer, MAX_PATH, "bwapi-data\\bwapi.ini");
    autoMenuGameType = std::string(buffer);
  }
  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    this->inGame = false;
    events.push_back(Event::MenuFrame());
    this->server.update();
    int menu = *BW::BWDATA_glGluesMode;
    BW::dialog *tempDlg;
    if (autoMenuMode == "SINGLE_PLAYER")
    {
      switch ( menu )
      {
//main menu
      case 0: 
        if ( !actMainMenu )
        {
          actMainMenu = true;
          BW::FindDialogGlobal("MainMenu")->findIndex(3)->activate();
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();

        actRegistry = false;
        break;
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
          BW::FindDialogGlobal("Login")->findIndex(4)->activate();
        }
        actRaceSel = false;
        break;
//single player play custom / load replay selection screen
      case 22:
        actRegistry = false;
        strcpy(BW::BWDATA_menuMapRelativePath, autoMenuMapPath.c_str());
        strcpy(BW::BWDATA_menuMapFileName, autoMenuMapName.c_str());
        if ( !actRaceSel )
        {
          actRaceSel = true;
          BW::FindDialogGlobal("RaceSelection")->findIndex(10)->activate();
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
          this->pressKey('C');
          //tempDlg->findIndex(13)->doEvent(14, 2);    // This is too efficient and will cause whatever trick being used to fail (infinite loop)
        else
        {
          int enemyCount = atoi(this->autoMenuEnemyCount.c_str());
          if (enemyCount < 1) enemyCount = 1;
          if (enemyCount > 7) enemyCount = 7;
          Race r = Races::getRace(this->autoMenuRace);
          if (r != Races::Unknown && r != Races::None)
            this->_changeRace(0, r);
          Race er = Races::getRace(this->autoMenuEnemyRace);
          if (er != Races::Unknown && er != Races::None)
          {
            for(int i = 0; i < enemyCount; i++)
              this->_changeRace(i + 1, er);
          }
          //close remaining slots
          for(int i = enemyCount; i < 7; i++)
            tempDlg->findIndex((short)(21 + i))->setSelectedIndex(0);

          GameType gt = GameTypes::getGameType(this->autoMenuGameType);
          if (gt != GameTypes::None && gt != GameTypes::Unknown)
            tempDlg->findIndex(17)->setSelectedByValue(gt.getID());

          this->pressKey('O');
          /*if ( !actCreate )
          {
            actCreate = true;
            tempDlg->findIndex(12)->activate();
          }*/
        }
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
          BW::FindDialogGlobal("End")->findIndex(7)->activate();
        }
        break;
// Mission Briefings
      case 7:
      case 8:
      case 9:
        if ( !actBriefing )
        {
          actBriefing = true;
          BW::FindDialogGlobal("TerranRR")->findIndex(13)->activate();
          BW::FindDialogGlobal("ReadyZ")->findIndex(13)->activate();
        }
        break;
      }
    }
    else if (autoMenuMode == "LAN_UDP")
    {
      switch ( menu )
      {
//main menu
      case 0: 
        if ( !actMainMenu )
        {
          actMainMenu = true;
          BW::FindDialogGlobal("MainMenu")->findIndex(4)->activate();
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();

        actConnSel = false;
        break;
// Select connection
      case 2:
        actMainMenu = false;

        //tempDlg = BW::FindDialogGlobal("ConnSel");
        //tempDlg->findIndex(5)->setSelectedByString("Local Area Network (UDP)"); // This doesn't work yet
        this->pressKey(VK_DOWN);
        this->pressKey(VK_DOWN);
        this->pressKey(VK_DOWN);
        this->pressKey(VK_DOWN);
        this->pressKey(VK_DOWN); // move 5 because of the custom SNP, doesn't affect people without it
        this->pressKey('O');
        actRegistry = false;
        break;
//registry screen
      case 5: 
        actConnSel = false;
        tempDlg = BW::FindDialogGlobal("gluPEdit");
        if ( tempDlg )
        {
          tempDlg->findIndex(4)->setText("BWAPI");
          tempDlg->findIndex(1)->activate();
        }
        else if ( !actRegistry )
        {
          actRegistry = true;
          BW::FindDialogGlobal("Login")->findIndex(4)->activate();
        }
        actGameSel = false;
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
          BW::FindDialogGlobal("End")->findIndex(7)->activate();
        }
        break;
// Mission Briefings
      case 7:
      case 8:
      case 9:
        if ( !actBriefing )
        {
          actBriefing = true;
          BW::FindDialogGlobal("TerranRR")->findIndex(13)->activate();
          BW::FindDialogGlobal("ReadyZ")->findIndex(13)->activate();
        }
        break;
      }

      if (autoMenuMapName.length() > 0)
      {
        switch ( menu )
        {
//lan games lobby
        case 10: 
          actRegistry = false;
          strcpy(BW::BWDATA_menuMapRelativePath, autoMenuMapPath.c_str());
          strcpy(BW::BWDATA_menuMapFileName, autoMenuMapName.c_str());
          if ( !actGameSel )
          {
            actGameSel = true;
            BW::FindDialogGlobal("GameSel")->findIndex(15)->activate();
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
            this->pressKey('C');
          else
          {
            GameType gt = GameTypes::getGameType(this->autoMenuGameType);
            if (gt != GameTypes::None && gt != GameTypes::Unknown)
              tempDlg->findIndex(17)->setSelectedByValue(gt.getID());

            this->pressKey('O');
            /*if ( !actCreate )
            {
              actCreate = true;
              tempDlg->findIndex(12)->activate();
            }*/
          }
          break;
// in lobby
        case 3:
          actCreate = false;
          Race r = Races::getRace(this->autoMenuRace);
          if (r != Races::Unknown)
            this->_changeRace(0, r);
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
          this->pressKey('O');
          //BW::FindDialogGlobal("GameSel")->findIndex(13)->activate();  // might bug
          break;
//multiplayer game ready screen
        case 3: 
          Race r = Races::getRace(this->autoMenuRace);
          if (r != Races::Unknown)
            this->_changeRace(1, r);
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
          BW::FindDialogGlobal("MainMenu")->findIndex(4)->activate();
        }
        tempDlg = BW::FindDialogGlobal("Delete");
        if ( tempDlg )
          tempDlg->findIndex(7)->activate();
        actConnSel = false;
        break;
//multiplayer select connection screen
      case 2: 
        actMainMenu = false;
        if ( !actConnSel )
        {
          actConnSel = true;
          BW::FindDialogGlobal("ConnSel")->findIndex(9)->activate();
        }
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
          BW::FindDialogGlobal("End")->findIndex(7)->activate();
        }
        break;
// Mission Briefings
      case 7:
      case 8:
      case 9:
        if ( !actBriefing )
        {
          actBriefing = true;
          BW::FindDialogGlobal("TerranRR")->findIndex(13)->activate();
          BW::FindDialogGlobal("ReadyZ")->findIndex(13)->activate();
        }
        break;
      }
    }
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->unitArray[i]->setSelected(false);

    this->saveSelected();
    for (int i = 0; savedSelectionStates[i] != NULL; i++)
      BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i])->setSelected(true);
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::_isBattleNet()
  {
    return (*BW::BWDATA_IsBattleNet != 0);
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::_isSinglePlayer() const
  {
    return (*BW::BWDATA_IsMultiplayer == 0);
  }
  //------------------------------------------- IS ON START CALLED -------------------------------------------
  bool GameImpl::isOnStartCalled() const
  {
    return this->onStartCalled;
  }
  //------------------------------------------ SET ON START CALLED -------------------------------------------
  void GameImpl::setOnStartCalled(bool onStartCalled)
  {
    this->onStartCalled = onStartCalled;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::_isInGame() const
  {
    return *(BW::BWDATA_InGame) != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::_isReplay() const
  {
    return *(BW::BWDATA_InReplay) != 0;
  }
  //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
  void GameImpl::printEx(int pID, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (_isInGame() || _isReplay())
    {
      __asm
      {
        pushad
        push 0       // Unknown
        mov eax, pID   // Player ID (-1 for notification area)
        push txtout  // Text
        call dword ptr [BW::BWFXN_PrintText]
        popad
      }
    }
    else
      sendText(txtout); // until lobby print private text is found
  }
  // 0 = to all, 1 = to allies, 2 = to specified
  void  GameImpl::sendTextEx(u8 txfilter, u8 plfilter, const char *text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    char* txtout = buffer;

    if (_isReplay())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (_isInGame() && _isSinglePlayer())
    {
      BW::CheatFlags::Enum cheatID = BW::getCheatFlag(buffer);
      if (cheatID != BW::CheatFlags::None)
      {
        this->cheatFlags ^= cheatID;
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UseCheat(this->cheatFlags), sizeof(BW::Orders::UseCheat));
        if (cheatID == BW::CheatFlags::ShowMeTheMoney ||
            cheatID == BW::CheatFlags::BreatheDeep ||
            cheatID == BW::CheatFlags::WhatsMineIsMine)
          this->cheatFlags ^= cheatID;
      }
      else
      {
        printEx(this->BWAPIPlayer->getIndex(), "%s", buffer);
      }
      return;
    }

    if (_isInGame())
    {
      switch (txfilter)
      {
      case 0:
        memset(BW::BWDATA_SendTextFilter, 0xFF, 2);
        break;
      case 1:

        break;
      default:

        break;
      }
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }
    }
    else
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
  }

  void GameImpl::pressKey(int key)
  {
    INPUT *keyp          = new INPUT;
    keyp->type           = INPUT_KEYBOARD;
    keyp->ki.wVk         = (WORD)key;
    keyp->ki.dwFlags     = 0;
    keyp->ki.time        = 0;
    keyp->ki.wScan       = 0;
    keyp->ki.dwExtraInfo = 0;
    SendInput(1,keyp,sizeof(INPUT));
    keyp->ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1,keyp,sizeof(INPUT));
  }
  void GameImpl::mouseDown(int x, int y)
  {
    INPUT *i          = new INPUT;
    i->type           = INPUT_MOUSE;
    i->mi.dx          = x;
    i->mi.dy          = y;
    i->mi.dwFlags     = MOUSEEVENTF_LEFTDOWN;
    i->mi.dwExtraInfo = 0;
    SendInput(1,i,sizeof(INPUT));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    INPUT *i          = new INPUT;
    i->type           = INPUT_MOUSE;
    i->mi.dx          = x;
    i->mi.dy          = y;
    i->mi.dwFlags     = MOUSEEVENTF_LEFTUP;
    i->mi.dwExtraInfo = 0;
    SendInput(1,i,sizeof(INPUT));
  }

  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
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
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<u8>(race.getID()), (u8)slot), 3);
  }
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command* command)
  {
    command->execute(0);
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
  }
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    /* initialize the variables */
    this->frameCount  = 0;
    this->textSize    = 1;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    this->opponent    = NULL;
    this->calledOnEnd = false;
    this->bulletCount = 0;

    /* set all the flags to the default of disabled */
    for (int i = 0; i < FLAG_COUNT; i++)
      this->flags[i] = false;

    /* load the map data */
    map.load();
    this->savedMapHash = Map::getMapHash();

    if (*(BW::BWDATA_InReplay)) /* set replay flags */
    {
      for (int i = 0; i < FLAG_COUNT; i++)
        this->flags[i] = true;
    }
    else
    {
      /* find the current player by name */
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if (this->players[i] != NULL && strcmp(BW::BWDATA_CurrentPlayer, this->players[i]->getName().c_str()) == 0)
          this->BWAPIPlayer = this->players[i];

      /* error if player not found */
      if (this->BWAPIPlayer == NULL)
      {
        this->commandLog->log("Error: Could not locate BWAPI player.");
        return;
      }

      /* find the opponent player */
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if ((this->players[i]->getType() == BW::PlayerType::Computer ||
             this->players[i]->getType() == BW::PlayerType::Player ||
             this->players[i]->getType() == BW::PlayerType::EitherPreferComputer) &&
            this->opponent == NULL &&
            this->BWAPIPlayer->isEnemy(this->players[i]))
          this->opponent = this->players[i];

      /* error if opponent not found */
      if (this->opponent == NULL)
        this->commandLog->log("Warning: Could not find any opponent");
    }

    /* get the start locations */
    BW::Positions* posptr = BW::BWDATA_startPositions;
    this->startLocations.clear();
    this->playerSet.clear();
    this->forces.clear();
    while (posptr->x != 0 || posptr->y != 0)
    {
      startLocations.insert(BWAPI::TilePosition((int)((posptr->x - BW::TILE_SIZE*2)          / BW::TILE_SIZE),
                                                (int)((posptr->y - (int)(BW::TILE_SIZE*1.5)) / BW::TILE_SIZE)));
      posptr++;
    }

    /* get force names */
    std::set<std::string> force_names;
    std::map<std::string, ForceImpl*> force_name_to_forceimpl;
    this->server.clearAll();

    for (int i = 0; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        players[i]->setID(server.getPlayerID(players[i]));
        force_names.insert(std::string(this->players[i]->getForceName()));
        this->playerSet.insert(this->players[i]);
      }

    /* create ForceImpl for force names */
    foreach (std::string i, force_names)
    {
      ForceImpl* newforce = new ForceImpl(i);
      this->forces.insert((Force*)newforce);
      force_name_to_forceimpl.insert(std::make_pair(i, newforce));
    }

    /* create ForceImpl for players */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
    {
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        ForceImpl* force = force_name_to_forceimpl.find(std::string(this->players[i]->getForceName()))->second;
        force->players.insert(this->players[i]);
        this->players[i]->force = force;
      }
    }
    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());

    canvas = BW::CreateCanvas("Canvas");
    canvas->initialize();
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Get Player ID */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
    {
      if ( BW::BWDATA_Players->player[i].dwStormId == (u32)dwStormId )
        return i;
    }
    return -1;
  }
  int GameImpl::playerIdToStormId(int dwPlayerId)
  {
    /* Get storm ID */
    return BW::BWDATA_Players->player[dwPlayerId].dwStormId;
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
      return true;
    }
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d",getLatency());
      return true;
    }
    else if (parsed[0] == "/speed")
    {
      if (parsed[1] != "")
        setLocalSpeed(atoi(parsed[1].c_str()));
      else
        setLocalSpeed();
      return true;
    }
    else if (parsed[0] == "/restart")
    {
      restartGame();
      return true;
    }
    else if (parsed[0] == "/dlg")
    {
      if ( !myDlg )
      {
        // Create the dialog window
        myDlg = BW::CreateDialogWindow("Test Dialog", 100, 100, 300, 200);

        // Add additional controls to the window
        BW::dialog *test = new BW::dialog(BW::ctrls::cLIST, 1, "testing123", 8, 30, 200, 100);
        myDlg->addControl(test);

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
      }
      return true;
    }
/*    else if ( parsed[0] == "/add" )
    {
      if ( myDlg )
        myDlg->findIndex(1)->addListEntry("TEST");
      return true;
    }
    else if ( parsed[0] == "/rem" )
    {
      if ( myDlg )
        myDlg->findIndex(1)->removeListEntry();
      return true;
    }
    else if ( parsed[0] == "/clear" )
    {
      if ( myDlg )
        myDlg->findIndex(1)->clearList();
      return true;
    }
    else if ( parsed[0] == "/sel" )
    {
      if ( myDlg )
        myDlg->findIndex(1)->setSelectedIndex(10);
      return true;
    }*/
    return false;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    this->setOnStartCalled(false);

    if ( myDlg )
    {
      delete myDlg;
      myDlg = NULL;
    }
    if ( canvas )
    {
      delete canvas;
      canvas = NULL;
    }

    if (this->client != NULL)
    {
      if (this->calledOnEnd == false)
      {
        bool win = true;
        if (this->_isReplay())
          win = false;
        else
        { 
          for(UnitImpl* i = UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeList_VisibleUnit_First); i!=NULL ; i = i->getNext())
          {
            if (self()->isEnemy(i->_getPlayer) && i->_getType.isBuilding())
              win = false;
          }
        }
        this->client->onEnd(win);
        events.push_back(Event::MatchEnd(win));
        this->calledOnEnd = true;
      }
      delete this->client;
      this->client = NULL;
    }
    this->units.clear();
    this->forces.clear();
    this->playerSet.clear();
    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->myPylons.clear();
    this->unitsToBeAdded.clear();

    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();

    this->commandBuffer.clear();
    FreeLibrary(hMod);
    Util::Logger::globalLog->logCritical("Unloaded AI Module");
    for (int i = 0; i < 13; i++) // Why is this 13? There can only be 12 units selected.
      this->savedSelectionStates[i] = NULL;

    this->invalidIndices.clear();
    this->selectedUnitSet.clear();
    this->startedClient = false;
    foreach (UnitImpl* d, this->deadUnits)
      delete d;

    this->deadUnits.clear();

    for (unsigned int i = 0; i < this->shapes.size(); i++)
      delete this->shapes[i];

    this->shapes.clear();

    for(int i = 0 ; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL)
        this->players[i]->onGameEnd();
    this->setLocalSpeed(-1);

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      if (unitArray[i] == NULL)
        continue;
      unitArray[i]->userSelected      = false;
      unitArray[i]->_exists           = false;
      unitArray[i]->dead              = false;
      unitArray[i]->savedPlayer       = NULL;
      unitArray[i]->savedUnitType     = NULL;
      unitArray[i]->staticInformation = false;
      unitArray[i]->lastVisible       = false;
      unitArray[i]->lastType          = UnitTypes::Unknown;
      unitArray[i]->lastPlayer        = NULL;
      unitArray[i]->nukeDetected      = false;
      unitArray[i]->setID(-1);
    }
    this->cheatFlags  = 0;
    this->bulletCount = 0;
    this->calledOnEnd = false;
    this->loadAutoMenuData();
    this->server.clearAll();
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
  //--------------------------------------------- SAVE SELECTED ----------------------------------------------
  void GameImpl::saveSelected()
  {
    memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4 * 12);
    savedSelectionStates[12] = NULL;
    selectedUnitSet.clear();
    for (int i = 0; savedSelectionStates[i] != NULL; i++)
      selectedUnitSet.insert(UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i]));
  }
  //--------------------------------------------- LOAD SELECTED ----------------------------------------------
  void GameImpl::loadSelected()
  {
    int unitCount = 0;
    while (savedSelectionStates[unitCount] != NULL)
      unitCount++;

    if (unitCount > 0)
      BW::selectUnits(unitCount, savedSelectionStates);
  }
  //------------------------------------------ copy Map To Shared Memory -------------------------------------
  void GameImpl::copyMapToSharedMemory()
  {
    map.copyToSharedMemory();
  }
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void GameImpl::onUnitDestroy(BWAPI::UnitImpl* unit)
  {
    /* Called when a unit dies(death animation), not when it is removed */
    if (!unit->_exists)
      return;
    this->units.erase(unit);
    deadUnits.push_back(unit);
    int index = unit->getIndex();
    unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],
                                    (u16)index);
    if (this->client != NULL)
    {
      bool isInUpdate = this->inUpdate;
      this->inUpdate = false;
      if (unit != NULL && unit->canAccessSpecial())
      {
        unit->makeVisible = true;
        if (unit->lastVisible)
        {
          this->client->onUnitHide(unit);
          events.push_back(Event::UnitHide(unit));
        }

        /* notify the client that the units in the transport died */
        std::set<Unit*> loadedSet = unit->getLoadedUnits();

        //units in terran bunker survive
        if (unit->getType().isFlyer())
        {
  		    foreach(Unit* loaded, loadedSet)
	  		    this->onUnitDestroy((UnitImpl*)loaded);
        }
        this->client->onUnitDestroy(unit);
        events.push_back(Event::UnitDestroy(unit));
        unit->makeVisible = false;
      }
      this->inUpdate = isInUpdate;
    }
    unit->die();
  }
  //---------------------------------------------- UPDATE UNITS ----------------------------------------------
  void GameImpl::updateUnits()
  {
    this->inUpdate = false;
    /* Clear all units on tile data */
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();

    /* Clear other stuff */
    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->myPylons.clear();
    std::list<BWAPI::UnitImpl*> morphUnits;
    std::list<BWAPI::UnitImpl*> showUnits;
    std::list<BWAPI::UnitImpl*> hideUnits;
    std::list<BWAPI::UnitImpl*> renegadeUnits;
    std::list< std::pair<BWAPI::UnitImpl*, bool> > dyingUnits;

    foreach (Player* i, playerSet)
      ((PlayerImpl*)i)->units.clear();

    foreach (UnitImpl* i, units)
    {
      if (i->canAccess())
      {
        if ((i->_getType.isInvincible()==false && i->getHitPoints()<=0) || i->getOrder().getID() == BW::OrderID::Die)
        {
          std::pair<UnitImpl*, bool> p(i,i->lastVisible);
          dyingUnits.push_back(p);
          continue;
        }
        int startX = (i->_getPosition.x() - i->_getType.dimensionLeft()) / BW::TILE_SIZE;
        int endX   = (i->_getPosition.x() + i->_getType.dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE; // Division - round up
        int startY = (i->_getPosition.y() - i->_getType.dimensionUp()) / BW::TILE_SIZE;
        int endY   = (i->_getPosition.y() + i->_getType.dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTileData[x][y].insert(i);

        ((PlayerImpl*)(i->_getPlayer))->units.insert(i);

        this->allUnits.insert(i);

        if (i->_getPlayer->isNeutral())
        {
          this->neutralUnits.insert(i);
          if (i->_getType == UnitTypes::Resource_Mineral_Field)
            this->minerals.insert(i);
          else
          {
            if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
              this->geysers.insert(i);
          }
        }
        else
        {
          if (i->_getPlayer == (Player*)this->BWAPIPlayer && i->_getType == UnitTypes::Protoss_Pylon && i->_isCompleted)
            this->myPylons.push_back(i);
        }
        if (i->lastType != i->_getType && i->lastType != UnitTypes::Unknown && i->_getType != UnitTypes::Unknown)
          morphUnits.push_back(i);

        if (i->lastPlayer != i->_getPlayer && i->lastPlayer != NULL && i->_getPlayer != NULL)
          renegadeUnits.push_back(i);
      }

      i->startingAttack           = i->getAirWeaponCooldown() > i->lastAirWeaponCooldown || i->getGroundWeaponCooldown() > i->lastGroundWeaponCooldown;
      i->lastAirWeaponCooldown    = i->getAirWeaponCooldown();
      i->lastGroundWeaponCooldown = i->getGroundWeaponCooldown();

      i->lastType   = i->_getType;
      i->lastPlayer = i->_getPlayer;
      if (!i->lastVisible && i->isVisible())
      {
        i->lastVisible = true;
        showUnits.push_back(i);
      }
      else
      {
        if (i->lastVisible && !i->isVisible())
        {
          i->lastVisible = false;
          hideUnits.push_back(i);
        }
      }
    }
    if (this->staticNeutralUnits.empty())
    {
      foreach (UnitImpl* i, units)
      {
        if (i->_getPlayer->isNeutral())
        {
          i->saveInitialInformation();
          this->staticNeutralUnits.insert(i);
          if (i->_getType == UnitTypes::Resource_Mineral_Field)
            this->staticMinerals.insert(i);
          else
          {
            if (i->_getType == UnitTypes::Resource_Vespene_Geyser)
              this->staticGeysers.insert(i);
          }
        }
      }
    }

    //here we have all dying units, not just the ones visible to the AI client
    for each(std::pair<BWAPI::UnitImpl*, bool> i in dyingUnits)
    {
      //set lastVisible if the unit was visible to the AI client just before death.
      //This will cause onUnitDestroy to also call onUnitHide before onUnitDestroy.
      if (i.second)
        i.first->lastVisible = true;
      this->onUnitDestroy(i.first); //GameImpl::onUnitDestroy will decide what unit destroy messages to pass along to the AI client
      i.first->lastVisible = false;
    }

    foreach (BWAPI::UnitImpl* i, unitsToBeAdded)
      if (this->client && i->_exists)
        if (i->canAccess())
        {
          this->client->onUnitCreate(i);
          events.push_back(Event::UnitCreate(i));
        }

    /* Pass all renegade units to the AI client */
    foreach (BWAPI::UnitImpl* i, renegadeUnits)
      if (this->client && i->_exists)
      {
        this->client->onUnitRenegade(i);
        events.push_back(Event::UnitRenegade(i));
      }

    /* Pass all morphing units to the AI client */
    foreach (BWAPI::UnitImpl* i, morphUnits)
      if (this->client && i->_exists)
      {
        this->client->onUnitMorph(i);
        events.push_back(Event::UnitMorph(i));
      }

    /* Pass all showing units to the AI client */
    foreach (BWAPI::UnitImpl* i, showUnits)
      if (this->client && i->_exists)
      {
        this->client->onUnitShow(i);
        events.push_back(Event::UnitShow(i));
      }

    /* Pass all hiding units to the AI client */
    foreach (BWAPI::UnitImpl* i, hideUnits)
      if (this->client && i->_exists)
      {
        i->makeVisible = true;
        this->client->onUnitHide(i);
        events.push_back(Event::UnitHide(i));
        i->makeVisible = false;
      }
    this->inUpdate = true;
  }
  //--------------------------------------------- UPDATE BULLETS ---------------------------------------------
  void GameImpl::updateBullets()
  {
    for(int i=0;i<BW::BULLET_ARRAY_MAX_LENGTH;i++)
    {
      this->bulletArray[i]->setExists(false);
    }
    this->bullets.clear();
    for(BW::Bullet* curritem = *BW::BWDATA_BulletNodeTable_FirstElement ; curritem; curritem = curritem->nextBullet)
    {
      BulletImpl* b = BulletImpl::BWBulletToBWAPIBullet(curritem);
      b->setExists(true);
      if (b->exists())
        this->bullets.insert(b);
    }
    for(int i=0;i<BW::BULLET_ARRAY_MAX_LENGTH;i++)
    {
      this->bulletArray[i]->saveExists();
    }
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  void GameImpl::setLastError(BWAPI::Error e)
  {
    /* implies that an error has occured */
    this->lastError = e;
  }
  //----------------------------------------------------- DRAW -----------------------------------------------
  void GameImpl::addShape(Shape* s)
  {
    /* Adds a shape to the draw queue */
    this->shapes.push_back(s);
  }
  bool GameImpl::inScreen(int ctype, int x, int y)
  {
    int screen_x1 = x;
    int screen_y1 = y;
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
    }
    if (screen_x1 < 0   || screen_y1 < 0 ||
        screen_x1 > 640 || screen_y1 > 480) return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
    }
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480)) return false;
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
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
      screen_x3 -= *(BW::BWDATA_ScreenX);
      screen_y3 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      screen_x3 += BW::BWDATA_Mouse->x;
      screen_y3 += BW::BWDATA_Mouse->y;
    }
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640 && screen_x3 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480 && screen_y3 > 480)) return false;
    return true;
  }
//--------------------------------------------------- ON SAVE ------------------------------------------------
  void GameImpl::onSaveGame(char *name)
  {
    /* called when the game is being saved */
    if (this->client != NULL)
    {
      this->client->onSaveGame(std::string(name));  // calls the client callback
      events.push_back(Event::SaveGame(std::string(name)));
    }
  }
//--------------------------------------------------- ISCRIPT ------------------------------------------------
  BWAPI::UnitImpl *GameImpl::spriteToUnit(BW::CSprite *sprite)
  {
    /* Retrieves a sprite's parent unit */
    BWAPI::UnitImpl* unit=NULL;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++) // iterate through every unit
      if (BW::BWDATA_UnitNodeTable->unit[i].sprite == sprite) // compare unit with sprite we're looking for
        unit=unitArray[i];

    return unit;
  }

  void GameImpl::iscriptParser(BW::CSprite *sprite, u8 anim)
  {
    BWAPI::UnitImpl *unit = spriteToUnit(sprite); // get sprite's parent unit
    if (unit != NULL)   // make sure the unit exists
    {
      unit->animState = anim; // associate the animation directly with the unit

      /* New onUnitDestroy hook. Triggered when the unit's animation changes.
      For example: unit plays death animation, we can check its order to see if it's really dying
      and call the old onUnitDestroy. */
      if (unit->getOrder().getID() == BW::OrderID::Die)
        this->onUnitDestroy(unit);
    }
  }

}