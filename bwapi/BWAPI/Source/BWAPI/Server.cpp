#include "Server.h"

#include <Util/Logger.h>
#include <Util/Foreach.h>

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"
#include <BWAPI/Client/GameData.h>

#include "DLLMain.h"
#include "../../svnrev.h"

namespace BWAPI
{
  #define PIPE_TIMEOUT 2000
  #define PIPE_SYSTEM_BUFFER_SIZE 4096
  Server::Server()
  {
    connected = false;
    int size = sizeof(GameData);
    mapFileHandle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_shared_memory");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make shared memory");
    data = (GameData*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, size);
    initializeSharedMemory();

    pipeObjectHandle = CreateNamedPipe("\\\\.\\pipe\\bwapi_pipe",
                                           PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                           PIPE_UNLIMITED_INSTANCES,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_TIMEOUT,
                                           NULL);
    if (pipeObjectHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make pipe");
    COMMTIMEOUTS c;
    c.ReadIntervalTimeout         = 100;
    c.ReadTotalTimeoutMultiplier  = 100;
    c.ReadTotalTimeoutConstant    = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant   = 2000;
    SetCommTimeouts(pipeObjectHandle,&c);

  }
  Server::~Server()
  {
    DisconnectNamedPipe(pipeObjectHandle);
  }
  void Server::update()
  {
    if (connected)
    {
      updateSharedMemory();
      callOnFrame();
      processCommands();
    }
    else
    {
      data->eventCount = 0;
      ((GameImpl*)Broodwar)->events.clear();
      if (!((GameImpl*)Broodwar)->startedClient)
        checkForConnections();
    }
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    data->stringCount      = 0;
  }
  bool Server::isConnected()
  {
    return connected;
  }
  int Server::addString(const char* text)
  {
    strncpy(data->strings[data->stringCount],text,256);
    return data->stringCount++;
  }
  int Server::addEvent(BWAPI::Event e)
  {
    assert(data->eventCount < GameData::MAX_EVENTS);
    BWAPIC::Event* e2 = &(data->events[data->eventCount]);
    data->eventCount++;
    int id   = data->eventCount;
    e2->type = e.type;
    e2->v1   = 0;
    e2->v2   = 0;
    switch (e.type)
    {
    case BWAPI::EventType::MatchEnd:
      e2->v1 = e.isWinner;
      break;
    case BWAPI::EventType::SendText:
    case BWAPI::EventType::SaveGame:
      e2->v1 = addString(e.text.c_str());
      break;
    case BWAPI::EventType::PlayerLeft:
      e2->v1 = getPlayerID(e.player);
      break;
    case BWAPI::EventType::ReceiveText:
      e2->v1  = getPlayerID(e.player);
      e2->v2  = addString(e.text.c_str());
      break;
    case BWAPI::EventType::NukeDetect:
      e2->v1 = e.position.x();
      e2->v2 = e.position.y();
      break;
    case BWAPI::EventType::UnitDiscover:
    case BWAPI::EventType::UnitEvade:
    case BWAPI::EventType::UnitCreate:
    case BWAPI::EventType::UnitDestroy:
    case BWAPI::EventType::UnitMorph:
    case BWAPI::EventType::UnitShow:
    case BWAPI::EventType::UnitHide:
    case BWAPI::EventType::UnitRenegade:
      e2->v1 = getUnitID(e.unit);
      break;
    default:
      break;
    }
    return id;
  }

