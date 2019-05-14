#include "Server.h"

#include<Util/Convenience.h>

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "BulletImpl.h"
#include "RegionImpl.h"
#include <BWAPI/Client/GameTable.h>
#include <BWAPI/Client/GameData.h>

#include <BW/Pathing.h>
#include <BW/Offsets.h>

#include "../Config.h"
#include "../svnrev.h"

namespace BWAPI
{
  Server::Server()
  {
    data = new GameData;
    //data = std::make_unique<GameData>();
    initializeGameData();
  }
  Server::~Server()
  {
    //Do we need to disconect the protoclient here?
    delete data;
  }
  void Server::update()
  {
    if (connected)
    {
      // Update BWAPI Client
      updateGameData();
      callOnFrame();
      processMessages();
    }
    else
    {
      checkForConnections();
    }
  }
  bool Server::isConnected() const
  {
    return connected;
  }
  int Server::addEvent(const BWAPI::Event& e)
  {
    assert(data->eventCount < GameData::MAX_EVENTS);
    BWAPIC::Event* e2 = &(data->events[data->eventCount++]);
    int id = data->eventCount;
    e2->type = e.getType();
    e2->v1 = 0;
    e2->v2 = 0;
    switch (e.getType())
    {
    case BWAPI::EventType::MatchEnd:
      e2->v1 = e.isWinner();
      break;
    case BWAPI::EventType::SendText:
    case BWAPI::EventType::SaveGame:
      e2->v1 = addString(e.getText().c_str());
      break;
    case BWAPI::EventType::PlayerLeft:
      e2->v1 = getPlayerID(e.getPlayer());
      break;
    case BWAPI::EventType::ReceiveText:
      e2->v1 = getPlayerID(e.getPlayer());
      e2->v2 = addString(e.getText().c_str());
      break;
    case BWAPI::EventType::NukeDetect:
      e2->v1 = e.getPosition().x;
      e2->v2 = e.getPosition().y;
      break;
    case BWAPI::EventType::UnitDiscover:
    case BWAPI::EventType::UnitEvade:
    case BWAPI::EventType::UnitCreate:
    case BWAPI::EventType::UnitDestroy:
    case BWAPI::EventType::UnitMorph:
    case BWAPI::EventType::UnitShow:
    case BWAPI::EventType::UnitHide:
    case BWAPI::EventType::UnitRenegade:
    case BWAPI::EventType::UnitComplete:
      e2->v1 = getUnitID(e.getUnit());
      break;
    default:
      break;
    }
    return id;
  }
  int Server::addString(const char* text)
  {
    StrCopy(data->eventStrings[data->eventStringCount], text);
    return data->eventStringCount++;
  }
  void Server::clearAll()
  {
    //clear force info
    forceVector.clear();
    forceLookup.clear();

    //clear player info
    playerVector.clear();
    playerLookup.clear();

    //clear unit info
    unitVector.clear();
    unitLookup.clear();
  }
  int Server::getForceID(Force force)
  {
    if (!force)
      return -1;
    if (forceLookup.find(force) == forceLookup.end())
    {
      forceLookup[force] = (int)(forceVector.size());
      forceVector.push_back(force);
    }
    return forceLookup[force];
  }
  Force Server::getForce(int id) const
  {
    if (forceVector.size() <= static_cast<unsigned>(id))
      return nullptr;
    return forceVector[id];
  }
  int Server::getPlayerID(Player player)
  {
    if (!player)
      return -1;
    if (playerLookup.find(player) == playerLookup.end())
    {
      playerLookup[player] = (int)(playerVector.size());
      playerVector.push_back(player);
    }
    return playerLookup[player];
  }
  Player Server::getPlayer(int id) const
  {
    if (playerVector.size() <= static_cast<unsigned>(id))
      return nullptr;
    return playerVector[id];
  }
  int Server::getUnitID(Unit unit)
  {
    if (!unit)
      return -1;
    if (unitLookup.find(unit) == unitLookup.end())
    {
      unitLookup[unit] = (int)(unitVector.size());
      unitVector.push_back(unit);
    }
    return unitLookup[unit];
  }
  Unit Server::getUnit(int id) const
  {
    if (unitVector.size() <= static_cast<unsigned>(id))
      return nullptr;
    return unitVector[id];
  }
  void Server::onMatchStart()
  {
    data->self = getPlayerID(BroodwarImpl.self());
    data->enemy = getPlayerID(BroodwarImpl.enemy());
    data->neutral = getPlayerID(BroodwarImpl.neutral());
    data->isMultiplayer = BroodwarImpl.isMultiplayer();
    data->isBattleNet = BroodwarImpl.isBattleNet();
    data->isReplay = BroodwarImpl.isReplay();

    // Locally store the map size
    TilePosition mapSize(BroodwarImpl.mapWidth(), BroodwarImpl.mapHeight());
    WalkPosition mapWalkSize(mapSize);

    // Load walkability
    for (int x = 0; x < mapWalkSize.x; ++x)
      for (int y = 0; y < mapWalkSize.y; ++y)
      {
        data->isWalkable[x][y] = BroodwarImpl.isWalkable(x, y);
      }

    // Load buildability, ground height, tile region id
    for (int x = 0; x < mapSize.x; ++x)
      for (int y = 0; y < mapSize.y; ++y)
      {
        data->isBuildable[x][y] = BroodwarImpl.isBuildable(x, y);
        data->getGroundHeight[x][y] = BroodwarImpl.getGroundHeight(x, y);
        if (BW::BWDATA::SAIPathing)
          data->mapTileRegionId[x][y] = BW::BWDATA::SAIPathing->mapTileRegionId[y][x];
        else
          data->mapTileRegionId[x][y] = 0;
      }

    // Load pathing info
    if (BW::BWDATA::SAIPathing)
    {
      data->regionCount = BW::BWDATA::SAIPathing->regionCount;
      for (int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesMiniTileMask[i] = BW::BWDATA::SAIPathing->splitTiles[i].minitileMask;
        data->mapSplitTilesRegion1[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn2;

        BWAPI::Region r = BroodwarImpl.getRegion(i);
        if (r)
        {
          data->regions[i] = *static_cast<RegionImpl*>(r)->getData();
        }
        else
        {
          MemZero(data->regions[i]);
        }
      }
    }

    // Store the map size
    data->mapWidth = mapSize.x;
    data->mapHeight = mapSize.y;

    // Retrieve map strings
    StrCopy(data->mapFileName, BroodwarImpl.mapFileName());
    StrCopy(data->mapPathName, BroodwarImpl.mapPathName());
    StrCopy(data->mapName, BroodwarImpl.mapName());
    StrCopy(data->mapHash, BroodwarImpl.mapHash());

    data->startLocationCount = BroodwarImpl.getStartLocations().size();
    int idx = 0;
    for (TilePosition t : BroodwarImpl.getStartLocations())
    {
      data->startLocations[idx].x = t.x;
      data->startLocations[idx].y = t.y;
      idx++;
    }

    //static force data
    data->forces[0].name[0] = '\0';
    for (Force i : BroodwarImpl.getForces())
    {
      int id = getForceID(i);
      StrCopy(data->forces[id].name, i->getName());
    }

    //static player data
    for (Player i : BroodwarImpl.getPlayers())
    {
      int id = getPlayerID(i);
      PlayerData* p = &(data->players[id]);
      PlayerData* p2 = static_cast<PlayerImpl*>(i)->self;

      StrCopy(p->name, i->getName());
      p->race = i->getRace();
      p->type = i->getType();
      p->force = getForceID(i->getForce());
      p->color = p2->color;
      p->isParticipating = p2->isParticipating;

      for (int j = 0; j < 12; ++j)
      {
        p->isAlly[j] = false;
        p->isEnemy[j] = false;
      }
      for (Player j : BroodwarImpl.getPlayers())
      {
        p->isAlly[getPlayerID(j)] = i->isAlly(j);
        p->isEnemy[getPlayerID(j)] = i->isEnemy(j);
      }
      p->isNeutral = i->isNeutral();
      p->startLocationX = i->getStartLocation().x;
      p->startLocationY = i->getStartLocation().y;
    }

    data->forceCount = forceVector.size();
    data->playerCount = playerVector.size();
    data->initialUnitCount = unitVector.size();

    data->botAPM_noselects = 0;
    data->botAPM_selects = 0;
  }
  void Server::checkForConnections()
  {
    if (connected || localOnly)
      return;
    protoClient.checkForConnection(data->client_version, "x", "x");
    if (!protoClient.isConnected())
      return;
    connected = true;
  }
  void Server::initializeGameData()
  {
    //called once when Starcraft starts. Not at the start of every match.
    data->instanceID = gdwProcNum;
    data->revision = SVN_REV;
    data->client_version = CLIENT_VERSION;
    data->isDebug = (BUILD_DEBUG == 1);
    data->eventCount = 0;
    data->eventStringCount = 0;
    data->commandCount = 0;
    data->unitCommandCount = 0;
    data->shapeCount = 0;
    data->stringCount = 0;
    data->mapFileName[0] = 0;
    data->mapPathName[0] = 0;
    data->mapName[0] = 0;
    data->mapHash[0] = 0;
    data->hasGUI = true;
    data->hasLatCom = true;
    clearAll();
  }
  void Server::updateGameData()
  {
    for (Unit u : BroodwarImpl.evadeUnits)
      data->units[getUnitID(u)] = static_cast<UnitImpl*>(u)->data;

    data->frameCount = BroodwarImpl.getFrameCount();
    data->replayFrameCount = BroodwarImpl.getReplayFrameCount();
    data->randomSeed = BroodwarImpl.getRandomSeed();
    data->fps = BroodwarImpl.getFPS();
    data->botAPM_noselects = BroodwarImpl.getAPM(false);
    data->botAPM_selects = BroodwarImpl.getAPM(true);
    data->latencyFrames = BroodwarImpl.getLatencyFrames();
    data->latencyTime = BroodwarImpl.getLatencyTime();
    data->remainingLatencyFrames = BroodwarImpl.getRemainingLatencyFrames();
    data->remainingLatencyTime = BroodwarImpl.getRemainingLatencyTime();
    data->elapsedTime = BroodwarImpl.elapsedTime();
    data->countdownTimer = BroodwarImpl.countdownTimer();
    data->averageFPS = BroodwarImpl.getAverageFPS();
    data->mouseX = BroodwarImpl.getMousePosition().x;
    data->mouseY = BroodwarImpl.getMousePosition().y;
    data->isInGame = BroodwarImpl.isInGame();
    if (BroodwarImpl.isInGame())
    {
      data->gameType = BroodwarImpl.getGameType();
      data->latency = BroodwarImpl.getLatency();

      // Copy the mouse states
      for (int i = 0; i < M_MAX; ++i)
        data->mouseState[i] = BroodwarImpl.getMouseState((MouseButton)i);

      // Copy the key states
      for (int i = 0; i < K_MAX; ++i)
        data->keyState[i] = BroodwarImpl.getKeyState((Key)i);

      // Copy the screen position
      data->screenX = BroodwarImpl.getScreenPosition().x;
      data->screenY = BroodwarImpl.getScreenPosition().y;

      for (int i = 0; i < BWAPI::Flag::Max; ++i)
        data->flags[i] = BroodwarImpl.isFlagEnabled(i);

      data->isPaused = BroodwarImpl.isPaused();
      data->selectedUnitCount = BroodwarImpl.getSelectedUnits().size();

      int idx = 0;
      for (Unit t : BroodwarImpl.getSelectedUnits())
        data->selectedUnits[idx++] = getUnitID(t);

      //dynamic map data
      Map::copyToSharedMemory();
      //(no dynamic force data)

      //dynamic player data
      for (Player i : BroodwarImpl.getPlayers())
      {
        int id = getPlayerID(i);
        if (id >= 12)
          continue;
        PlayerData* p = &(data->players[id]);
        PlayerData* p2 = static_cast<PlayerImpl*>(i)->self;

        p->isVictorious = i->isVictorious();
        p->isDefeated = i->isDefeated();
        p->leftGame = i->leftGame();
        p->minerals = p2->minerals;
        p->gas = p2->gas;
        p->gatheredMinerals = p2->gatheredMinerals;
        p->gatheredGas = p2->gatheredGas;
        p->repairedMinerals = p2->repairedMinerals;
        p->repairedGas = p2->repairedGas;
        p->refundedMinerals = p2->refundedMinerals;
        p->refundedGas = p2->refundedGas;
        for (int j = 0; j < 3; ++j)
        {
          p->supplyTotal[j] = p2->supplyTotal[j];
          p->supplyUsed[j] = p2->supplyUsed[j];
        }
        for (int j = 0; j < UnitTypes::Enum::MAX; ++j)
        {
          p->allUnitCount[j] = p2->allUnitCount[j];
          p->visibleUnitCount[j] = p2->visibleUnitCount[j];
          p->completedUnitCount[j] = p2->completedUnitCount[j];
          p->deadUnitCount[j] = p2->deadUnitCount[j];
          p->killedUnitCount[j] = p2->killedUnitCount[j];
        }
        p->totalUnitScore = p2->totalUnitScore;
        p->totalKillScore = p2->totalKillScore;
        p->totalBuildingScore = p2->totalBuildingScore;
        p->totalRazingScore = p2->totalRazingScore;
        p->customScore = p2->customScore;

        for (int j = 0; j < 63; ++j)
        {
          p->upgradeLevel[j] = p2->upgradeLevel[j];
          p->isUpgrading[j] = p2->isUpgrading[j];
        }

        for (int j = 0; j < 47; ++j)
        {
          p->hasResearched[j] = p2->hasResearched[j];
          p->isResearching[j] = p2->isResearching[j];
        }
        memcpy(p->isResearchAvailable, p2->isResearchAvailable, sizeof(p->isResearchAvailable));
        memcpy(p->isUnitAvailable, p2->isUnitAvailable, sizeof(p->isUnitAvailable));
        memcpy(p->maxUpgradeLevel, p2->maxUpgradeLevel, sizeof(p->maxUpgradeLevel));
      }

      //dynamic unit data
      for (Unit i : BroodwarImpl.getAllUnits())
        data->units[getUnitID(i)] = static_cast<UnitImpl*>(i)->data;

      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; ++i)
      {
        Unit u = BroodwarImpl.indexToUnit(i);
        int id = -1;
        if (u)
          id = getUnitID(u);
        data->unitArray[i] = id;
      }

      unitFinder* xf = data->xUnitSearch;
      unitFinder* yf = data->yUnitSearch;
      const BW::unitFinder* bwxf = BW::BWDATA::UnitOrderingX.data();
      const BW::unitFinder* bwyf = BW::BWDATA::UnitOrderingY.data();
      int bwSearchSize = BW::BWDATA::UnitOrderingCount;

      for (int i = 0; i < bwSearchSize; ++i, bwxf++, bwyf++)
      {
        if (bwxf->unitIndex > 0 && bwxf->unitIndex <= BW::UNIT_ARRAY_MAX_LENGTH)
        {
          UnitImpl* u = BroodwarImpl.unitArray[bwxf->unitIndex - 1];
          if (u && u->canAccess())
          {
            xf->searchValue = bwxf->searchValue;
            xf->unitIndex = getUnitID(u);
            xf++;
          }
        } // x index

        if (bwyf->unitIndex > 0 && bwyf->unitIndex <= BW::UNIT_ARRAY_MAX_LENGTH)
        {
          UnitImpl* u = BroodwarImpl.unitArray[bwyf->unitIndex - 1];
          if (u && u->canAccess())
          {
            yf->searchValue = bwyf->searchValue;
            yf->unitIndex = getUnitID(u);
            yf++;
          }
        } // x index

      } // loop unit finder

      // Set size
      data->unitSearchSize = xf - data->xUnitSearch; // we assume an equal number of y values was put into the array


      //dynamic bullet data
      for (int id = 0; id < 100; ++id)
        data->bullets[id] = BroodwarImpl.getBulletFromIndex(id)->data;

      //dynamic nuke dot data
      int j = 0;
      data->nukeDotCount = BroodwarImpl.getNukeDots().size();
      for (Position const &nd : BroodwarImpl.getNukeDots())
      {
        data->nukeDots[j].x = nd.x;
        data->nukeDots[j].y = nd.y;
        ++j;
      }
    }

    // iterate events
    for (Event &e : BroodwarImpl.events)
    {
      if (e.getType() == EventType::MatchStart)
      {
        onMatchStart();
      }

      // Add the event to the server queue
      addEvent(e);

      // ignore if tournament AI not loaded
      if (!BroodwarImpl.tournamentAI)
        continue;

      // call the tournament module callbacks for server/client
      BroodwarImpl.isTournamentCall = true;
      GameImpl::SendClientEvent(BroodwarImpl.tournamentAI, e);
      BroodwarImpl.isTournamentCall = false;
    }
    BroodwarImpl.events.clear();
  }
  void Server::callOnFrame()
  {
    //What should we do here?
  }
  void Server::processMessages()
  {
    while (protoClient.messageQueueSize())
    {
      auto message = protoClient.getNextMessage();
      if (message.get() == nullptr)
        return;
      if (message->has_endofqueue())
        return;
      if (message->has_initbroadcast())
      {
        //Logic to decide if we want to connect or disconnect is needed,
        //for now, just constructing the server response.
        auto newMessage = std::make_unique<bwapi::message::Message>();
        auto newServerResponse = std::make_unique<bwapi::init::ServerResponse>();
        newServerResponse->set_enginetype("x");
        newServerResponse->set_engineversion("x");
        newServerResponse->set_apiversion(1);
        protoClient.queueMessage(std::move(newMessage));
      }
      else if (message->has_command())
      {
        //proccess command
        auto command = message->command();
        if (command.has_sendtext())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.sendTextEx(command.sendtext().toallies() != 0, "%s", command.sendtext().text());
        }
      }
    }
  }
  void Server::setWaitForResponse(bool wait)
  {

  }
  void Server::receiveData()
  {
    if (!protoClient.isConnected())
      return;
    protoClient.receiveMessages();
  }
  void Server::sendData()
  {
    if (!protoClient.isConnected())
      return;
    protoClient.transmitMessages();
  }
}