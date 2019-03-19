#include "Client.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

namespace BWAPI
{
  Client BWAPIClient;
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
    if ( this->connected )
    {
      std::cout << "Already connected." << std::endl;
      return true;
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
    events.clear();
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
    }
  }
  void Client::clearAll()
  {

  }
  void Client::onMatchStart(Game& game)
  {
    clearAll();
    wasInGame = true;

    //load forces, players, and initial units from shared memory
    for (int i = 1; i < data->forceCount; ++i)
      forces.insert(&forceVector[i]);
    for (int i = 0; i < data->playerCount; ++i)
      playerSet.insert(&playerVector[i]);
    for (int i = 0; i < data->initialUnitCount; ++i)
    {
      if (unitVector[i].exists())
        accessibleUnits.insert(&unitVector[i]);
      //save the initial state of each initial unit
      unitVector[i].saveInitialState();
    }

    //load start locations from shared memory
    for (int i = 0; i < data->startLocationCount; ++i)
    {
      game.gameData.startPositions.emplace_back(data->startLocations[i].x, data->startLocations[i].y);
    }

    for (int i = 0; i < data->regionCount; ++i)
    {
      this->regionArray[i] = new RegionImpl(i);
      regionsList.insert(this->regionArray[i]);
    }
    for (int i = 0; i < data->regionCount; ++i)
      this->regionArray[i]->setNeighbors();

    thePlayer = getPlayer(data->self);
    theEnemy = getPlayer(data->enemy);
    theNeutral = getPlayer(data->neutral);
    _allies.clear();
    _enemies.clear();
    _observers.clear();
    // check if the current player exists
    if (thePlayer)
    {
      // iterate each player
      for (Player p : playerSet)
      {
        // check if the player should not be updated
        if (!p || p->leftGame() || p->isDefeated() || p == thePlayer)
          continue;
        // add player to allies set
        if (thePlayer->isAlly(p))
          _allies.insert(p);
        // add player to enemies set
        if (thePlayer->isEnemy(p))
          _enemies.insert(p);
        // add player to observers set
        if (p->isObserver())
          _observers.insert(p);
      }
    }
    onMatchFrame(game);
    staticMinerals = minerals;
    staticGeysers = geysers;
    staticNeutralUnits = neutralUnits;
    textSize = Text::Size::Default;
  }
  void Client::onMatchEnd(Game& game)
  {
    clearAll();
  }

  void bwapi4To5(BWAPI4::PlayerData& oldData, PlayerData& newData)
  {
    newData.name = oldData.name;
    newData.race = BWAPI::Race(oldData.race);
    newData.type = BWAPI::PlayerType(oldData.type);
    //newData.force = oldData.force; fuck
    std::copy(std::begin(oldData.isAlly), std::end(oldData.isAlly), newData.isAlly);
    std::copy(std::begin(oldData.isEnemy), std::end(oldData.isEnemy), newData.isEnemy);
    newData.isNeutral = oldData.isNeutral;
    newData.startLocationX = oldData.startLocationX;
    newData.startLocationY = oldData.startLocationY;
    newData.isVictorious = oldData.isVictorious;
    newData.isDefeated = oldData.isDefeated;
    newData.leftGame = oldData.leftGame;
    newData.isParticipating = oldData.isParticipating;

    newData.minerals = oldData.minerals;
    newData.gas = oldData.gas;
    newData.gatheredMinerals = oldData.gatheredMinerals;
    newData.gatheredGas = oldData.gatheredGas;
    newData.repairedMinerals = oldData.repairedMinerals;
    newData.repairedGas = oldData.repairedGas;
    newData.refundedMinerals = oldData.refundedMinerals;
    newData.refundedGas = oldData.refundedGas;
    newData.supplyTotal[0] = oldData.supplyTotal[0];
    newData.supplyTotal[1] = oldData.supplyTotal[1];
    newData.supplyTotal[2] = oldData.supplyTotal[2];
    newData.supplyUsed[0] = oldData.supplyUsed[0];
    newData.supplyUsed[1] = oldData.supplyUsed[1];
    newData.supplyUsed[2] = oldData.supplyUsed[2];

    std::copy(std::begin(oldData.allUnitCount), std::end(oldData.allUnitCount), newData.allUnitCount);
    std::copy(std::begin(oldData.visibleUnitCount), std::end(oldData.visibleUnitCount), newData.visibleUnitCount);
    std::copy(std::begin(oldData.completedUnitCount), std::end(oldData.completedUnitCount), newData.completedUnitCount);
    std::copy(std::begin(oldData.deadUnitCount), std::end(oldData.deadUnitCount), newData.deadUnitCount);
    std::copy(std::begin(oldData.killedUnitCount), std::end(oldData.killedUnitCount), newData.killedUnitCount);

    std::copy(std::begin(oldData.upgradeLevel), std::end(oldData.upgradeLevel), newData.upgradeLevel);
    std::copy(std::begin(oldData.hasResearched), std::end(oldData.hasResearched), newData.hasResearched);
    std::copy(std::begin(oldData.isResearching), std::end(oldData.isResearching), newData.isResearching);
    std::copy(std::begin(oldData.isUpgrading), std::end(oldData.isUpgrading), newData.isUpgrading);

    newData.color = BWAPI::Color(oldData.color);

    newData.totalUnitScore = oldData.totalUnitScore;
    newData.totalKillScore = oldData.totalKillScore;
    newData.totalBuildingScore = oldData.totalBuildingScore;
    newData.totalRazingScore = oldData.totalRazingScore;
    newData.customScore = oldData.customScore;

    std::copy(std::begin(oldData.maxUpgradeLevel), std::end(oldData.maxUpgradeLevel), newData.maxUpgradeLevel);
    std::copy(std::begin(oldData.isResearchAvailable), std::end(oldData.isResearchAvailable), newData.isResearchAvailable);
    std::copy(std::begin(oldData.isUnitAvailable), std::end(oldData.isUnitAvailable), newData.isUnitAvailable);
  }
  void bwapi4To5(BWAPI4::UnitData& oldData, UnitData& newData)
  {

  }
  void bwapi4To5(BWAPI4::RegionData& oldData, RegionData& newData)
  {

  }
  void bwapi4To5(BWAPI4::BulletData& oldData, BulletData& newData)
  {

  }
  void bwapi4To5(BWAPI4::ForceData& oldData, ForceData& newData)
  {

  }
}