  void Server::setWaitForResponse(bool wait)
  {
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    if (wait)
      dwMode |= PIPE_WAIT;
    else
      dwMode |= PIPE_NOWAIT;
    SetNamedPipeHandleState(pipeObjectHandle,&dwMode,NULL,NULL);
  }
  void Server::checkForConnections()
  {
    if (connected)
      return;
    BOOL success = ConnectNamedPipe(pipeObjectHandle, NULL);
    if (!success && GetLastError() != ERROR_PIPE_CONNECTED)
      return;
    if (GetLastError() == ERROR_PIPE_CONNECTED)
      connected = true;
    if (!connected)
      return;
    setWaitForResponse(true);
  }
  void Server::initializeSharedMemory()
  {
    //called once when Starcraft starts. Not at the start of every match.
    data->instanceID       = gdwProcNum;
    data->revision         = SVN_REV;
    data->isDebug          = (BUILD_DEBUG == 1);
    data->eventCount       = 0;
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    data->stringCount      = 0;
    data->mapFileName[0]   = 0;
    data->mapPathName[0]   = 0;
    data->mapName[0]       = 0;
    data->mapHash[0]       = 0;
    clearAll();
  }
  void Server::onMatchStart()
  {
    data->self          = getPlayerID(Broodwar->self());
    data->enemy         = getPlayerID(Broodwar->enemy());
    data->neutral       = getPlayerID(Broodwar->neutral());
    data->isMultiplayer = Broodwar->isMultiplayer();
    data->isBattleNet   = Broodwar->isBattleNet();
    data->isReplay      = Broodwar->isReplay();

    //load static map data
    for(int x = 0; x < Broodwar->mapWidth()*4; ++x)
      for(int y = 0; y < Broodwar->mapHeight()*4; ++y)
      {
        data->isWalkable[x][y] = Broodwar->isWalkable(x, y);
      }

    for(int x = 0; x < Broodwar->mapWidth(); ++x)
      for(int y = 0; y < Broodwar->mapHeight(); ++y)
      {
        data->isBuildable[x][y]     = Broodwar->isBuildable(x, y);
        data->getGroundHeight[x][y] = Broodwar->getGroundHeight(x, y);
        if ( BW::BWDATA_SAIPathing )
          data->mapTileRegionId[x][y] = BW::BWDATA_SAIPathing->mapTileRegionId[y][x];
        else
          data->mapTileRegionId[x][y] = 0;
      }
    
    memset(data->mapSplitTilesMiniTileMask, 0, sizeof(data->mapSplitTilesMiniTileMask));
    memset(data->mapSplitTilesRegion1,      0, sizeof(data->mapSplitTilesRegion1));
    memset(data->mapSplitTilesRegion2,      0, sizeof(data->mapSplitTilesRegion2));
    memset(data->regionGroupIndex,          0, sizeof(data->regionGroupIndex));
    if ( BW::BWDATA_SAIPathing )
    {
      for(int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesMiniTileMask[i] = BW::BWDATA_SAIPathing->splitTiles[i].minitileMask;
        data->mapSplitTilesRegion1[i]      = BW::BWDATA_SAIPathing->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i]      = BW::BWDATA_SAIPathing->splitTiles[i].rgn2;
        data->regionGroupIndex[i]          = BW::BWDATA_SAIPathing->regions[i].groupIndex;
      }
    }

    data->mapWidth  = Broodwar->mapWidth();
    data->mapHeight = Broodwar->mapHeight();
    strncpy(data->mapFileName, Broodwar->mapFileName().c_str(), MAX_PATH);
    strncpy(data->mapPathName, Broodwar->mapPathName().c_str(), MAX_PATH);
    strncpy(data->mapName, Broodwar->mapName().c_str(), 32);
    strncpy(data->mapHash, Broodwar->mapHash().c_str(), 40);
    data->mapFileName[MAX_PATH] = 0;
    data->mapPathName[MAX_PATH] = 0;
    data->mapName[32]           = 0;
    data->mapHash[40]           = 0;

    data->startLocationCount = Broodwar->getStartLocations().size();
    int i = 0;
    foreach(TilePosition t, Broodwar->getStartLocations())
    {
      data->startLocations[i].x = t.x();
      data->startLocations[i].y = t.y();
      i++;
    }
    //static force data
    foreach(Force *i, Broodwar->getForces())
    {
      int id = getForceID(i);
      strncpy(data->forces[id].name, i->getName().c_str(), 32);
    }
    //static player data
    foreach(Player *i, Broodwar->getPlayers())
    {
      int id = getPlayerID(i);
      PlayerData* p = &(data->players[id]);
      PlayerData* p2 = ((PlayerImpl*)i)->self;

      strncpy(p->name, i->getName().c_str(), 32);
      p->race  = i->getRace().getID();
      p->type  = i->getType().getID();
      p->force = getForceID(i->getForce());
      p->color = p2->color;
      p->colorByte = p2->colorByte;

      for(int j = 0; j < 12; ++j)
      {
        p->isAlly[j]  = false;
        p->isEnemy[j] = false;
      }
      foreach(Player *j, Broodwar->getPlayers())
      {
        p->isAlly[getPlayerID(j)]  = i->isAlly(j);
        p->isEnemy[getPlayerID(j)] = i->isEnemy(j);
      }
      p->isNeutral      = i->isNeutral();
      p->startLocationX = i->getStartLocation().x();
      p->startLocationY = i->getStartLocation().y();
    }

