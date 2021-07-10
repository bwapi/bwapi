#include "Server.h"

#include "../Convenience.h"

#include <string>
#include <sstream>

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "BulletImpl.h"
#include "RegionImpl.h"
#include <BWAPI4/Client/GameData.h>

#include "../BW/Pathing.h"
#include "../BW/Offsets.h"
#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>

#include "../Config.h"
#include "../svnrev.h"

#include <BWAPI4/Event.h>

namespace BWAPI4
{
  const int CLIENT_VERSION = 10002;

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
    if (isConnected())
    {
      // Update BWAPI Client
      updateGameData();
      callOnFrame();
      processMessages();
    }
    else
    {
      BroodwarImpl.events.clear();
      checkForConnections();
    }
  }
  bool Server::isConnected() const
  {
    return protoClient.isConnected();
  }
  void Server::addEvent(const BWAPI4::Event& e)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newEvent = newMessage->mutable_event();
    switch (e.getType())
    {
    case BWAPI4::EventType::MatchEnd:
    {
      auto newMatchEnd = newEvent->mutable_matchend();
      newMatchEnd->set_winner(e.isWinner());
    }
    break;
    case BWAPI4::EventType::SendText:
    {
      auto newSendText = newEvent->mutable_sendtext();
      newSendText->set_text(e.getText());
    }
    break;
    case BWAPI4::EventType::SaveGame:
    {
      auto newSaveGame = newEvent->mutable_savegame();
      newSaveGame->set_text(e.getText());
    }
    break;
    case BWAPI4::EventType::PlayerLeft:
    {
      auto newPlayerLeft = newEvent->mutable_playerleft();
      newPlayerLeft->set_player(getPlayerID(e.getPlayer()));
    }
    break;
    case BWAPI4::EventType::ReceiveText:
    {
      auto newReceiveText = newEvent->mutable_receivetext();
      newReceiveText->set_player(getPlayerID(e.getPlayer()));
      newReceiveText->set_text(e.getText());
    }
    break;
    case BWAPI4::EventType::NukeDetect:
    {
      auto newNukeDetect = newEvent->mutable_nukedetect();
      auto target = newNukeDetect->mutable_target();
      target->set_x(e.getPosition().x);
      target->set_y(e.getPosition().y);
      target->set_scale(1);
    }
    break;
    case BWAPI4::EventType::UnitDiscover:
    {
      auto newUnitDiscover = newEvent->mutable_unitdiscover();
      newUnitDiscover->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitEvade:
    {
      auto newUnitEvade = newEvent->mutable_unitevade();
      newUnitEvade->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitCreate:
    {
      auto newUnitCreate = newEvent->mutable_unitcreate();
      newUnitCreate->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitDestroy:
    {
      auto newUnitDestroy = newEvent->mutable_unitdestroy();
      newUnitDestroy->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitMorph:
    {
      auto newUnitMorph = newEvent->mutable_unitmorph();
      newUnitMorph->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitShow:
    {
      auto newUnitShow = newEvent->mutable_unitshow();
      newUnitShow->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitHide:
    {
      auto newUnitHide = newEvent->mutable_unithide();
      newUnitHide->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitRenegade:
    {
      auto newUnitRenegade = newEvent->mutable_unitrenegade();
      newUnitRenegade->set_unit(getUnitID(e.getUnit()));
    }
    break;
    case BWAPI4::EventType::UnitComplete:
    {
      auto newUnitComplete = newEvent->mutable_unitcomplete();
      newUnitComplete->set_unit(getUnitID(e.getUnit()));
    }
    break;
    default:
      break;
    }
    protoClient.queueMessage(std::move(newMessage));
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
  int Server::getForceID(ForceImpl* force)
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
    auto staticMapMessage = std::make_unique<bwapi::message::Message>();
    auto staticMap = staticMapMessage->mutable_frameupdate()->mutable_game()->mutable_gamedata()->mutable_staticmap();

    // Locally store the map size
    TilePosition mapSize(BroodwarImpl.mapWidth(), BroodwarImpl.mapHeight());
    WalkPosition mapWalkSize(mapSize);

    // Load walkability
    auto isWalkable = staticMap->mutable_iswalkable();
    for (int x = 0; x < mapWalkSize.x; ++x)
    {
      for (int y = 0; y < mapWalkSize.y; ++y)
      {
        auto point = isWalkable->Add();
        point->set_x(x);
        point->set_y(y);
        point->set_value(BroodwarImpl.isWalkable(x, y));
      }
    }

    /*auto isWalkableArr = &data->isWalkable[0][0];
    *staticMap->mutable_iswalkable() = { isWalkableArr, isWalkableArr + 1024 * 1024 };*/

    // Load buildability, ground height, tile region id
    auto isBuildable = staticMap->mutable_isbuildable();
    auto groundHeight = staticMap->mutable_groundheight();
    auto mapTileRegionId = staticMap->mutable_maptileregionid();
    for (int x = 0; x < mapSize.x; ++x)
      for (int y = 0; y < mapSize.y; ++y)
      {
        auto buildablePoint = isBuildable->Add();
        buildablePoint->set_x(x);
        buildablePoint->set_y(y);
        buildablePoint->set_value(BroodwarImpl.isBuildable(x, y));

        auto groundPoint = groundHeight->Add();
        groundPoint->set_x(x);
        groundPoint->set_y(y);
        groundPoint->set_value(BroodwarImpl.getGroundHeight(x, y));

        unsigned short rgnId = BW::BWDATA::SAIPathing ? BW::BWDATA::SAIPathing->mapTileRegionId[y][x] : 0;
        auto tilePoint = mapTileRegionId->Add();
        tilePoint->set_x(x);
        tilePoint->set_y(y);
        tilePoint->set_value(rgnId);
      }

    /*auto isBuildableArr = &data->isBuildable[0][0];
    *staticMap->mutable_isbuildable() = { isBuildableArr, isBuildableArr + 256 * 256 };
    auto groundHeightArr = &data->getGroundHeight[0][0];
    *staticMap->mutable_groundheight() = { groundHeightArr, groundHeightArr + 256 * 256 };
    auto mapTileRegionIdArr = &data->mapTileRegionId[0][0];
    *staticMap->mutable_maptileregionid() = { mapTileRegionIdArr, mapTileRegionIdArr + 256 * 256 };*/

    // Load pathing info
    if (BW::BWDATA::SAIPathing)
    {
      auto regionMessage = std::make_unique<bwapi::message::Message>();
      auto regionUpdate = regionMessage->mutable_frameupdate();
      auto regionGame = regionUpdate->mutable_game();
      for (int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesRegion1[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn2;

        BWAPI4::RegionImpl* r = BroodwarImpl.getRegion(i);
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
          auto &regionData = *r->getData();
          auto region = regionGame->add_regions();
          fillRegionData(regionData, region);
        }
      }
      protoClient.queueMessage(std::move(regionMessage));
    }
    *staticMap->mutable_mapsplittilesregion1() = { data->mapSplitTilesRegion1, data->mapSplitTilesRegion1 + 5000 };
    *staticMap->mutable_mapsplittilesregion2() = { data->mapSplitTilesRegion2, data->mapSplitTilesRegion2 + 5000 };

    // Store the map size
    auto size = staticMap->mutable_size();
    size->set_x(mapSize.x);
    size->set_y(mapSize.y);

    // Retrieve map strings
    staticMap->set_mapfilename(BroodwarImpl.mapFileName());
    staticMap->set_mappath(BroodwarImpl.mapPathName());
    staticMap->set_mapname(BroodwarImpl.mapName());
    staticMap->set_maphash(BroodwarImpl.mapHash());

    for (TilePosition t : BroodwarImpl.getStartLocations())
    {
      auto startLocation = staticMap->add_startpositions();
      startLocation->set_x(t.x);
      startLocation->set_y(t.y);
      startLocation->set_scale(1);
    }

    protoClient.queueMessage(std::move(staticMapMessage));

    //static player data
    auto playersMessage = std::make_unique<bwapi::message::Message>();
    auto playersFrameUpdate = playersMessage->mutable_frameupdate();
    auto playersGame = playersFrameUpdate->mutable_game();
    for (Player i : BroodwarImpl.getPlayers())
    {
      int id = getPlayerID(i);
      PlayerData* p = &(data->players[id]);
      PlayerData* p2 = i->self;
      auto player = playersGame->add_players();
      player->set_id(id);

      StrCopy(p->name, i->getName());
      player->add_name(p->name);

      p->race = i->getRace();
      player->add_race(i->getRace());
      p->type = i->getType();
      player->add_type(i->getType());
      p->force = getForceID(i->getForce());
      player->add_force(p->force);
      p->color = p2->color;
      player->add_color(p2->color);
      p->isParticipating = p2->isParticipating;
      player->add_isparticipating(p2->isParticipating);

      for (int j = 0; j < 12; ++j)
      {
        p->isAlly[j] = false;
        p->isEnemy[j] = false;
      }
      for (Player j : BroodwarImpl.getPlayers())
      {
        p->isAlly[getPlayerID(j)] = i->isAlly(j);
        auto isAlly = player->add_isally();
        isAlly->set_index(getPlayerID(j));
        isAlly->set_value(i->isAlly(j));
        p->isEnemy[getPlayerID(j)] = i->isEnemy(j);
        auto isEnemy = player->add_isenemy();
        isEnemy->set_index(getPlayerID(j));
        isEnemy->set_value(i->isEnemy(j));
      }

      p->isNeutral = i->isNeutral();
      player->add_isneutral(i->isNeutral());
      p->startLocationX = i->getStartLocation().x;
      player->add_startlocationx(p->startLocationX);
      p->startLocationY = i->getStartLocation().y;
      player->add_startlocationy(p->startLocationY);
    }
    protoClient.queueMessage(std::move(playersMessage));

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
    protoClient.checkForConnection(CLIENT_VERSION, "x", "x");
    if (!protoClient.isConnected())
      return;
    protoClient.stopListen();
  }
  void Server::initializeGameData()
  {
    //called once when Starcraft starts. Not at the start of every match.
    data->hasGUI = true;
    data->hasLatCom = true;
    *oldData = *data;
    clearAll();
  }
  void Server::updateGameData()
  {
    for (Unit u : BroodwarImpl.evadeUnits)
      data->units[getUnitID(u)] = static_cast<UnitImpl*>(u)->data;

    auto message = std::make_unique<bwapi::message::Message>();
    auto frameUpdate = message->mutable_frameupdate();
    auto game = frameUpdate->mutable_game();
    auto gameData = game->mutable_gamedata();
    auto mapData = gameData->mutable_map();

    // TODO: Investigate whether https://github.com/bwapi/bwapi/commit/82ce2e7e4b958052a4f72eb7965769dc25221e27 needs to be applied

    if (BroodwarImpl.isInGame())
    {
      //dynamic player data
      auto playersMessage = std::make_unique<bwapi::message::Message>();
      auto playersFrameUpdate = playersMessage->mutable_frameupdate();
      auto playersGame = playersFrameUpdate->mutable_game();
      for (Player i : BroodwarImpl.getPlayers())
      {
        int id = getPlayerID(i);
        if (id >= 12)
          continue;
        auto player = playersGame->add_players();
        PlayerData* p = &(data->players[id]);
        PlayerData* p2 = i->self;
        player->set_id(id);

        if (p->isVictorious != i->isVictorious())
        {
          p->isVictorious = i->isVictorious();
          player->add_isvictorious(i->isVictorious());
        }
        if (p->isDefeated != i->isDefeated())
        {
          p->isDefeated = i->isDefeated();
          player->add_isdefeated(i->isDefeated());
        }
        if (p->leftGame != i->leftGame())
        {
          p->leftGame = i->leftGame();
          player->add_leftgame(i->leftGame());
        }
        if (p->minerals != p2->minerals)
        {
          p->minerals = p2->minerals;
          player->add_minerals(p2->minerals);
        }
        if (p->gas != p2->gas)
        {
          p->gas = p2->gas;
          player->add_gas(p2->gas);
        }
        if (p->gatheredMinerals != p2->gatheredMinerals)
        {
          p->gatheredMinerals = p2->gatheredMinerals;
          player->add_gatheredminerals(p2->gatheredMinerals);
        }
        if (p->gatheredGas != p2->gatheredGas)
        {
          p->gatheredGas = p2->gatheredGas;
          player->add_gatheredgas(p2->gatheredGas);
        }
        if (p->repairedMinerals != p2->repairedMinerals)
        {
          p->repairedMinerals = p2->repairedMinerals;
          player->add_repairedminerals(p2->repairedMinerals);
        }
        if (p->repairedGas != p2->repairedGas)
        {
          p->repairedGas = p2->repairedGas;
          player->add_repairedgas(p2->repairedGas);
        }
        if (p->refundedMinerals != p2->refundedMinerals)
        {
          p->refundedMinerals = p2->refundedMinerals;
          player->add_refundedminerals(p2->refundedMinerals);
        }
        if (p->refundedGas != p2->refundedGas)
        {
          p->refundedGas = p2->refundedGas;
          player->add_refundedgas(p2->refundedGas);
        }
        for (int j = 0; j < 3; ++j)
        {
          if (p->supplyTotal[j] != p2->supplyTotal[j])
          {
            auto supplyTotal = player->add_supplytotal();
            p->supplyTotal[j] = p2->supplyTotal[j];
            supplyTotal->set_index(j);
            supplyTotal->set_value(p2->supplyTotal[j]);
          }
          if (p->supplyUsed[j] != p2->supplyUsed[j])
          {
            auto supplyUsed = player->add_supplyused();
            p->supplyUsed[j] = p2->supplyUsed[j];
            supplyUsed->set_index(j);
            supplyUsed->set_value(p2->supplyUsed[j]);
          }
        }
        for (int j = 0; j < UnitTypes::Enum::MAX; ++j)
        {
          if (p->allUnitCount[j] != p2->allUnitCount[j])
          {
            auto allUnitCount = player->add_allunitcount();
            p->allUnitCount[j] = p2->allUnitCount[j];
            allUnitCount->set_index(j);
            allUnitCount->set_value(p2->allUnitCount[j]);
          }
          if (p->visibleUnitCount[j] != p2->visibleUnitCount[j])
          {
            auto visibleUnitCount = player->add_visibleunitcount();
            p->visibleUnitCount[j] = p2->visibleUnitCount[j];
            visibleUnitCount->set_index(j);
            visibleUnitCount->set_value(p2->visibleUnitCount[j]);
          }
          if (p->completedUnitCount[j] != p2->completedUnitCount[j])
          {
            auto completedUnitCount = player->add_completedunitcount();
            p->completedUnitCount[j] = p2->completedUnitCount[j];
            completedUnitCount->set_index(j);
            completedUnitCount->set_value(p2->completedUnitCount[j]);
          }
          if (p->deadUnitCount[j] != p2->deadUnitCount[j])
          {
            auto deadUnitCount = player->add_deadunitcount();
            p->deadUnitCount[j] = p2->deadUnitCount[j];
            deadUnitCount->set_index(j);
            deadUnitCount->set_value(p2->completedUnitCount[j]);
          }
          if (p->killedUnitCount[j] != p2->killedUnitCount[j])
          {
            auto killedUnitCount = player->add_killedunitcount();
            p->killedUnitCount[j] = p2->killedUnitCount[j];
            killedUnitCount->set_index(j);
            killedUnitCount->set_value(p2->killedUnitCount[j]);
          }
          if (p->isUnitAvailable[j] != p2->isUnitAvailable[j])
          {
            auto isUnitAvailable = player->add_isunitavailable();
            p->isUnitAvailable[j] = p2->isUnitAvailable[j];
            isUnitAvailable->set_index(j);
            isUnitAvailable->set_value(p2->isUnitAvailable[j]);
          }
        }

        if (p->totalUnitScore != p2->totalUnitScore)
        {
          p->totalUnitScore = p2->totalUnitScore;
          player->add_totalunitscore(p2->totalUnitScore);
        }
        if (p->totalKillScore != p2->totalKillScore)
        {
          p->totalKillScore = p2->totalKillScore;
          player->add_totalkillscore(p2->totalKillScore);
        }
        if (p->totalBuildingScore != p2->totalBuildingScore)
        {
          p->totalBuildingScore = p2->totalBuildingScore;
          player->add_totalbuildingscore(p2->totalBuildingScore);
        }
        if (p->totalRazingScore != p2->totalRazingScore)
        {
          p->totalRazingScore = p2->totalRazingScore;
          player->add_totalrazingscore(p2->totalRazingScore);
        }
        if (p->customScore != p2->customScore)
        {
          p->customScore = p2->customScore;
          player->add_customscore(p2->customScore);
        }

        for (int j = 0; j < 63; ++j)
        {
          if (p->upgradeLevel[j] != p2->upgradeLevel[j])
          {
            auto upgradeLevel = player->add_upgradelevel();
            p->upgradeLevel[j] = p2->upgradeLevel[j];
            upgradeLevel->set_index(j);
            upgradeLevel->set_value(p2->upgradeLevel[j]);
          }
          if (p->isUpgrading[j] != p2->isUpgrading[j])
          {
            auto isUpgrading = player->add_isupgrading();
            p->isUpgrading[j] = p2->isUpgrading[j];
            isUpgrading->set_index(j);
            isUpgrading->set_value(p2->isUpgrading[j]);
          }
          if (p->maxUpgradeLevel[j] != p2->maxUpgradeLevel[j])
          {
            auto maxUpgradeLevel = player->add_maxupgradelevel();
            p->maxUpgradeLevel[j] = p2->maxUpgradeLevel[j];
            maxUpgradeLevel->set_index(j);
            maxUpgradeLevel->set_value(p2->maxUpgradeLevel[j]);
          }
        }

        for (int j = 0; j < 47; ++j)
        {
          if (p->hasResearched[j] != p2->hasResearched[j])
          {
            auto hasResearched = player->add_hasresearched();
            p->hasResearched[j] = p2->hasResearched[j];
            hasResearched->set_index(j);
            hasResearched->set_value(p2->hasResearched[j]);
          }
          if (p->isResearching[j] != p2->isResearching[j])
          {
            auto isResearching = player->add_isresearching();
            p->isResearching[j] = p2->isResearching[j];
            isResearching->set_index(j);
            isResearching->set_value(p2->isResearching[j]);
          }
          if (p->isResearchAvailable[j] != p2->isResearchAvailable[j])
          {
            auto isResearchAvailable = player->add_isresearchavailable();
            p->isResearchAvailable[j] = p2->isResearchAvailable[j];
            isResearchAvailable->set_index(j);
            isResearchAvailable->set_value(p2->isResearchAvailable[j]);
          }
        }
        // I am testing thse in the for loops.
        /*
        memcpy(p->isResearchAvailable, p2->isResearchAvailable, sizeof(p->isResearchAvailable));
        memcpy(p->isUnitAvailable, p2->isUnitAvailable, sizeof(p->isUnitAvailable));
        memcpy(p->maxUpgradeLevel, p2->maxUpgradeLevel, sizeof(p->maxUpgradeLevel));
        */
      }
      protoClient.queueMessage(std::move(playersMessage));

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
      }

      //dynamic nuke dot data
      for (Position const &nd : BroodwarImpl.getNukeDots())
      {
        auto newDot = gameData->add_nukedots();
        newDot->set_x(nd.x);
        newDot->set_y(nd.y);
      }
    }

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
      unit->set_isbeinghealed(u.isBeingHealed);
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
      for (int i = 0; i < u.trainingQueueCount; i++)
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
    }
    //if (oldData->gameType != data->gameType)
    gameData->set_gametype(BroodwarImpl.getGameType());
    gameData->set_framecount(BroodwarImpl.getFrameCount());
    //if (oldData->latencyFrames != data->latencyFrames)
    gameData->set_latencyframes(BroodwarImpl.getLatencyFrames());
    //turnsize
    //gamespeed
    //frameskip
    //if (oldData->remainingLatencyFrames != data->remainingLatencyFrames)
    gameData->set_remaininglatencyframes(BroodwarImpl.getRemainingLatencyFrames());
    gameData->set_lasteventtime(BroodwarImpl.getLastEventTime());
    //replayvisionplayers
    //if (oldData->latencyTime != data->latencyTime)
    gameData->set_latencytime(BroodwarImpl.getLatencyTime());
    //if (oldData->remainingLatencyTime != data->remainingLatencyTime)
    gameData->set_remaininglatencytime(BroodwarImpl.getRemainingLatencyTime());
    //if (oldData->elapsedTime != data->elapsedTime)
    gameData->set_elapsedtime(BroodwarImpl.elapsedTime());
    //millisecondsperframe
    gameData->set_averagefps(static_cast<float>(BroodwarImpl.getAverageFPS()));
    gameData->set_countdowntimer(BroodwarImpl.countdownTimer());
    gameData->set_ispaused(BroodwarImpl.isPaused());
    gameData->set_isingame(BroodwarImpl.isInGame());
    gameData->set_ismultiplayer(BroodwarImpl.isMultiplayer());
    gameData->set_isbattlenet(BroodwarImpl.isBattleNet());
    gameData->set_isreplay(BroodwarImpl.isReplay());
    //clientunitselection
    gameData->set_hasgui(data->hasGUI);
    //gamename
    std::stringstream randomSeed;
    randomSeed << BroodwarImpl.getRandomSeed();
    gameData->set_randomseed(randomSeed.str());
    BroodwarImpl.events.clear();
    if (BroodwarImpl.isInGame())
    {

      //dynamic map data
      Map::copyToSharedMemory(mapData);
      //(no dynamic force data)

      if (BroodwarImpl.self())
        gameData->set_player(BroodwarImpl.self()->getID());

      //screensize
      auto screenPosition = gameData->mutable_screenposition();
      screenPosition->set_x(BroodwarImpl.getScreenPosition().x);
      screenPosition->set_y(BroodwarImpl.getScreenPosition().y);



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
        fillBulletData(BroodwarImpl.getBulletFromIndex(i)->data, bulletData, i);
      }
      protoClient.queueMessage(std::move(bulletsMessage));
    }
    protoClient.queueMessage(std::move(message));
    if (!BroodwarImpl.isInGame())
    {
      clearAll();
    }
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
            BroodwarImpl.sendTextEx(command.sendtext().toallies(), "%s", command.sendtext().text().c_str());
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
        else if (command.has_unitcommand() && BroodwarImpl.isInGame())
        {
          Unit target = nullptr;
          if (command.unitcommand().targetid() >= 0 && command.unitcommand().targetid() < (int)unitVector.size())
            target = unitVector[command.unitcommand().targetid()];
          for (auto unitID : command.unitcommand().unitid())
          {
            auto unit = unitVector[unitID];
            unit->issueCommand(UnitCommand(unit,
              UnitCommandType(command.unitcommand().unitcommandtype()),
              target, command.unitcommand().x(),
              command.unitcommand().y(),
              command.unitcommand().extra()));
          }
        }
        else if (command.has_shape() && BroodwarImpl.isInGame())
        {
          auto shapeCmd = command.shape();
          BWAPIC4::Shape shape{ static_cast<BWAPI::ShapeType::Enum>(shapeCmd.type()), static_cast<BWAPI::CoordinateType::Enum>(shapeCmd.ctype()), shapeCmd.x1(), shapeCmd.y1(), shapeCmd.x2(), shapeCmd.y2(), shapeCmd.extra1(), shapeCmd.extra2(), shapeCmd.color(), shapeCmd.issolid(), shapeCmd.text() };
          BroodwarImpl.addShape(shape);
        }
        else if (command.has_createunit() && BroodwarImpl.isInGame())
        {
          auto cmd = command.createunit();
          for (int i = 0; i < cmd.count(); ++i)
          {
            BroodwarImpl.createUnit(UnitType(cmd.unittype()), getPlayer(cmd.playerid()), cmd.x(), cmd.y());
          }
        }
        else if (command.has_killunits() && BroodwarImpl.isInGame())
        {
          auto cmd = command.killunits();
          for (int i = 0; i < cmd.unitid_size(); ++i)
          {
            if (cmd.removeinstantly())
            {
              BroodwarImpl.removeUnit(getUnit(cmd.unitid(i)));
            }
            else
            {
              BroodwarImpl.killUnit(getUnit(cmd.unitid(i)));
            }
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