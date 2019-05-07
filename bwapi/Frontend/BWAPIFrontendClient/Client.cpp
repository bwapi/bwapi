#include "Client.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

#undef max

namespace BWAPI
{
  //Client BWAPIClient;
  Client::Client()
    : pipeObjectHandle(INVALID_HANDLE_VALUE)
    , mapFileHandle(INVALID_HANDLE_VALUE)
    , gameTableFileHandle(INVALID_HANDLE_VALUE)
  {}
  Client::~Client()
  {
    this->disconnect();
  }
  bool Client::isConnected() const
  {
    return this->connected;
  }
  bool Client::connect()
  {
    if ( protoClient.isConnected() )
    {
      std::cout << "Already connected." << std::endl;
      return true;
    }

    while (true)
    {
      std::cout << "Connecting..." << std::endl;
      protoClient.lookForServer(0, "x", false);
      if (protoClient.isConnected())
      {
        connected = true;
        return connected;
      }
    }

    int serverProcID    = -1;
    int gameTableIndex  = -1;

    this->gameTable = NULL;
    this->gameTableFileHandle = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, "Local\\bwapi_shared_memory_game_list" );
    if ( !this->gameTableFileHandle )
    {
      std::cerr << "Game table mapping not found." << std::endl;
      return false;
    }
    this->gameTable = static_cast<GameTable*>( MapViewOfFile(this->gameTableFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameTable)) );
    if ( !this->gameTable )
    {
      std::cerr << "Unable to map Game table." << std::endl;
      return false;
    }

    //Find row with most recent keep alive that isn't connected
    DWORD latest = 0;
    for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; i++)
    {
      std::cout << i << " | " << gameTable->gameInstances[i].serverProcessID << " | " << gameTable->gameInstances[i].isConnected << " | " << gameTable->gameInstances[i].lastKeepAliveTime << std::endl;
      if (gameTable->gameInstances[i].serverProcessID != 0 && !gameTable->gameInstances[i].isConnected)
      {
        if ( gameTableIndex == -1 || latest == 0 || gameTable->gameInstances[i].lastKeepAliveTime < latest )
        {
          latest = gameTable->gameInstances[i].lastKeepAliveTime;
          gameTableIndex = i;
        }
      }
    }

    if (gameTableIndex != -1)
      serverProcID = gameTable->gameInstances[gameTableIndex].serverProcessID;

    if (serverProcID == -1)
    {
      std::cerr << "No server proc ID" << std::endl;
      return false;
    }
    
    std::stringstream sharedMemoryName;
    sharedMemoryName << "Local\\bwapi_shared_memory_";
    sharedMemoryName << serverProcID;

    std::stringstream communicationPipe;
    communicationPipe << "\\\\.\\pipe\\bwapi_pipe_";
    communicationPipe << serverProcID;

    pipeObjectHandle = CreateFileA(communicationPipe.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if ( pipeObjectHandle == INVALID_HANDLE_VALUE )
    {
      std::cerr << "Unable to open communications pipe: " << communicationPipe.str() << std::endl;
      CloseHandle(gameTableFileHandle);
      return false;
    }

    COMMTIMEOUTS c;
    c.ReadIntervalTimeout         = 100;
    c.ReadTotalTimeoutMultiplier  = 100;
    c.ReadTotalTimeoutConstant    = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant   = 2000;
    SetCommTimeouts(pipeObjectHandle,&c);

    std::cout << "Connected" << std::endl;
    mapFileHandle = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, sharedMemoryName.str().c_str());
    if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
    {
      std::cerr << "Unable to open shared memory mapping: " << sharedMemoryName.str() << std::endl;
      CloseHandle(pipeObjectHandle);
      CloseHandle(gameTableFileHandle);
      return false;
    }
    data = static_cast<BWAPI4::GameData*>( MapViewOfFile(mapFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(BWAPI4::GameData)) );
    if ( data == nullptr )
    {
      std::cerr << "Unable to map game data." << std::endl;
      return false;
    }

    // Create new instance of Game/Broodwar
    /*
    if ( BWAPI::BroodwarPtr )
      delete static_cast<GameImpl*>(BWAPI::BroodwarPtr);
    BWAPI::BroodwarPtr = new GameImpl(data);
    assert( BWAPI::BroodwarPtr != nullptr );
    */
    /*
    if (BWAPI::CLIENT_VERSION != BWAPI::Broodwar->getClientVersion())
    {
      //error
      std::cerr << "Error: Client and Server are not compatible!" << std::endl;
      std::cerr << "Client version: " << BWAPI::CLIENT_VERSION << std::endl;
      std::cerr << "Server version: " << BWAPI::Broodwar->getClientVersion() << std::endl;
      disconnect();
      std::this_thread::sleep_for(std::chrono::milliseconds{ 2000 });
      return false;
    }
    */
    //wait for permission from server before we resume execution
    int code = 1;
    while ( code != 2 )
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(code), &receivedByteCount, NULL);
      if ( !success )
      {
        disconnect();
        std::cerr << "Unable to read pipe object." << std::endl;
        return false;
      }
    }
    
    std::cout << "Connection successful" << std::endl;
    //assert( BWAPI::BroodwarPtr != nullptr);

    this->connected = true;
    return true;
  }
  void Client::disconnect()
  {
    if ( !this->connected ) return;
    
    if ( gameTableFileHandle != INVALID_HANDLE_VALUE )
      CloseHandle(gameTableFileHandle);
    gameTableFileHandle = INVALID_HANDLE_VALUE;

    if ( pipeObjectHandle != INVALID_HANDLE_VALUE )
      CloseHandle(pipeObjectHandle);
    pipeObjectHandle = INVALID_HANDLE_VALUE;
    
    if ( mapFileHandle != INVALID_HANDLE_VALUE )
      CloseHandle(mapFileHandle);
    mapFileHandle = INVALID_HANDLE_VALUE;

    this->connected = false;
    std::cout << "Disconnected" << std::endl;

    //if ( BWAPI::BroodwarPtr )
    //  delete static_cast<GameImpl*>(BWAPI::BroodwarPtr);
    //BWAPI::BroodwarPtr = nullptr;
  }
  void Client::update(Game& game)
  {
    DWORD writtenByteCount;
    int code = 1;
    WriteFile(pipeObjectHandle, &code, sizeof(code), &writtenByteCount, NULL);
    //std::cout << "wrote to pipe" << std::endl;

    while (code != 2)
    {
      DWORD receivedByteCount;
      //std::cout << "reading pipe" << std::endl;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(code), &receivedByteCount, NULL);
      if ( !success )
      {
        std::cout << "failed, disconnecting" << std::endl;
        disconnect();
        return;
      }
    }
    //std::cout << "about to enter event loop" << std::endl;

    for(int i = 0; i < data->eventCount; ++i)
    {
      EventType::Enum type(data->events[i].type);

      if ( type == EventType::MatchStart )
        onMatchStart(game);
      if ( type == EventType::MatchFrame || type == EventType::MenuFrame )
        onMatchFrame(game);
    }
    if ( wasInGame && !game.isInGame() )
      onMatchEnd(game);
  }

  void Client::onMatchFrame(Game& game)
  {
    /*events.clear();
    bullets.clear();
    for (int i = 0; i < 100; ++i)
    {
      if (bulletVector[i].exists())
        bullets.insert(&bulletVector[i]);
    }
    nukeDots.clear();
    for (int i = 0; i < data->nukeDotCount; ++i)
      nukeDots.push_back(Position(data->nukeDots[i].x, data->nukeDots[i].y));

    for (int e = 0; e < data->eventCount; ++e)
    {
      events.push_back(this->makeEvent(data->events[e]));
      int id = data->events[e].v1;
      if (data->events[e].type == EventType::UnitDiscover)
      {
        Unit u = &unitVector[id];
        accessibleUnits.insert(u);
        static_cast<PlayerImpl*>(u->getPlayer())->units.insert(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.insert(u);
          if (u->getType().isMineralField())
            minerals.insert(u);
          else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
            geysers.insert(u);
        }
        else
        {
          if (u->getPlayer() == this->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.insert(u);
        }
      }
      else if (data->events[e].type == EventType::UnitEvade)
      {
        Unit u = &unitVector[id];
        accessibleUnits.erase(u);
        static_cast<PlayerImpl*>(u->getPlayer())->units.erase(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.erase(u);
          if (u->getType().isMineralField())
            minerals.erase(u);
          else if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
            geysers.erase(u);
        }
        else
        {
          if (u->getPlayer() == this->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.erase(u);
        }
      }
      else if (data->events[e].type == EventType::UnitRenegade)
      {
        Unit u = &unitVector[id];
        for (auto &p : playerSet)
          static_cast<PlayerImpl*>(p)->units.erase(u);
        static_cast<PlayerImpl*>(u->getPlayer())->units.insert(u);
      }
      else if (data->events[e].type == EventType::UnitMorph)
      {
        Unit u = &unitVector[id];
        if (u->getType() == UnitTypes::Resource_Vespene_Geyser)
        {
          geysers.insert(u);
          neutralUnits.insert(u);
        }
        else if (u->getType().isRefinery())
        {
          geysers.erase(u);
          neutralUnits.erase(u);
        }
      }
    }
    for (Unit ui : accessibleUnits)
    {
      UnitImpl *u = static_cast<UnitImpl*>(ui);
      u->connectedUnits.clear();
      u->loadedUnits.clear();
    }
    for (Unit u : accessibleUnits)
    {
      if (u->getType() == UnitTypes::Zerg_Larva && u->getHatchery())
        static_cast<UnitImpl*>(u->getHatchery())->connectedUnits.insert(u);
      if (u->getType() == UnitTypes::Protoss_Interceptor && u->getCarrier())
        static_cast<UnitImpl*>(u->getCarrier())->connectedUnits.insert(u);
      if (u->getTransport())
        static_cast<UnitImpl*>(u->getTransport())->loadedUnits.insert(u);
    }
    selectedUnits.clear();
    for (int i = 0; i < data->selectedUnitCount; ++i)
    {
      Unit u = getUnit(data->selectedUnits[i]);
      if (u)
        selectedUnits.insert(u);
    }
    // clear player sets
    _allies.clear();
    _enemies.clear();
    _observers.clear();
    if (thePlayer)
    {
      // iterate each player
      for (Player p : playerSet)
      {
        // check if player should be skipped
        if (!p || p->leftGame() || p->isDefeated() || p == thePlayer)
          continue;
        // add player to allies set
        if (thePlayer->isAlly(p))
          _allies.insert(p);
        // add player to enemy set
        if (thePlayer->isEnemy(p))
          _enemies.insert(p);
        // add player to obs set
        if (p->isObserver())
          _observers.insert(p);
      }
    }*/
  }
  void Client::clearAll()
  {
    players.clear();
    units.clear();
    regions.clear();
    bullets.clear();
    forces.clear();
  }
  void Client::onMatchStart(Game& game)
  {
    clearAll();
    wasInGame = true;

    initGame(game);

    initForces(game);
    initPlayers(game);
    initInitialUnits(game);
    initRegions(game);

    onMatchFrame(game);
    // staticMinerals = minerals;
    // staticGeysers = geysers;
    // staticNeutralUnits = neutralUnits;
    // textSize = Text::Size::Default;
  }
  void Client::initForces(Game& game)
  {
    /*for (int i = 1; i < data->forceCount; ++i)
    {
      forces.emplace(std::make_pair(i, ForceData{ game, ForceID(i) }));
      ForceData& newForceData = forces[i];
      bwapi4To5(data->forces[i], newForceData);

      game.forces.emplace(newForceData);
    }*/
  }
  void Client::initPlayers(Game& game)
  {
    /*for (int i = 0; i < data->playerCount; ++i)
    {
      ForceData force = forces[data->players[i].force];
      players.emplace(std::make_pair(i, PlayerData{ game, PlayerID(i), force.id }));
      PlayerData& newPlayerData = players[i];
      bwapi4To5(data->players[i], newPlayerData);

      game.players.emplace(newPlayerData);

      force.players.emplace_back(newPlayerData.id);
    }*/
  }
  void Client::initInitialUnits(Game& game)
  {
    // TODO figure out where these things come from
    /*for (int i = 0; i < data->initialUnitCount; ++i)
    {
      if (unitVector[i].exists())
        accessibleUnits.insert(&unitVector[i]);
      //save the initial state of each initial unit
      unitVector[i].saveInitialState();
    }*/
  }
  void Client::initRegions(Game& game)
  {
    /*for (int i = 0; i < data->regionCount; ++i)
    {
      auto &newRegionData = *regions.emplace(game, RegionID{ i }).first;
      bwapi4To5(data->regions[i], newRegionData);

      game.regions.emplace(newRegionData);
    }*/
  }
  void Client::onMatchEnd(Game& game)
  {
    clearAll();
  }

  void Client::initGame(Game& game)
  {
    // game.gameData->apiVersion = data->apiVersion;
    // game.gameData->engine = data->engine;
    // game.gameData->engineVersion = data->engineVersion;
    // game.gameData->tournament = data->tournament;

    game.gameData->gameType = data->gameType;
    game.gameData->frameCount = data->frameCount;
    game.gameData->latencyFrames = data->latencyFrames;
    // game.gameData->turnSize = data->turnSize;
    // game.gameData->gameSpeed = data->gameSpeed;
    // game.gameData->frameSkip = data->frameSkip;
    game.gameData->remainingLatencyFrames = data->remainingLatencyFrames;

    // game.gameData->replayVisionPlayers = data->replayVisionPlayers;

    game.gameData->remainingLatencyTime = data->remainingLatencyTime;
    game.gameData->elapsedTime = data->elapsedTime;
    // game.gameData->millisecondsPerFrame = data->millisecondsPerFrame;
    game.gameData->averageFPS = data->averageFPS;

    game.gameData->countdownTimer = data->countdownTimer;
    game.gameData->isPaused = data->isPaused;
    game.gameData->isInGame = data->isInGame;
    game.gameData->isMultiplayer = data->isMultiplayer;
    game.gameData->isReplay = data->isReplay;
    // game.gameData->clientUnitSelection = data->clientUnitSelection;
    game.gameData->hasGUI = data->hasGUI;

    game.gameData->mapPath = data->mapPathName;
    game.gameData->mapName = data->mapName;
    // game.gameData->gameName = data->gameName;
    game.gameData->randomSeed = data->randomSeed;

    game.gameData->startPositions.clear();
    for (int i = 0; i < data->startLocationCount; ++i)
    {
      game.gameData->startPositions.emplace_back(data->startLocations[i].x, data->startLocations[i].y);
    }

    game.gameData->regions.clear();
    for (int i = 0; i < data->regionCount; ++i)
    {
      game.gameData->regions.emplace_back(data->regions[i].id);
    }

    game.gameData->player = PlayerID(data->self);

    // game.gameData->screenSize = data->screenSize;
    game.gameData->screenPosition = Position(data->screenX, data->screenY);

    game.gameData->map.size = TilePosition(data->mapWidth, data->mapHeight);
    // game.gameData->map.tileset = data->tileset;

    std::memcpy(game.gameData->map.groundHeight, data->getGroundHeight, sizeof(data->getGroundHeight));
    std::memcpy(game.gameData->map.isBuildable, data->isBuildable, sizeof(data->isBuildable));
    std::memcpy(game.gameData->map.isVisible, data->isVisible, sizeof(data->isVisible));
    std::memcpy(game.gameData->map.isExplored, data->isExplored, sizeof(data->isExplored));
    std::memcpy(game.gameData->map.hasCreep, data->hasCreep, sizeof(data->hasCreep));
    std::memcpy(game.gameData->map.isOccupied, data->isOccupied, sizeof(data->isOccupied));
    std::memcpy(game.gameData->map.isWalkable, data->isWalkable, sizeof(data->isWalkable));
  }
  void Client::updateGame(Game& game)
  {

  }
}