    data->forceCount       = forceVector.size();
    data->playerCount      = playerVector.size();
    data->initialUnitCount = unitVector.size();

    data->botAPM_noselects = 0;
    data->botAPM_selects   = 0;
  }
  void Server::clearAll()
  {
    //clear force info
    data->forceCount = 0;
    forceVector.clear();
    forceLookup.clear();

    //clear player info
    data->playerCount = 0;
    playerVector.clear();
    playerLookup.clear();

    //clear unit info
    data->initialUnitCount = 0;
    unitVector.clear();
    unitLookup.clear();
  }

  void Server::updateSharedMemory()
  {
    data->eventCount   = 0;
    bool matchStarting = false;
    
    foreach(Event e, BroodwarImpl.events)
    {
      addEvent(e);
      if (e.type == EventType::MatchStart)
        matchStarting = true;
    }
    foreach(UnitImpl* u, BroodwarImpl.lastEvadedUnits)
      data->units[u->getID()] = u->data;

    ((GameImpl*)Broodwar)->events.clear();

    data->frameCount             = Broodwar->getFrameCount();
    data->fps                    = Broodwar->getFPS();
    data->botAPM_noselects       = Broodwar->getAPM();
    data->botAPM_selects         = Broodwar->getAPM(true);
    data->latencyFrames          = Broodwar->getLatencyFrames();
    data->latencyTime            = Broodwar->getLatencyTime();
    data->remainingLatencyFrames = Broodwar->getRemainingLatencyFrames();
    data->remainingLatencyTime   = Broodwar->getRemainingLatencyTime();
    data->replayFrameCount       = Broodwar->getReplayFrameCount();
    data->averageFPS             = Broodwar->getAverageFPS();
    data->mouseX                 = Broodwar->getMousePosition().x();
    data->mouseY                 = Broodwar->getMousePosition().y();
    data->isInGame               = Broodwar->isInGame();
    data->hasLatCom              = Broodwar->isLatComEnabled();
    if (Broodwar->isInGame())
    {
      data->gameType          = Broodwar->getGameType().getID();
      data->latency           = Broodwar->getLatency();
      for(int i = 0; i < 3; ++i)
        data->mouseState[i]   = Broodwar->getMouseState(i);
      for(int i = 0; i < 256; ++i)
        data->keyState[i]     = Broodwar->getKeyState(i);
      data->screenX           = Broodwar->getScreenPosition().x();
      data->screenY           = Broodwar->getScreenPosition().y();
      data->flags[0]          = Broodwar->isFlagEnabled(0);
      data->flags[1]          = Broodwar->isFlagEnabled(1);
      data->isPaused          = Broodwar->isPaused();
      data->selectedUnitCount = Broodwar->getSelectedUnits().size();
      int i = 0;
      foreach(Unit *t, Broodwar->getSelectedUnits())
      {
        data->selectedUnits[i] = getUnitID(t);
        i++;
      }

      //dynamic map data
      BroodwarImpl.copyMapToSharedMemory();
      //(no dynamic force data)

      //dynamic player data
      foreach(Player *i, Broodwar->getPlayers())
      {
        int id         = getPlayerID(i);
        PlayerData* p  = &(data->players[id]);
        PlayerData* p2 = ((PlayerImpl*)i)->self;

        p->isVictorious       = i->isVictorious();
        p->isDefeated         = i->isDefeated();
        p->leftGame           = i->leftGame();
        p->minerals           = p2->minerals;
        p->gas                = p2->gas;
        p->cumulativeMinerals = p2->cumulativeMinerals;
        p->cumulativeGas      = p2->cumulativeGas;
        for(int j = 0; j < 3; ++j)
        {
          p->supplyTotal[j]   = p2->supplyTotal[j];
          p->supplyUsed[j]    = p2->supplyUsed[j];
        }
        for(int j = 0; j < BWAPI_UNIT_TYPE_MAX_COUNT; ++j)
        {
          p->allUnitCount[j]        = p2->allUnitCount[j];
          p->visibleUnitCount[j]    = p2->visibleUnitCount[j];
          p->completedUnitCount[j]  = p2->completedUnitCount[j];
          p->deadUnitCount[j]       = p2->deadUnitCount[j];
          p->killedUnitCount[j]     = p2->killedUnitCount[j];
        }
        p->totalUnitScore      = p2->totalUnitScore;
        p->totalKillScore      = p2->totalKillScore;
        p->totalBuildingScore  = p2->totalBuildingScore;
        p->totalRazingScore    = p2->totalRazingScore;
        p->customScore         = p2->customScore;
        for(int j = 0; j < 63; ++j)
        {
          p->upgradeLevel[j] = p2->upgradeLevel[j];
          p->isUpgrading[j]  = p2->isUpgrading[j];
        }
        for(int j = 0; j < 47; ++j)
        {
          p->hasResearched[j] = p2->hasResearched[j];
          p->isResearching[j] = p2->isResearching[j];
        }
      }

      //dynamic unit data
      foreach(Unit *i, Broodwar->getAllUnits())
        data->units[i->getID()] = ((UnitImpl*)i)->data;

      for(int i = 0; i < 1700; ++i)
      {
        Unit* u = Broodwar->indexToUnit(i);
        int id = -1;
        if ( u )
          id = u->getID();
        data->unitArray[i] = id;
      }

      //dynamic bullet data
      for(int id = 0; id < 100; ++id)
        data->bullets[id] = BroodwarImpl.getBulletFromIndex(id)->data;
      
      //dynamic nuke dot data
      int j = 0;
      data->nukeDotCount = Broodwar->getNukeDots().size();
      foreach(Position nd, Broodwar->getNukeDots())
      {
        data->nukeDots[j].x = nd.x();
        data->nukeDots[j].y = nd.y();
        ++j;
      }

    }
    if (matchStarting)
      Server::onMatchStart();
  }

  int Server::getForceID(Force* force)
  {
    if ( !force )
      return -1;
    if (forceLookup.find(force) == forceLookup.end())
    {
      forceLookup[force] = (int)(forceVector.size());
      forceVector.push_back(force);
    }
    return forceLookup[force];
  }
  Force* Server::getForce(int id)
  {
    if ((int)forceVector.size() <= id || id < 0)
      return NULL;
    return forceVector[id];
  }

  int Server::getPlayerID(Player* player)
  {
    if ( !player )
      return -1;
    if (playerLookup.find(player) == playerLookup.end())
    {
      playerLookup[player] = (int)(playerVector.size());
      playerVector.push_back(player);
    }
    return playerLookup[player];
  }
  Player* Server::getPlayer(int id)
  {
    if ((int)playerVector.size() <= id || id < 0)
      return NULL;
    return playerVector[id];
  }

  int Server::getUnitID(Unit* unit)
  {
    if ( !unit )
      return -1;
    if (unitLookup.find(unit) == unitLookup.end())
    {
      unitLookup[unit] = (int)(unitVector.size());
      unitVector.push_back(unit);
    }
    return unitLookup[unit];
  }
  Unit* Server::getUnit(int id)
  {
    if ((int)unitVector.size() <= id || id < 0)
      return NULL;
    return unitVector[id];
  }

  void Server::callOnFrame()
  { 
    DWORD writtenByteCount;
    int code = 2;
    WriteFile(pipeObjectHandle, &code, sizeof(int), &writtenByteCount, NULL);
    while (code != 1)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(int), &receivedByteCount,NULL);
      if (!success)
      {
        DisconnectNamedPipe(pipeObjectHandle);
        connected = false;
        setWaitForResponse(false);
        break;
      }
    }
  }
  void Server::processCommands()
  {
    for(int i = 0; i < data->commandCount; ++i)
    {
      BWAPIC::CommandType::Enum c = data->commands[i].type;
      int v1 = data->commands[i].value1;
      int v2 = data->commands[i].value2;
      switch (c)
      {
      case BWAPIC::CommandType::SetScreenPosition:
        if (Broodwar->isInGame())
          Broodwar->setScreenPosition(v1,v2);
        break;
      case BWAPIC::CommandType::PingMinimap:
        if (Broodwar->isInGame())
          Broodwar->pingMinimap(v1,v2);
        break;
      case BWAPIC::CommandType::EnableFlag:
        if (Broodwar->isInGame())
          Broodwar->enableFlag(v1);
        break;
      case BWAPIC::CommandType::Printf:
        if (Broodwar->isInGame())
          Broodwar->printf("%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::SendText:
        if (Broodwar->isInGame())
          Broodwar->sendText("%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::ChangeRace:
        Broodwar->changeRace(Race(v1));
        break;
      case BWAPIC::CommandType::StartGame:
        Broodwar->startGame();
        break;
      case BWAPIC::CommandType::PauseGame:
        if (Broodwar->isInGame())
          Broodwar->pauseGame();
        break;
      case BWAPIC::CommandType::ResumeGame:
        if (Broodwar->isInGame())
          Broodwar->resumeGame();
        break;
      case BWAPIC::CommandType::LeaveGame:
        if (Broodwar->isInGame())
          Broodwar->leaveGame();
        break;
      case BWAPIC::CommandType::RestartGame:
        if (Broodwar->isInGame())
          Broodwar->restartGame();
        break;
      case BWAPIC::CommandType::SetLocalSpeed:
        if (Broodwar->isInGame())
          Broodwar->setLocalSpeed(v1);
        break;
      case BWAPIC::CommandType::SetTextSize:
        if (Broodwar->isInGame())
          Broodwar->setTextSize(v1);
        break;
      case BWAPIC::CommandType::SetLatCom:
        Broodwar->setLatCom(v1 == 1);
        break;
      case BWAPIC::CommandType::SetGui:
        Broodwar->setGUI(v1 == 1);
        break;
      default:
        break;
      }
    }
    if (Broodwar->isInGame())
    {
      for(int i = 0; i < data->unitCommandCount; ++i)
      {
        if (data->unitCommands[i].unitIndex < 0 || data->unitCommands[i].unitIndex >= (int)unitVector.size())
          continue;
        Unit* unit   = unitVector[data->unitCommands[i].unitIndex];
        Unit* target = NULL;
        if (data->unitCommands[i].targetIndex >= 0 && data->unitCommands[i].targetIndex < (int)unitVector.size())
          target = unitVector[data->unitCommands[i].targetIndex];

        unit->issueCommand(UnitCommand(unit, data->unitCommands[i].type, target, data->unitCommands[i].x, data->unitCommands[i].y, data->unitCommands[i].extra));
      }
      for(int i = 0; i < data->shapeCount; ++i)
      {
        BWAPIC::ShapeType::Enum s = data->shapes[i].type;
        /* Note: Variables here so that the calls aren't excessively long */
        int  ctype         = data->shapes[i].ctype;
        int  x1            = data->shapes[i].x1;
        int  y1            = data->shapes[i].y1;
        bool isSolid       = data->shapes[i].isSolid;
        BWAPI::Color color = Color(data->shapes[i].color);

        switch (s)
        {
          case BWAPIC::ShapeType::Text:
            Broodwar->drawText(ctype, x1, y1, data->strings[data->shapes[i].extra1]);
            break;
          case BWAPIC::ShapeType::Box:
            Broodwar->drawBox(ctype, x1, y1, data->shapes[i].x2, data->shapes[i].y2, color, isSolid);
            break;
          case BWAPIC::ShapeType::Triangle:
            Broodwar->drawTriangle(ctype, x1, y1, data->shapes[i].x2, data->shapes[i].y2, data->shapes[i].extra1, data->shapes[i].extra2, color, isSolid);
            break;
          case BWAPIC::ShapeType::Circle:
            Broodwar->drawCircle(ctype, x1, y1, data->shapes[i].extra1, color, isSolid);
            break;
          case BWAPIC::ShapeType::Ellipse:
            Broodwar->drawEllipse(ctype, x1, y1, data->shapes[i].extra1, data->shapes[i].extra2, color, data->shapes[i].isSolid);
            break;
          case BWAPIC::ShapeType::Dot:
            Broodwar->drawDot(ctype, x1, y1, color);
            break;
          case BWAPIC::ShapeType::Line:
            Broodwar->drawLine(ctype, x1, y1, data->shapes[i].x2, data->shapes[i].y2, color);
            break;
          default:
            break;
        } // switch
      } // for
    } // if isInGame
  }

}