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
    oldData = new GameData;
    //data = std::make_unique<GameData>();
    initializeGameData();
  }
  Server::~Server()
  {
    //Do we need to disconect the protoclient here?
    delete data;
    delete oldData;
  }
  void Server::update()
  {
    if (isConnected())
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
    // Reset these variables.
    data->eventCount = 0;
    data->stringCount = 0;
  }
  bool Server::isConnected() const
  {
    return protoClient.isConnected();
  }
  void Server::addEvent(const BWAPI::Event& e)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newEvent = std::make_unique<bwapi::event::Event>();// newMessage->mutable_event();
    switch (e.getType())
    {
    case BWAPI::EventType::MatchEnd:
    {
      auto newMatchEnd = newEvent->mutable_matchend();
      newMatchEnd->set_winner(e.isWinner());
    }
      break;
    case BWAPI::EventType::SendText:
    {
      auto newSendText = newEvent->mutable_sendtext();
      newSendText->set_text(e.getText().c_str());
    }
    break;
    case BWAPI::EventType::SaveGame:
    {
      auto newSaveGame = newEvent->mutable_savegame();
      newSaveGame->set_text(e.getText());
    }
      break;
    case BWAPI::EventType::PlayerLeft:
    {
      auto newPlayerLeft = newEvent->mutable_playerleft();
      newPlayerLeft->set_player(getPlayerID(e.getPlayer()));
    }
      break;
    case BWAPI::EventType::ReceiveText:
    {
      auto newReceiveText = newEvent->mutable_receivetext();
      newReceiveText->set_player(getPlayerID(e.getPlayer()));
      newReceiveText->set_text(e.getText());
    }
      break;
    case BWAPI::EventType::NukeDetect:
    {
      auto newNukeDetect = newEvent->mutable_nukedetect();
      auto target = newNukeDetect->mutable_target();
      target->set_x(e.getPosition().x);
      target->set_y(e.getPosition().y);
      target->set_scale(1);
    }
      break;
    case BWAPI::EventType::UnitDiscover:
    {
      auto newUnitDiscover = newEvent->mutable_unitdiscover();
      newUnitDiscover->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitEvade:
    {
      auto newUnitEvade = newEvent->mutable_unitevade();
      newUnitEvade->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitCreate:
    {
      auto newUnitCreate = newEvent->mutable_unitcreate();
      newUnitCreate->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitDestroy:
    {
      auto newUnitDestroy = newEvent->mutable_unitdestroy();
      newUnitDestroy->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitMorph:
    {
      auto newUnitMorph = newEvent->mutable_unitmorph();
      newUnitMorph->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitShow:
    {
      auto newUnitShow = newEvent->mutable_unitshow();
      newUnitShow->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitHide:
    {
      auto newUnitHide = newEvent->mutable_unithide();
      newUnitHide->set_unit(getUnitID(e.getUnit()));
    }
      break;
    case BWAPI::EventType::UnitRenegade:
    {
      auto newUnitRenegade = newEvent->mutable_unitrenegade();
      newUnitRenegade->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI::EventType::UnitComplete:
    {
      auto newUnitComplete = newEvent->mutable_unitcomplete();
      newUnitComplete->set_unit(getUnitID(e.getUnit()));
    }
      break;
    default:
      break;
    }
    newMessage->set_allocated_event(newEvent.release());
    protoClient.queueMessage(std::move(newMessage));
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
    if (!listening)
    {
      protoClient.initListen();
      listening = true;
    }
    protoClient.checkForConnection(data->client_version, "x", "x");
    if (!protoClient.isConnected())
      return;
    protoClient.stopListen();
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
    *oldData = *data;
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
      {
        data->units[getUnitID(i)] = static_cast<UnitImpl*>(i)->data;
      }

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
    auto message = std::make_unique<bwapi::message::Message>();
    auto frameUpdate = message->mutable_frameupdate();
    auto game = frameUpdate->mutable_game();
    auto gameData = game->mutable_gamedata();
    //Diff data and oldData
    //if (oldData->gameType != data->gameType)
      gameData->set_gametype(data->gameType);
    gameData->set_framecount(data->frameCount);
    //if (oldData->latencyFrames != data->latencyFrames)
      gameData->set_latencyframes(data->latencyFrames);
    //turnsize
    //gamespeed
    //frameskip
    //if (oldData->remainingLatencyFrames != data->remainingLatencyFrames)
      gameData->set_remaininglatencyframes(data->remainingLatencyFrames);
    gameData->set_lasteventtime(BroodwarImpl.getLastEventTime());
    //replayvisionplayers
    //if (oldData->latencyTime != data->latencyTime)
      gameData->set_latencytime(data->latencyTime);
    //if (oldData->remainingLatencyTime != data->remainingLatencyTime)
      gameData->set_remaininglatencytime(data->remainingLatencyTime);
    //if (oldData->elapsedTime != data->elapsedTime)
      gameData->set_elapsedtime(data->elapsedTime);
    //millisecondsperframe
    gameData->set_averagefps(data->averageFPS);
    gameData->set_countdowntimer(data->countdownTimer);
    gameData->set_ispaused(data->isPaused);
    gameData->set_isingame(data->isInGame);
    gameData->set_ismultiplayer(data->isMultiplayer);
    gameData->set_isbattlenet(data->isBattleNet);
    gameData->set_isreplay(data->isReplay);
    //clientunitselection
    gameData->set_hasgui(data->hasGUI);
    gameData->set_mappath(data->mapPathName);
    gameData->set_mapname(data->mapName);
    gameData->set_mapfilename(data->mapFileName);
    //gamename
    std::stringstream randomSeed;
    randomSeed << data->randomSeed;
    gameData->set_randomseed(randomSeed.str());
    if (data->isInGame)
    {
      for (auto location : data->startLocations)
      {
        auto startLocation = gameData->add_startpositions();
        startLocation->set_x(location.x);
        startLocation->set_y(location.y);
        startLocation->set_scale(1);
      }

      for (auto region : data->regions)
      {
        gameData->add_regions(region.id);
      }
    
      if (BroodwarImpl.self())
        gameData->set_player(BroodwarImpl.self()->getID());

      //screensize
      //screenposition
      auto mapData = gameData->mutable_map();
      auto size = mapData->mutable_size();
      size->set_x(data->mapWidth);
      size->set_y(data->mapHeight);
      //tileset
      mapData->set_maphash(data->mapHash);
      /*
      for (auto& i : data->getGroundHeight)
      {
        for (auto& j : i)
          mapData->add_groundheight(j);
      }
      for (auto& i : data->isBuildable)
      {
        for (auto& j : i)
          mapData->add_isbuildable(j);
      }
      for (auto& i : data->isVisible)
      {
        for (auto& j : i)
          mapData->add_isvisible(j);
      }
      for (auto& i : data->isExplored)
      {
        for (auto& j : i)
          mapData->add_isexplored(j);
      }
      for (auto& i : data->hasCreep)
      {
        for (auto& j : i)
          mapData->add_hascreep(j);
      }
      for (auto& i : data->isOccupied)
      {
        for (auto& j : i)
          mapData->add_isoccupied(i);
      }
      for (auto& i : data->isWalkable)
      {
        for (auto& j : i)
          mapData->add_iswalkable(j);
      }
      for (auto& i : data->mapTileRegionId)
      {
        for (auto& j : i)
          mapData->add_maptileregionid(j);
      }
      for (auto i : data->mapSplitTilesMiniTileMask)
        mapData->add_mapsplittilesminitilemask(i);
      for (auto i : data->mapSplitTilesRegion1)
        mapData->add_mapsplittilesregion1(i);
      for (auto i : data->mapSplitTilesRegion2)
        mapData->add_mapsplittilesregion2(i);
      */


      auto playersMessage = std::make_unique<bwapi::message::Message>();
      auto playersFrameUpdate = playersMessage->mutable_frameupdate();
      auto playersGame = playersFrameUpdate->mutable_game();
      for (int p = 0; p < data->playerCount; p++)
      {
        auto player = playersGame->add_players();
        player->set_id(p);
        auto &pdata = data->players[p];
        player->set_color(pdata.color);
      }
      protoClient.queueMessage(std::move(playersMessage));

      auto fillUnitMessage = [](const UnitData &u, bwapi::data::Unit *unit) {
        auto setPosition = [](auto& p, auto& x, auto& y, auto s)
        {
          p->set_x(x);
          p->set_y(y);
          p->set_scale(s);
        };
        unit->set_acidsporecount(u.acidSporeCount);
        unit->set_addon(u.addon);
        unit->set_airweaponcooldown(u.airWeaponCooldown);
        auto orderTargetPosition = unit->mutable_ordertargetposition();
        setPosition(orderTargetPosition, u.orderTargetPositionX, u.orderTargetPositionY, 1);
        auto position = unit->mutable_position();
        setPosition(position, u.positionX, u.positionY, 1);
        auto rallyPosition = unit->mutable_rallyposition();
        setPosition(rallyPosition, u.rallyPositionX, u.rallyPositionY, 1);
        auto targetPosition = unit->mutable_targetposition();
        setPosition(targetPosition, u.targetPositionX, u.targetPositionY, 1);
        unit->set_angle(u.angle);
        unit->set_buildtype(u.buildType);
        unit->set_buildunit(u.buildUnit);
        unit->set_buttonset(u.buttonset);
        unit->set_carrier(u.carrier);
        unit->set_carryresourcetype(u.carryResourceType);
        unit->set_defensematrixpoints(u.defenseMatrixPoints);
        unit->set_defensematrixtimer(u.defenseMatrixTimer);
        unit->set_energy(u.energy);
        unit->set_ensnaretimer(u.ensnareTimer);
        unit->set_exists(u.exists);
        unit->set_groundweaponcooldown(u.groundWeaponCooldown);
        unit->set_hasnuke(u.hasNuke);
        unit->set_hatchery(u.hatchery);
        unit->set_hitpoints(u.hitPoints);
        unit->set_id(u.id);
        unit->set_interceptorcount(u.interceptorCount);
        unit->set_irradiatetimer(u.irradiateTimer);
        unit->set_isaccelerating(u.isAccelerating);
        unit->set_isattackframe(u.isAttackFrame);
        unit->set_isattacking(u.isAttacking);
        unit->set_isbeinggathered(u.isBeingGathered);
        unit->set_isblind(u.isBlind);
        unit->set_isbraking(u.isBraking);
        unit->set_isburrowed(u.isBurrowed);
        unit->set_iscloaked(u.isCloaked);
        unit->set_iscompleted(u.isCompleted);
        unit->set_isconstructing(u.isConstructing);
        unit->set_isdetected(u.isDetected);
        unit->set_isgathering(u.isGathering);
        unit->set_ishallucination(u.isHallucination);
        unit->set_isidle(u.isIdle);
        unit->set_isinterruptible(u.isInterruptible);
        //unit->set_isbeinghealed()
        unit->set_isinvincible(u.isInvincible);
        unit->set_islifted(u.isLifted);
        unit->set_ismorphing(u.isMorphing);
        unit->set_ismoving(u.isMoving);
        unit->set_isparasited(u.isParasited);
        unit->set_ispowered(u.isPowered);
        unit->set_isselected(u.isSelected);
        unit->set_isstartingattack(u.isStartingAttack);
        unit->set_isstuck(u.isStuck);
        unit->set_istraining(u.isTraining);
        unit->set_isunderdarkswarm(u.isUnderDarkSwarm);
        unit->set_isunderdweb(u.isUnderDWeb);
        unit->set_isunderstorm(u.isUnderStorm);
        unit->set_isvisible(u.isVisible);
        unit->set_killcount(u.killCount);
        unit->set_type(u.type);
      };

      auto unitsMessage = std::make_unique<bwapi::message::Message>();
      auto unitsFrameUpdate = unitsMessage->mutable_frameupdate();
      auto unitsGame = unitsFrameUpdate->mutable_game();
      for (auto &bwunit : BroodwarImpl.getAllUnits())
      {
        auto &u = data->units[getUnitID(bwunit)];
        auto unit = unitsGame->add_units();
        fillUnitMessage(u, unit);
      }
      for (auto &bwunit : BroodwarImpl.evadeUnits)
      {
        auto &u = data->units[getUnitID(bwunit)];
        auto unit = unitsGame->add_units();
        fillUnitMessage(u, unit);
      }
      protoClient.queueMessage(std::move(unitsMessage));
    }
    protoClient.queueMessage(std::move(message));
    //*oldData = *data;
  }
  void Server::callOnFrame()
  {
    protoClient.transmitMessages();
    protoClient.receiveMessages();
    processMessages();
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
        if (command.has_setscreenposition())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.setScreenPosition(command.setscreenposition().x(), command.setscreenposition().y());
        }
        else if (command.has_pingminimap())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.pingMinimap(command.pingminimap().x(), command.pingminimap().y());
        }
        else if (command.has_printf())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.printf("%s", command.printf().text().c_str());
        }
        else if (command.has_sendtext())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.sendTextEx(command.sendtext().toallies() != 0, "%s", command.sendtext().text().c_str());
        }
        else if (command.has_pausegame())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.pauseGame();
        }
        else if (command.has_resumegame())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.resumeGame();
        }
        else if (command.has_leavegame())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.leaveGame();
        }
        else if (command.has_restartgame())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.restartGame();
        }
        else if (command.has_setlocalspeed())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.setLocalSpeed(command.setlocalspeed().speed());
        }
        else if (command.has_setalliance())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.setAlliance(getPlayer(command.setalliance().playerid()),
                                               command.setalliance().settings() != 0,
                                               command.setalliance().settings() == 2);
        }
        else if (command.has_setvision())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.setVision(getPlayer(command.setvision().playerid()),
                                             command.setvision().settings() != 0);
        }
        else if (command.has_setcommandoptimizationlevel())
        {
          if (BroodwarImpl.isInGame())
            BroodwarImpl.setCommandOptimizationLevel(command.setcommandoptimizationlevel().commandoptimizationlevel());
        }
        else if (command.has_unitcommand() && BroodwarImpl.isInGame())
        {
          Unit target = nullptr;
          if (command.unitcommand().targetid() >= 0 && command.unitcommand().targetid() < (int)unitVector.size())
            target = unitVector[command.unitcommand().targetid()];
          for (auto unitID : command.unitcommand().unitid())
          {
            auto unit = unitVector[unitID];
            unit->issueCommand(UnitCommand(unit, 
                                           command.unitcommand().unitcommandtype(),
                                           target, command.unitcommand().x(),
                                           command.unitcommand().y(),
                                           command.unitcommand().extra()));
          }
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