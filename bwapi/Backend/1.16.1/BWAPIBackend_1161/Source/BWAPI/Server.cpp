#include "Server.h"

#include "../Convenience.h"

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "BulletImpl.h"
#include "RegionImpl.h"
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
    // Reset these variables.
    data->eventCount = 0;
    data->stringCount = 0;
    data->shapeCount = 0;
    if (isConnected())
    {
      // Update BWAPI Client
      updateGameData();
      callOnFrame();
      processMessages();
    }
    else
    {
      BroodwarImpl.processEvents();
      BroodwarImpl.events.clear();
      checkForConnections();
    }
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

    auto staticMapMessage = std::make_unique<bwapi::message::Message>();
    auto staticMap = staticMapMessage->mutable_frameupdate()->mutable_game()->mutable_gamedata()->mutable_staticmap();

    // Locally store the map size
    TilePosition mapSize(BroodwarImpl.mapWidth(), BroodwarImpl.mapHeight());
    WalkPosition mapWalkSize(mapSize);

    // Load walkability
    for (int x = 0; x < mapWalkSize.x; ++x)
      for (int y = 0; y < mapWalkSize.y; ++y)
      {
        data->isWalkable[x][y] = BroodwarImpl.isWalkable(x, y);
      }

    auto isWalkableArr = &data->isWalkable[0][0];
    *staticMap->mutable_iswalkable() = { isWalkableArr, isWalkableArr + 1024 * 1024 };

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

    auto isBuildableArr = &data->isBuildable[0][0];
    *staticMap->mutable_isbuildable() = { isBuildableArr, isBuildableArr + 256 * 256 };
    auto groundHeightArr = &data->getGroundHeight[0][0];
    *staticMap->mutable_groundheight() = { groundHeightArr, groundHeightArr + 256 * 256 };
    auto mapTileRegionIdArr = &data->mapTileRegionId[0][0];
    *staticMap->mutable_maptileregionid() = { mapTileRegionIdArr, mapTileRegionIdArr + 256 * 256 };

    // Load pathing info
    if (BW::BWDATA::SAIPathing)
    {
      data->regionCount = BW::BWDATA::SAIPathing->regionCount;
      auto regionMessage = std::make_unique<bwapi::message::Message>();
      auto regionUpdate = regionMessage->mutable_frameupdate();
      auto regionGame = regionUpdate->mutable_game();
      for (int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesMiniTileMask[i] = BW::BWDATA::SAIPathing->splitTiles[i].minitileMask;
        data->mapSplitTilesRegion1[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn2;

        BWAPI::Region r = BroodwarImpl.getRegion(i);
        if (r)
        {
          auto fillRegionData = [](const RegionData &regionData, bwapi::data::Region *r)
          {
            r->set_bottommost(regionData.bottomMost);
            r->set_center_x(regionData.center_x);
            r->set_center_y(regionData.center_y);
            r->set_id(regionData.id);
            r->set_isaccessible(regionData.isAccessible);
            r->set_ishigherground(regionData.isHigherGround);
            r->set_islandid(regionData.islandID);
            r->set_leftmost(regionData.leftMost);
            r->set_neighborcount(regionData.neighborCount);
            *r->mutable_neighbors() = { std::begin(regionData.neighbors), std::end(regionData.neighbors) };            
            r->set_priority(regionData.priority);
            r->set_rightmost(regionData.rightMost);
            r->set_topmost(regionData.topMost);
          };
          data->regions[i] = *static_cast<RegionImpl*>(r)->getData();
          auto &regionData = data->regions[i];
          auto region = regionGame->add_regions();
          fillRegionData(regionData, region);
        }
        else
        {
          MemZero(data->regions[i]);
        }
      }
      protoClient.queueMessage(std::move(regionMessage));
    }
    *staticMap->mutable_mapsplittilesregion1() = { data->mapSplitTilesRegion1, data->mapSplitTilesRegion1 + 5000 };
    *staticMap->mutable_mapsplittilesregion2() = { data->mapSplitTilesRegion2, data->mapSplitTilesRegion2 + 5000 };

    // Store the map size
    data->mapWidth = mapSize.x;
    data->mapHeight = mapSize.y;
    auto size = staticMap->mutable_size();
    size->set_x(data->mapWidth);
    size->set_y(data->mapHeight);

    // Retrieve map strings
    StrCopy(data->mapFileName, BroodwarImpl.mapFileName());
    staticMap->set_mapfilename(data->mapFileName);
    StrCopy(data->mapPathName, BroodwarImpl.mapPathName());
    staticMap->set_mappath(data->mapPathName);
    StrCopy(data->mapName, BroodwarImpl.mapName());
    staticMap->set_mapname(data->mapName);
    StrCopy(data->mapHash, BroodwarImpl.mapHash());
    staticMap->set_maphash(data->mapHash);

    protoClient.queueMessage(std::move(staticMapMessage));

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

    auto fillForceMessage = [&](const Force &f, bwapi::data::Force *force)
    {
      *force->mutable_name() = f->getName();
      force->set_id(getForceID(f));
      for (auto p : f->getPlayers())
        force->add_players(p->getID());
    };

    auto forceMessage = std::make_unique<bwapi::message::Message>();
    auto forceFrameUpdate = forceMessage->mutable_frameupdate();
    auto forceGame = forceFrameUpdate->mutable_game();

    for (auto f : BroodwarImpl.getForces())
    {
      auto force = forceGame->add_forces();
      fillForceMessage(f, force);
    }
    protoClient.queueMessage(std::move(forceMessage));

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
    //gamename
    std::stringstream randomSeed;
    randomSeed << data->randomSeed;
    gameData->set_randomseed(randomSeed.str());

    auto fillUnitMessage = [](const Unit &bwu, const UnitData &u, bwapi::data::Unit *unit) {
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
      *unit->mutable_isvisible() = { u.isVisible, u.isVisible + 9 };
      unit->set_killcount(u.killCount);
      for (auto &l : bwu->getLarva())
        unit->add_larva(l->getID());
      unit->set_lastattackerplayer(u.lastAttackerPlayer);
      //unit->set_lastcommand() <-- Do we want to pass this?
      //unit->set_lastcommandframe() <-- Do we want to pass this?
      unit->set_lasthitpoints(u.lastHitPoints);
      for (auto &lu : bwu->getLoadedUnits())
        unit->add_loadedunits(lu->getID());
      unit->set_lockdowntimer(u.lockdownTimer);
      unit->set_maelstromtimer(u.maelstromTimer);
      unit->set_nydusexit(u.nydusExit);
      unit->set_order(u.order);
      unit->set_ordertarget(u.orderTarget);
      unit->set_ordertimer(u.orderTimer);
      unit->set_plaguetimer(u.plagueTimer);
      unit->set_player(u.player);
      unit->set_powerup(u.powerUp);
      unit->set_rallyunit(u.rallyUnit);
      unit->set_recentlyattacked(u.recentlyAttacked);
      unit->set_remainingbuildtime(u.remainingBuildTime);
      unit->set_remainingresearchtime(u.remainingResearchTime);
      unit->set_remainingtraintime(u.remainingTrainTime);
      unit->set_remainingupgradetime(u.remainingUpgradeTime);
      unit->set_removetimer(u.removeTimer);
      unit->set_replayid(u.replayID);
      unit->set_resourcegroup(u.resourceGroup);
      unit->set_resources(u.resources);
      unit->set_scarabcount(u.scarabCount);
      unit->set_secondaryorder(u.secondaryOrder);
      unit->set_shields(u.shields);
      unit->set_spellcooldown(u.spellCooldown);
      unit->set_spiderminecount(u.spiderMineCount);
      unit->set_stasistimer(u.stasisTimer);
      unit->set_stimtimer(u.stimTimer);
      unit->set_target(u.target);
      unit->set_tech(u.tech);
      for (int i = 0; i < 5; i++)
        unit->add_trainingqueue(u.trainingQueue[i]);
      unit->set_transport(u.transport);
      unit->set_type(u.type);
      unit->set_upgrade(u.upgrade);
      unit->set_velocityx(u.velocityX);
      unit->set_velocityy(u.velocityY);
    };

    auto unitsMessage = std::make_unique<bwapi::message::Message>();
    auto unitsFrameUpdate = unitsMessage->mutable_frameupdate();
    auto unitsGame = unitsFrameUpdate->mutable_game();
    for (auto &bwunit : BroodwarImpl.getAllUnits())
    {
      auto &u = data->units[getUnitID(bwunit)];
      auto unit = unitsGame->add_units();
      fillUnitMessage(bwunit, u, unit);
    }
    for (auto &bwunit : BroodwarImpl.evadeUnits)
    {
      auto &u = data->units[getUnitID(bwunit)];
      auto unit = unitsGame->add_units();
      fillUnitMessage(bwunit, u, unit);
    }
    protoClient.queueMessage(std::move(unitsMessage));

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
    if (data->isInGame)
    {
      for (auto location : data->startLocations)
      {
        auto startLocation = gameData->add_startpositions();
        startLocation->set_x(location.x);
        startLocation->set_y(location.y);
        startLocation->set_scale(1);
      }

      if (BroodwarImpl.self())
        gameData->set_player(BroodwarImpl.self()->getID());

      auto playersMessage = std::make_unique<bwapi::message::Message>();
      auto playersFrameUpdate = playersMessage->mutable_frameupdate();
      auto playersGame = playersFrameUpdate->mutable_game();
      for (int p = 0; p < data->playerCount; p++)
      {
        auto player = playersGame->add_players();
        player->set_id(p);
        auto &pdata = data->players[p];
        auto array_size = sizeof(pdata.allUnitCount) / sizeof(int);
        *player->mutable_allunitcount() = { pdata.allUnitCount, pdata.allUnitCount + array_size };
        *player->mutable_completedunitcount() = { pdata.completedUnitCount, pdata.completedUnitCount + array_size };
        *player->mutable_deadunitcount() = { pdata.deadUnitCount, pdata.deadUnitCount + array_size };
        *player->mutable_isunitavailable() = { pdata.isUnitAvailable, pdata.isUnitAvailable + array_size };
        *player->mutable_killedunitcount() = { pdata.killedUnitCount, pdata.killedUnitCount + array_size };
        *player->mutable_visibleunitcount() = { pdata.visibleUnitCount, pdata.visibleUnitCount + array_size };
        player->set_color(pdata.color);
        player->set_customscore(pdata.customScore);
        player->set_gas(pdata.gas);
        player->set_gatheredgas(pdata.gatheredGas);
        player->set_gatheredminerals(pdata.gatheredMinerals);
        for (int i = 0; i < 47; i++)
        {
          player->add_hasresearched(pdata.hasResearched[i]);
          player->add_isresearchavailable(pdata.isResearchAvailable[i]);
          player->add_isresearching(pdata.isResearching[i]);
        }
        for (int i = 0; i < 12; i++)
        {
          player->add_isally(pdata.isAlly[i]);
          player->add_isenemy(pdata.isEnemy[i]);
        }
        player->set_isdefeated(pdata.isDefeated);
        player->set_isneutral(pdata.isNeutral);
        player->set_isparticipating(pdata.isParticipating);
        for (int i = 0; i < 63; i++)
        {
          player->add_isupgrading(pdata.isUpgrading[i]);
          player->add_maxupgradelevel(pdata.maxUpgradeLevel[i]);
          player->add_upgradelevel(pdata.upgradeLevel[i]);
        }
        player->set_isvictorious(pdata.isVictorious);
        player->set_leftgame(pdata.leftGame);
        player->set_minerals(pdata.minerals);
        player->set_name(pdata.name);
        player->set_race(pdata.race);
        player->set_refundedgas(pdata.refundedGas);
        player->set_refundedminerals(pdata.refundedMinerals);
        player->set_repairedgas(pdata.repairedGas);
        player->set_repairedminerals(pdata.repairedMinerals);
        player->set_startlocationx(pdata.startLocationX);
        player->set_startlocationy(pdata.startLocationY);
        for (int i = 0; i < 3; i++)
        {
          player->add_supplytotal(pdata.supplyTotal[i]);
          player->add_supplyused(pdata.supplyUsed[i]);
        }
        player->set_totalbuildingscore(pdata.totalBuildingScore);
        player->set_totalkillscore(pdata.totalKillScore);
        player->set_totalrazingscore(pdata.totalRazingScore);
        player->set_totalunitscore(pdata.totalUnitScore);
        player->set_type(pdata.type);
      }
      protoClient.queueMessage(std::move(playersMessage));
 
      //screensize
      auto screenPosition = gameData->mutable_screenposition();
      screenPosition->set_x(BroodwarImpl.getScreenPosition().x);
      screenPosition->set_y(BroodwarImpl.getScreenPosition().y);
      auto mapData = gameData->mutable_map();

      
      auto bulletsMessage = std::make_unique<bwapi::message::Message>();
      auto fillBulletData = [](const BulletData &b, bwapi::data::Bullet *bulletData, int id)
      {
        auto setPosition = [](auto& p, auto& x, auto& y, auto s)
        {
          p->set_x(x);
          p->set_y(y);
          p->set_scale(s);
        };
        auto position = bulletData->mutable_position();
        setPosition(position, b.positionX, b.positionY, 1);
        auto targetPosition = bulletData->mutable_targetposition();
        setPosition(targetPosition, b.targetPositionX, b.targetPositionY, 1);
        bulletData->set_angle(b.angle);
        bulletData->set_exists(b.exists);
        bulletData->set_id(id);
        *bulletData->mutable_isvisible() = { b.isVisible, b.isVisible + 9 };
        bulletData->set_player(b.player);
        bulletData->set_removetimer(b.removeTimer);
        bulletData->set_source(b.source);
        bulletData->set_target(b.target);
        bulletData->set_type(b.type);
        bulletData->set_velocityx(b.velocityX);
        bulletData->set_velocityy(b.velocityY);
      };
      for (int i = 0; i < 100; i++)
      {
        auto bulletData = bulletsMessage->mutable_frameupdate()->mutable_game()->add_bullets();
        fillBulletData(data->bullets[i], bulletData, i);
      }
      protoClient.queueMessage(std::move(bulletsMessage));

      //tileset


      auto isVisibleArr = &data->isVisible[0][0];
      *mapData->mutable_isvisible() = { isVisibleArr, isVisibleArr + 256 * 256 };
      auto isExploredArr = &data->isExplored[0][0];
      *mapData->mutable_isexplored() = { isExploredArr, isExploredArr + 256 * 256 };
      auto hasCreepArr = &data->hasCreep[0][0];
      *mapData->mutable_hascreep() = { hasCreepArr, hasCreepArr + 256 * 256 };
      auto isOccupiedArr = &data->isOccupied[0][0];
      *mapData->mutable_isoccupied() = { isOccupiedArr, isOccupiedArr + 256 * 256 };
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
        else if (command.has_shape() && BroodwarImpl.isInGame())
        {
          assert(data->shapeCount < GameData::MAX_SHAPES);
          auto shape = command.shape();
          data->shapes[data->shapeCount] = BWAPIC::Shape{ static_cast<BWAPIC::ShapeType::Enum>(shape.type()), static_cast<BWAPI::CoordinateType::Enum>(shape.ctype()), shape.x1(), shape.y1(), shape.x2(), shape.y2(), shape.extra1(), shape.extra2(), shape.color(), shape.issolid() };
          data->shapeCount++;
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