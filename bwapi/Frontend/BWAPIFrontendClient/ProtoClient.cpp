#include "ProtoClient.h"
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

#undef max

namespace BWAPI
{
  //Client BWAPIClient;
  ProtoClient::ProtoClient()
  {}
  ProtoClient::~ProtoClient()
  {
    this->disconnect();
  }
  bool ProtoClient::isConnected() const
  {
    return protoClient.isConnected();
  }
  bool ProtoClient::connect()
  {
    if (protoClient.isConnected())
    {
      std::cout << "Already connected." << std::endl;
      return true;
    }

    while (true)
    {
      std::cout << "Connecting..." << std::endl;
      protoClient.lookForServer(0, "x", false);
      if (protoClient.isConnected())
        return true;
      {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
      }
    }
    return protoClient.isConnected();
  }
  void ProtoClient::disconnect()
  {
    if (!connected)
      return;
    protoClient.disconnect();

  }
  void ProtoClient::transmitMessages(Game& game)
  {
    game.flushCommandOptimizer();
    game.flush();

    protoClient.transmitMessages();
  }
  void ProtoClient::update(Game& game)
  {
    game.clearEvents();

    observationRequest();

    transmitMessages(game);

    protoClient.receiveMessages();
    //Connected to a 1.16.1 backend
    if (!protoClient.isRemaster())
    {
      while (protoClient.messageQueueSize())
      {
        auto message = protoClient.getNextMessage();
        if (message->has_frameupdate())
        {
          //update game here
          if (message->frameupdate().has_game())
          {
            auto gameMessage = message->frameupdate().game();
            if (gameMessage.has_gamedata())
            {
              auto gameUpdate = gameMessage.gamedata();
              game.gameData->player = PlayerID{ gameUpdate.player() };
              game.gameData->screenPosition = { gameUpdate.screenposition().x(), gameUpdate.screenposition().y() };
              game.gameData->frameCount = gameUpdate.framecount();
              game.gameData->latencyFrames = gameUpdate.latencyframes();
              game.gameData->remainingLatencyFrames = gameUpdate.remaininglatencyframes();
              game.gameData->lastEventTime = gameUpdate.lasteventtime();
              game.gameData->latencyTime = gameUpdate.latencytime();
              game.gameData->remainingLatencyTime = gameUpdate.remaininglatencytime();
              game.gameData->elapsedTime = gameUpdate.elapsedtime();
              game.gameData->averageFPS = gameUpdate.averagefps();
              game.gameData->countdownTimer = gameUpdate.countdowntimer();
              game.gameData->isPaused = gameUpdate.ispaused();
              game.gameData->isInGame = gameUpdate.isingame();
              game.gameData->isMultiplayer = gameUpdate.ismultiplayer();
              game.gameData->isBattleNet = gameUpdate.isbattlenet();
              game.gameData->isReplay = gameUpdate.isreplay();
              game.gameData->hasGUI = gameUpdate.hasgui();
              game.gameData->randomSeed = gameUpdate.randomseed();
              game.gameData->nukeDots.clear();
              for (auto &nd : gameUpdate.nukedots())
                game.gameData->nukeDots.emplace_back(Position{ nd.x(), nd.y() });
              if (gameUpdate.has_staticmap())
              {
                auto staticMap = gameUpdate.staticmap();
                game.gameData->map.size = TilePosition{ staticMap.size().x(), staticMap.size().y() };
                for (auto isWalkable : staticMap.iswalkable())
                  game.gameData->map.isWalkable[isWalkable.x()][isWalkable.y()] = isWalkable.value();

                auto groundHeight = staticMap.groundheight().begin();
                auto isBuildable = staticMap.isbuildable().begin();
                auto mapTile = staticMap.maptileregionid().begin();
                while (groundHeight != staticMap.groundheight().end())
                {
                  game.gameData->map.groundHeight[groundHeight->x()][groundHeight->y()] = groundHeight->value();
                  game.gameData->map.isBuildable[isBuildable->x()][isBuildable->y()] = isBuildable->value();
                  game.gameData->map.mapTileRegionId[mapTile->x()][mapTile->y()] = mapTile->value();
                  groundHeight++;
                  isBuildable++;
                  mapTile++;
                }
                game.gameData->map.mapHash = staticMap.maphash();

                // TODO: These uint32 are being implicitly cast to unsigned short, they need a proper explicit cast/conversion
                std::copy(staticMap.mapsplittilesminitilemask().begin(), staticMap.mapsplittilesminitilemask().end(), &game.gameData->map.mapSplitTilesMiniTileMask[0]);
                std::copy(staticMap.mapsplittilesregion1().begin(), staticMap.mapsplittilesregion1().end(), &game.gameData->map.mapSplitTilesRegion1[0]);
                std::copy(staticMap.mapsplittilesregion2().begin(), staticMap.mapsplittilesregion2().end(), &game.gameData->map.mapSplitTilesRegion2[0]);

                game.gameData->map.tileset = staticMap.tileset();
                game.gameData->mapName = staticMap.mapname();
                game.gameData->mapFileName = staticMap.mapfilename();
                game.gameData->mapPath = staticMap.mappath();

                for (auto &sp : staticMap.startpositions())
                  game.gameData->startPositions.push_back(TilePosition{ sp.x(), sp.y() });
              }
              if (gameUpdate.has_map())
              {
                auto map = gameUpdate.map();
                for (auto& data : map.hascreep())
                  game.gameData->map.hasCreep[data.x()][data.y()] = data.value();
                for (auto& data : map.isexplored())
                  game.gameData->map.isExplored[data.x()][data.y()] = data.value();
                for (auto& data : map.isoccupied())
                  game.gameData->map.isOccupied[data.x()][data.y()] = data.value();
                for (auto& data : map.isvisible())
                  game.gameData->map.isVisible[data.x()][data.y()] = data.value();
              }
            }
            for (auto &p : gameMessage.players())
            {
              auto fillPlayerData = [](PlayerData& playerData, const bwapi::data::Player& p) {
                auto &allUnitCount = p.allunitcount();
                auto &completedUnitCount = p.completedunitcount();
                auto &deadUnitCount = p.deadunitcount();
                auto &isUnitAvailable = p.isunitavailable();
                auto &killedUnitCount = p.killedunitcount();
                auto &visibleUnitCount = p.visibleunitcount();
                for (auto &j : allUnitCount)
                  playerData.allUnitCount[j.index()] = j.value();
                for (auto &j : completedUnitCount)
                  playerData.completedUnitCount[j.index()] = j.value();
                for (auto &j : deadUnitCount)
                  playerData.deadUnitCount[j.index()] = j.value();
                for (auto &j : isUnitAvailable)
                  playerData.isUnitAvailable[j.index()] = j.value();
                for (auto &j : killedUnitCount)
                  playerData.killedUnitCount[j.index()] = j.value();
                for (auto &j : visibleUnitCount)
                  playerData.visibleUnitCount[j.index()] = j.value();
                for (auto &j : p.color())
                  playerData.color = Color{ j };
                for (auto &j : p.customscore())
                  playerData.customScore = j;
                for (auto &j : p.force())
                  playerData.force = ForceID{ j };
                for (auto &j : p.gas())
                  playerData.gas = j;
                for (auto &j : p.gatheredgas())
                  playerData.gatheredGas = j;
                for (auto &j : p.gatheredminerals())
                  playerData.gatheredMinerals = j;
                auto &hasResearched = p.hasresearched();
                auto &isResearchAvailable = p.isresearchavailable();
                auto &isResearching = p.isresearching();
                for (auto &j : hasResearched)
                  playerData.hasResearched[j.index()] = j.value();
                for (auto &j : isResearchAvailable)
                  playerData.isResearchAvailable[j.index()] = j.value();
                for (auto &j : isResearching)
                  playerData.isResearching[j.index()] = j.value();
                auto &isAlly = p.isally();
                auto &isEnemy = p.isenemy();
                for (auto &j : isAlly)
                  playerData.isAlly[j.index()] = j.value();
                for (auto &j : isEnemy)
                  playerData.isEnemy[j.index()] = j.value();
                for (auto &j : p.isdefeated())
                  playerData.isDefeated = j;
                for (auto &j : p.isneutral())
                  playerData.isNeutral = j;
                for (auto &j : p.isparticipating())
                  playerData.isParticipating = j;
                auto &isUpgrading = p.isupgrading();
                auto &maxUpgradeLevel = p.maxupgradelevel();
                auto &upgradeLevel = p.upgradelevel();
                for (auto &j : isUpgrading)
                  playerData.isUpgrading[j.index()] = j.value();
                for (auto &j : maxUpgradeLevel)
                  playerData.maxUpgradeLevel[j.index()] = j.value();
                for (auto &j : upgradeLevel)
                  playerData.upgradeLevel[j.index()] = j.value();
                for (auto &j : p.isvictorious())
                  playerData.isVictorious = j;
                for (auto &j : p.leftgame())
                  playerData.leftGame = j;
                for (auto &j : p.minerals())
                  playerData.minerals = j;
                for (auto &j : p.name())
                  playerData.name = j;
                for (auto &j : p.race())
                  playerData.race = Race{ j };
                for (auto &j : p.refundedgas())
                  playerData.refundedGas = j;
                for (auto &j : p.refundedminerals())
                  playerData.refundedMinerals = j;
                for (auto &j : p.repairedgas())
                  playerData.repairedGas = j;
                for (auto &j : p.repairedminerals())
                  playerData.repairedMinerals = j;
                for (auto &j : p.startlocationx())
                  playerData.startLocationX = j;
                for (auto &j : p.startlocationy())
                  playerData.startLocationY = j;
                auto &supplyTotal = p.supplytotal();
                auto &supplyUsed = p.supplyused();
                for (auto &j : supplyTotal)
                  playerData.supplyTotal[j.index()] = j.value();
                for (auto &j : supplyUsed)
                  playerData.supplyUsed[j.index()] = j.value();
                for (auto &j : p.totalbuildingscore())
                  playerData.totalBuildingScore = j;
                for (auto &j : p.totalkillscore())
                  playerData.totalKillScore = j;
                for (auto &j : p.totalrazingscore())
                  playerData.totalRazingScore = j;
                for (auto &j : p.totalunitscore())
                  playerData.totalUnitScore = j;
                for (auto &j : p.type())
                  playerData.type = PlayerType{ j };
              };
              auto playerID = PlayerID{ p.id() };
              auto itr = players.find(playerID);
              if (itr == players.end())
              {
                auto &playerData = *players.emplace(game, playerID, ForceID{ -1 }).first;
                fillPlayerData(const_cast<PlayerData&>(playerData), p);
                game.updatePlayer(playerData);
              }
              else
              {
                fillPlayerData(const_cast<PlayerData &>(*itr), p);
              }
            }
            for (auto &u : gameMessage.units())
            {
              auto fillUnitData = [](UnitData& unitData, const bwapi::data::Unit& u) {
                unitData.acidSporeCount = u.acidsporecount();
                unitData.addon = UnitID{ u.addon() };
                unitData.airWeaponCooldown = u.airweaponcooldown();
                unitData.angle = u.angle();
                unitData.buildType = UnitType{ u.buildtype() };
                unitData.buildUnit = UnitID{ u.buildunit() };
                unitData.buttonset = u.buttonset();
                unitData.carrier = UnitID{ u.carrier() };
                unitData.carryResourceType = u.carryresourcetype();
                unitData.defenseMatrixPoints = u.defensematrixpoints();
                unitData.defenseMatrixTimer = u.defensematrixtimer();
                unitData.energy = u.energy();
                unitData.ensnareTimer = u.ensnaretimer();
                unitData.exists = u.exists();
                unitData.groundWeaponCooldown = u.groundweaponcooldown();
                unitData.hasNuke = u.hasnuke();
                unitData.hatchery = UnitID{ u.hatchery() };
                unitData.hitPoints = u.hitpoints();
                unitData.interceptorCount = u.interceptorcount();
                unitData.irradiateTimer = u.irradiatetimer();
                unitData.isAccelerating = u.isaccelerating();
                unitData.isAttackFrame = u.isattackframe();
                unitData.isAttacking = u.isattacking();
                unitData.isBeingGathered = u.isbeinggathered();
                unitData.isBeingHealed = u.isbeinghealed();
                unitData.isBlind = u.isblind();
                unitData.isBraking = u.isbraking();
                unitData.isBurrowed = u.isburrowed();
                unitData.isCloaked = u.iscloaked();
                unitData.isCompleted = u.iscompleted();
                unitData.isConstructing = u.isconstructing();
                unitData.isDetected = u.isdetected();
                unitData.isGathering = u.isgathering();
                unitData.isHallucination = u.ishallucination();
                unitData.isIdle = u.isidle();
                unitData.isInterruptible = u.isinterruptible();
                unitData.isInvincible = u.isinvincible();
                unitData.isLifted = u.islifted();
                unitData.isMorphing = u.ismorphing();
                unitData.isMoving = u.ismoving();
                unitData.isParasited = u.isparasited();
                unitData.isPowered = u.ispowered();
                unitData.isSelected = u.isselected();
                unitData.isStartingAttack = u.isstartingattack();
                unitData.isStuck = u.isstuck();
                unitData.isTraining = u.istraining();
                unitData.isUnderDarkSwarm = u.isunderdarkswarm();
                unitData.isUnderDWeb = u.isunderdweb();
                unitData.isUnderStorm = u.isunderstorm();
                std::copy(u.isvisible().begin(), u.isvisible().end(), unitData.isVisible);
                unitData.killCount = u.killcount();
                for (auto &l : u.larva())
                  unitData.larva.push_back(UnitID{ l });
                unitData.lastAttackerPlayer = PlayerID{ u.lastattackerplayer() };
                //unitData.lastCommand = UnitCommand{ u.lastcommand() };
                unitData.lastCommandFrame = u.lastcommandframe();
                unitData.lastHitPoints = u.lasthitpoints();
                unitData.loadedUnits.clear();
                for (auto &lu : u.loadedunits())
                  unitData.loadedUnits.push_back(UnitID{ lu });
                unitData.lockdownTimer = u.lockdowntimer();
                unitData.maelstromTimer = u.maelstromtimer();
                unitData.nydusExit = UnitID{ u.nydusexit() };
                unitData.order = Order{ u.order() };
                unitData.orderTarget = UnitID{ u.ordertarget() };
                unitData.orderTargetPosition = Position{ u.ordertargetposition().x(), u.ordertargetposition().y() };
                unitData.orderTimer = u.ordertimer();
                unitData.plagueTimer = u.plaguetimer();
                unitData.player = PlayerID{ u.player() };
                unitData.position = Position{ u.position().x(), u.position().y() };
                unitData.powerUp = UnitID{ u.powerup() };
                unitData.rallyPosition = Position{ u.rallyposition().x(), u.rallyposition().y() };
                unitData.rallyUnit = UnitID{ u.rallyunit() };
                unitData.recentlyAttacked = u.recentlyattacked();
                unitData.remainingBuildTime = u.remainingbuildtime();
                unitData.remainingResearchTime = u.remainingresearchtime();
                unitData.remainingTrainTime = u.remainingtraintime();
                unitData.remainingUpgradeTime = u.remainingupgradetime();
                unitData.removeTimer = u.removetimer();
                unitData.replayID = UnitID{ u.replayid() };
                unitData.resourceGroup = u.resourcegroup();
                unitData.resources = u.resources();
                unitData.scarabCount = u.scarabcount();
                unitData.secondaryOrder = Order{ u.secondaryorder() };
                unitData.shields = u.shields();
                unitData.spellCooldown = u.spellcooldown();
                unitData.spiderMineCount = u.spiderminecount();
                unitData.stasisTimer = u.stasistimer();
                unitData.stimTimer = u.stimtimer();
                unitData.target = UnitID{ u.target() };
                unitData.targetPosition = Position{ u.targetposition().x(), u.targetposition().y() };
                unitData.tech = TechType{ u.tech() };
                unitData.trainingQueue.clear();
                for (auto &tqu : u.trainingqueue())
                  unitData.trainingQueue.push_back(UnitType{ tqu });
                unitData.transport = UnitID{ u.transport() };
                unitData.type = UnitType{ u.type() };
                unitData.upgrade = UpgradeType{ u.upgrade() };
                unitData.velocityX = u.velocityx();
                unitData.velocityY = u.velocityy();
              };
              UnitID unitID = UnitID{ u.id() };
              auto itr = units.find(unitID);
              if (itr == units.end())
              {
                auto &newUnit = *units.emplace(game, unitID).first;
                fillUnitData(const_cast<UnitData &>(newUnit), u);
                game.addUnit(newUnit);
              }
              else
              {
                fillUnitData(const_cast<UnitData &>(*itr), u);
              }
            }
            for (auto &f : gameMessage.forces())
            {
              auto fillForceData = [](ForceData &forceData, const bwapi::data::Force f)
              {
                forceData.name = f.name();
                for (auto p : f.players())
                  forceData.players.push_back(PlayerID{ p });
              };
              auto forceID = ForceID{ f.id() };
              auto itr = forces.find(forceID);
              if (itr == forces.end())
              {
                auto &newForce = *forces.emplace(game, forceID).first;
                fillForceData(const_cast<ForceData &>(newForce), f);
                game.addForce(newForce);
              }
              else
                fillForceData(const_cast<ForceData &>(*itr), f);
            }
            for (auto &r : gameMessage.regions())
            {
              auto fillRegionData = [](RegionData &regionData, const bwapi::data::Region r)
              {
                regionData.bottomMost = r.bottommost();
                regionData.center_x = r.center_x();
                regionData.center_y = r.center_y();
                regionData.isAccessible = r.isaccessible();
                regionData.isHigherGround = r.ishigherground();
                regionData.islandID = r.islandid();
                regionData.leftMost = r.leftmost();
                regionData.neighborCount = r.neighborcount();
                std::copy(r.neighbors().begin(), r.neighbors().end(), regionData.neighbors);
                regionData.priority = r.priority();
                regionData.rightMost = r.rightmost();
                regionData.topMost = r.topmost();
              };
              auto regionID = RegionID{ r.id() };
              auto itr = regions.find(regionID);
              if (itr == regions.end())
              {
                auto &newRegion = *regions.emplace(game, regionID).first;
                fillRegionData(const_cast<RegionData &>(newRegion), r);
                game.addRegion(newRegion);
              }
              else
                fillRegionData(const_cast<RegionData &>(*itr), r);
            }
            for (auto &b : gameMessage.bullets())
            {
              auto fillBulletData = [](BulletData &bulletData, const bwapi::data::Bullet b)
              {
                bulletData.angle = b.angle();
                bulletData.exists = b.exists();
                std::copy(b.isvisible().begin(), b.isvisible().end(), bulletData.isVisible);
                bulletData.player = PlayerID{ b.player() };
                bulletData.position = Position{ b.position().x(), b.position().y() };
                bulletData.removeTimer = b.removetimer();
                bulletData.source = UnitID{ b.source() };
                bulletData.target = UnitID{ b.target() };
                bulletData.targetPosition = Position{ b.targetposition().x(), b.targetposition().y() };
                bulletData.type = BulletType{ b.type() };
                bulletData.velocityX = b.velocityx();
                bulletData.velocityY = b.velocityy();
              };
              auto bulletID = BulletID{ b.id() };
              auto itr = bullets.find(bulletID);
              if (itr == bullets.end())
              {
                auto &newBullet = *bullets.emplace(game, bulletID).first;
                fillBulletData(const_cast<BulletData &>(newBullet), b);
                game.addBullet(newBullet);
              }
              else
                fillBulletData(const_cast<BulletData &>(*itr), b);
            }
          }
        }
        else if (message->has_event())
        {
          Event e2;
          auto e = message->event();
          if (e.has_matchstart())
          {
            e2.setType(EventType::MatchStart);
          }
          else if (e.has_matchframe())
          {
            e2.setType(EventType::MatchFrame);
          }
          else if (e.has_matchend())
          {
            e2.setType(EventType::MatchEnd);
            e2.setWinner(e.matchend().winner());
          }
          else if (e.has_menuframe())
          {
            e2.setType(EventType::MenuFrame);
          }
          else if (e.has_sendtext())
          {
            e2.setType(EventType::SendText);
            e2.setText(e.sendtext().text());
          }
          else if (e.has_receivetext())
          {
            e2.setType(EventType::ReceiveText);
            e2.setText(e.receivetext().text());
            e2.setPlayer(game.getPlayer(PlayerID{ e.receivetext().player() }));
          }
          else if (e.has_savegame())
          {
            e2.setType(EventType::SaveGame);
            e2.setText(e.savegame().text());
          }
          else if (e.has_playerleft())
          {
            e2.setType(EventType::PlayerLeft);
            e2.setPlayer(game.getPlayer(PlayerID{ e.playerleft().player() }));
          }
          else if (e.has_nukedetect())
          {
            auto target = e.nukedetect().target();
            e2.setType(EventType::NukeDetect);
            e2.setPosition(Position{ target.x(),target.y() });
          }
          else if (e.has_unitdiscover())
          {
            e2.setType(EventType::UnitDiscover);
            e2.setUnit(game.getUnit(UnitID{ e.unitdiscover().unit() }));
          }
          else if (e.has_unitevade())
          {
            e2.setType(EventType::UnitEvade);
            e2.setUnit(game.getUnit(UnitID{ e.unitevade().unit() }));
          }
          else if (e.has_unitshow())
          {
            e2.setType(EventType::UnitShow);
            e2.setUnit(game.getUnit(UnitID{ e.unitshow().unit() }));
          }
          else if (e.has_unithide())
          {
            e2.setType(EventType::UnitHide);
            e2.setUnit(game.getUnit(UnitID{ e.unithide().unit() }));
          }
          else if (e.has_unitcreate())
          {
            e2.setType(EventType::UnitCreate);
            e2.setUnit(game.getUnit(UnitID{ e.unitcreate().unit() }));
          }
          else if (e.has_unitdestroy())
          {
            e2.setType(EventType::UnitDestroy);
            e2.setUnit(game.getUnit(UnitID{ e.unitdestroy().unit() }));
          }
          else if (e.has_unitmorph())
          {
            e2.setType(EventType::UnitMorph);
            e2.setUnit(game.getUnit(UnitID{ e.unitmorph().unit() }));
          }
          else if (e.has_unitrenegade())
          {
            e2.setType(EventType::UnitRenegade);
            e2.setUnit(game.getUnit(UnitID{ e.unitrenegade().unit() }));
          }
          else if (e.has_savegame())
          {
            e2.setType(EventType::SaveGame);
            e2.setText(e.savegame().text());
          }
          else if (e.has_unitcomplete())
          {
            e2.setType(EventType::UnitComplete);
            e2.setUnit(game.getUnit(UnitID{ e.unitcomplete().unit() }));
          }
          game.addEvent(e2);
        }
        else if (message->has_endofqueue())
          break;
      }
    }
    else //Connected to a Remaster backend
    {
      while (protoClient.responseQueueSize())
      {
        auto response = protoClient.getNextResponse();
        //logic to process each message
        if (response->has_action())
          ;// I don't think we need this, BWAPI generally
           // doesn't provide feedback from backend (for now).
        if (response->has_available_maps())
          ;// Will come back to this. Unsure if used for BWAPI.
        if (response->has_create_game())
          ;// Looks like this provides error information if create
           // game request fails. Nowhere to put this right now.
        if (response->has_data())
        {
          auto data = &response->data();
          for (auto &ability : data->abilities())
            ; // Nothing directly named abilities in BWAPI.
        }
        if (response->has_game_info())
        {
          game.gameData->isInGame = true;
          auto gameInfo = &response->game_info();
          if (gameInfo->has_local_map_path())
            game.gameData->mapPath = gameInfo->local_map_path();
          if (gameInfo->has_map_name())
            game.gameData->mapName = gameInfo->map_name();
          if (gameInfo->has_options())
            ;// what are these?
          if (gameInfo->has_start_raw())
          {
            auto startRaw = &gameInfo->start_raw();
            if (startRaw->has_map_size())
              game.gameData->map.size = TilePosition{ startRaw->map_size().x(), startRaw->map_size().y() };
            if (startRaw->has_pathing_grid())
              ;//how to translate this?
            if (startRaw->has_placement_grid())
              ;//how to translate this?
            if (startRaw->has_playable_area())
              ;//how to translate this?
            if (startRaw->has_terrain_height())
              ;//how to translate this?
          }
          auto playerInfo = &gameInfo->player_info();
          auto fillPlayerData = [](PlayerData& playerData, const SCRAPIProtocol::PlayerInfo& p) {
            playerData.race = static_cast<Race>(p.race_actual());
            playerData.type = static_cast<PlayerType>(p.type());
          };
          for (auto &currentPlayer : *playerInfo)
          {
            auto playerID = PlayerID{ static_cast<int>(currentPlayer.player_id()) };
            auto itr = players.find(playerID);
            if (itr == players.end())
            {
              auto &playerData = *players.emplace(game, playerID, ForceID{ -1 }).first;
              fillPlayerData(const_cast<PlayerData&>(playerData), currentPlayer);
              game.updatePlayer(playerData);
            }
            else
            {
              fillPlayerData(const_cast<PlayerData &>(*itr), currentPlayer);
            }
            if (currentPlayer.type() == SCRAPIProtocol::Participant)
              game.gameData->player = playerID;
          }
          auto playerID = PlayerID{ 11 };
          if (players.find(playerID) == players.end())
          {
            auto &playerData = *players.emplace(game, playerID, ForceID{ -1 }).first;
            const_cast<PlayerData&>(playerData).isNeutral = true;
            game.updatePlayer(playerData);
          }
        }
        if (response->has_observation())
        {
          auto responseObservation = &response->observation();          
          auto observation = &responseObservation->observation();
          auto observationRaw = observation->raw_data();
          auto player_common = observation->player_common();
          auto fillPlayerData = [](PlayerData& playerData, const SCRAPIProtocol::PlayerCommon& p, const SCRAPIProtocol::ObservationRaw& o) {
            playerData.minerals = p.minerals();
            playerData.gas = p.vespene();
            playerData.supplyTotal[playerData.race] = p.food_cap();
            playerData.supplyUsed[playerData.race] = p.food_used();
            
            
          };
          auto itr = players.find(game.gameData->player);
          fillPlayerData(const_cast<PlayerData &>(*itr), player_common, observationRaw);
          auto mapState = observationRaw.map_state();
          auto creep = mapState.creep();
          auto visibility = mapState.visibility();
          game.gameData->map.size = TilePosition{ creep.size().x(), creep.size().y() };
          for (int x = 0; x < creep.size().x(); x++)
          {
            for (int y = 0; y < creep.size().y(); y++)
            {
              game.gameData->map.hasCreep[x][y] = creep.data()[x + y * creep.size().x()] - '0';
              game.gameData->map.isVisible[x][y] = visibility.data()[x + y * creep.size().x()] - '0' == 1;
              game.gameData->map.isExplored[x][y] = (visibility.data()[x + y * creep.size().x()] - '0' == 1) || (visibility.data()[x + y * creep.size().x()] - '0' == 0);
            }
          }
          auto SCRAPIunits = observationRaw.units();
          auto fillUnitData = [&](UnitData& unitData, const SCRAPIProtocol::Unit& u) {
            // radius?
            // is_flying?
            // buff_ids?
            // assigned_harvesters
            // ideal_harvesters?
            // engaged_target_tag

            //unitData.acidSporeCount = u.acidsporecount(); // Where is this?
            unitData.type = static_cast<UnitType>(u.unit_type());
            if (u.has_add_on_tag())
              unitData.addon = static_cast<UnitID>(u.add_on_tag());
            unitData.airWeaponCooldown = u.weapon_cooldown();
            unitData.angle = static_cast<double>(u.facing());
            //unitData.buildUnit = UnitID{ u.buildunit() }; // What is this training, or what is building me?
            if (u.orders().size())
            {
              switch (static_cast<Order>(u.orders().begin()->ability_id()))
              {
              case Orders::Enum::IncompleteBuilding:
              case Orders::Enum::IncompleteWarping:
                unitData.buildType = unitData.type;
                break;
              case Orders::Enum::ConstructingBuilding:
                if (unitData.buildUnit != static_cast<UnitID>(-1))
                  unitData.buildType = units.find(unitData.buildUnit) == units.end() ? units.find(unitData.buildUnit)->type : UnitTypes::None;
                break;
                /*case Orders::Enum::IncompleteMorphing:
                {
                  UnitType type = getBuildQueue[getBuildQueueSlot % 5];
                  unitData.buildType = type == UnitTypes::None ? unitData.type : type.getID();
                }
                break;*/
              case Orders::Enum::PlaceBuilding:
              case Orders::Enum::PlaceProtossBuilding:
              case Orders::Enum::ZergUnitMorph:
              case Orders::Enum::ZergBuildingMorph:
              case Orders::Enum::DroneLand:
                unitData.buildType = static_cast<UnitType>(u.add_on_tag());
                break;
                /*case Orders::Enum::ResearchTech:
                  unitData.tech = static_cast<TechType>(u.add_on_tag());
                  unitData.remainingResearchTime = o->building.upgradeResearchTime;
                  break;
                /*case Orders::Enum::Upgrade:
                  unitData.upgrade = static_cast<UpgradeType>(u.add_on_tag()>
                  unitData.remainingUpgradeTime = o->building.upgradeResearchTime;
                  break;*/
              }
            }
            //unitData.carrier = UnitID{ u.carrier() };
            //unitData.carryResourceType = u.carryresourcetype();
            //unitData.defenseMatrixPoints = u.defensematrixpoints();
            //unitData.defenseMatrixTimer = u.defensematrixtimer();
            unitData.energy = u.energy();
            //unitData.ensnareTimer = u.ensnaretimer();
            unitData.groundWeaponCooldown = u.weapon_cooldown();
            //unitData.hasNuke = u.hasnuke();
            //unitData.hatchery = UnitID{ u.hatchery() };
            unitData.hitPoints = u.health();
            //unitData.interceptorCount = u.interceptorcount();
            //unitData.irradiateTimer = u.irradiatetimer();
            //unitData.isAccelerating = u.isaccelerating();
            //unitData.isAttackFrame = u.isattackframe();
            //unitData.isAttacking = u.isattacking();
            //unitData.isBeingGathered = u.isbeinggathered();
            //unitData.isBeingHealed = u.isbeinghealed();
            //unitData.isBlind = u.isblind();
            //unitData.isBraking = u.isbraking();
            unitData.isBurrowed = u.is_burrowed();
            unitData.isCloaked = (u.cloak() == 1) || (u.cloak() == 2);
            unitData.isCompleted = u.build_progress() == 1;
            //unitData.isConstructing = u.isconstructing();
            unitData.isDetected = (u.cloak() == 2) || (u.cloak() == 3);
            //unitData.isGathering = u.isgathering();
            //unitData.isHallucination = u.ishallucination();
            //unitData.isIdle = u.isidle();
            unitData.isInterruptible = true;
            //unitData.isInvincible = u.isinvincible();
            //unitData.isLifted = u.islifted();
            //unitData.isMorphing = u.ismorphing();
            //unitData.isMoving = u.ismoving();
            //unitData.isParasited = u.isparasited();
            if (unitData.type.requiresPsi())
              unitData.isPowered = u.is_powered();
            else
              unitData.isPowered = true;
            unitData.isSelected = u.is_selected();
            //unitData.isStartingAttack = u.isstartingattack();
            //unitData.isStuck = u.isstuck();
            //unitData.isTraining = u.istraining();
            //unitData.isUnderDarkSwarm = u.isunderdarkswarm();
            //unitData.isUnderDWeb = u.isunderdweb();
            //unitData.isUnderStorm = u.isunderstorm();
            unitData.isVisible[game.gameData->player.id] = (u.display_type() == SCRAPIProtocol::Visible);
            //unitData.killCount = u.killcount();
            //for (auto &l : u.larva())
              //unitData.larva.push_back(UnitID{ l });
            //unitData.lastAttackerPlayer = PlayerID{ u.lastattackerplayer() };
            ////unitData.lastCommand = UnitCommand{ u.lastcommand() };
            //unitData.lastCommandFrame = u.lastcommandframe();
            //unitData.lastHitPoints = u.lasthitpoints();
            unitData.loadedUnits.clear();
            for (auto &lu : u.passengers())
              unitData.loadedUnits.push_back(static_cast<UnitID>(lu.tag()));
            //unitData.lockdownTimer = u.lockdowntimer();
            //unitData.maelstromTimer = u.maelstromtimer();
            //unitData.nydusExit = UnitID{ u.nydusexit() };
            auto itr = u.orders().begin();
            if (itr != u.orders().end())
            {
              unitData.order = static_cast<Order>(u.orders().begin()->ability_id());
              if (itr->has_target_unit_tag())
                unitData.orderTarget = static_cast<UnitID>(itr->target_unit_tag());
              else if (itr->has_target_world_space_pos())
                unitData.orderTargetPosition = Position{ static_cast<int>(itr->target_world_space_pos().x()), static_cast<int>(itr->target_world_space_pos().y()) };
            }
            //unitData.orderTimer = u.ordertimer();
            //unitData.plagueTimer = u.plaguetimer();
            unitData.player = static_cast<PlayerID>(u.owner());
            unitData.position = Position{ static_cast<int>(u.pos().x()), static_cast<int>(u.pos().y()) };
            //unitData.powerUp = UnitID{ u.powerup() };
            //unitData.rallyPosition = Position{ u.rallyposition().x(), u.rallyposition().y() };
            //unitData.rallyUnit = UnitID{ u.rallyunit() };
            //unitData.recentlyAttacked = u.recentlyattacked();
            //unitData.remainingBuildTime = u.remainingbuildtime();
            //unitData.remainingResearchTime = u.remainingresearchtime();
            //unitData.remainingTrainTime = u.remainingtraintime();
            //unitData.remainingUpgradeTime = u.remainingupgradetime();
            //unitData.removeTimer = u.removetimer();
            //unitData.replayID = UnitID{ u.replayid() };
            //unitData.resourceGroup = u.resourcegroup();
            auto unitType = static_cast<UnitType>(u.unit_type());
            if (unitType.isMineralField())
              unitData.resources = u.mineral_contents();
            else if (unitType.isResourceContainer())
              unitData.resources = u.vespene_contents();
            //unitData.scarabCount = u.scarabcount();
            //unitData.secondaryOrder = Order{ u.secondaryorder() };
            unitData.shields = u.shield();
            //unitData.spellCooldown = u.spellcooldown();
            //unitData.spiderMineCount = u.spiderminecount();
            //unitData.stasisTimer = u.stasistimer();
            //unitData.stimTimer = u.stimtimer();
            //unitData.target = UnitID{ u.target() };
            //unitData.targetPosition = Position{ u.targetposition().x(), u.targetposition().y() };
            //unitData.tech = TechType{ u.tech() };
            //unitData.trainingQueue.clear();
            //for (auto &tqu : u.trainingqueue())
              //unitData.trainingQueue.push_back(UnitType{ tqu });
            //unitData.transport = UnitID{ u.transport() };
            //unitData.upgrade = UpgradeType{ u.upgrade() };
            //unitData.velocityX = u.velocityx();
            //unitData.velocityY = u.velocityy();
            unitData.exists = true;
          };
          for (auto &u : SCRAPIunits)
          {
            UnitID unitID = static_cast<UnitID>(u.tag());
            auto itr = units.find(unitID);
            if (itr == units.end())
            {
              auto &newUnit = *units.emplace(game, unitID).first;
              fillUnitData(const_cast<UnitData &>(newUnit), u);
              game.addUnit(newUnit);
              Event e;
              e.setType(EventType::UnitCreate);
              e.setUnit(game.getUnit(const_cast<UnitData &>(newUnit).id));
              game.addEvent(e);
              if (const_cast<UnitData &>(newUnit).isCompleted)
              {
                e.setType(EventType::UnitComplete);
                game.addEvent(e);
              }
            }
            else
            {
              fillUnitData(const_cast<UnitData &>(*itr), u);
              game.addUnit(*itr);
            }
          }
          if (observationRaw.has_event())
          {
            for (const auto& tag : observationRaw.event().dead_units())
            {
              Event e;
              auto unitID = static_cast<UnitID>(tag);
              auto itr = units.find(unitID);
              e.setType(EventType::UnitDestroy);
              e.setUnit(game.getUnit(unitID));
              game.addEvent(e);
              const_cast<UnitData &>(*itr).exists = false;
            }
          }
        }
        if (response->has_query())
        {
          for (auto pathing : response->query().pathing())
          {
            //pathing.
          }
        }
        
      }
    }
    game.update();
  }
  void ProtoClient::onMatchFrame(Game& game)
  {
  }
  void ProtoClient::clearAll()
  {
  }
  void ProtoClient::onMatchStart(Game& game)
  {
  }
  void ProtoClient::initForces(Game& game)
  {
  }
  void ProtoClient::initPlayers(Game& game)
  {
  }
  void ProtoClient::initInitialUnits(Game& game)
  {
  }
  void ProtoClient::initRegions(Game& game)
  {
  }
  void ProtoClient::onMatchEnd(Game& game)
  {
  }
  void ProtoClient::initGame(Game& game)
  {
  }
  void ProtoClient::updateGame(Game& game)
  {
  }

  void ProtoClient::setScreenPosition(int x, int y)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newSetScreenPosition = newMessage->mutable_command()->mutable_setscreenposition();
    newSetScreenPosition->set_x(x);
    newSetScreenPosition->set_y(y);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::pingMinimap(int x, int y)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newPingMiniMap = newMessage->mutable_command()->mutable_pingminimap();
    newPingMiniMap->set_x(x);
    newPingMiniMap->set_y(y);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::drawShape(ShapeType::Enum shapeType, CoordinateType::Enum coordinateType, int x1, int y1, int x2, int y2, int extra1, int extra2, Color color, bool isSolid)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newShape = newMessage->mutable_command()->mutable_shape();
    newShape->set_type(static_cast<bwapi::command::ShapeType>(shapeType));
    newShape->set_ctype(static_cast<bwapi::command::CoordinateType>(coordinateType));
    newShape->set_x1(x1);
    newShape->set_y1(y1);
    newShape->set_x2(x2);
    newShape->set_y2(y2);
    newShape->set_extra1(extra1);
    newShape->set_extra2(extra2);
    newShape->set_color(static_cast<int>(color));
    newShape->set_issolid(isSolid);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::drawText(CoordinateType::Enum coordinateType, const std::string &text, int x, int y, int textSize)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newShape = newMessage->mutable_command()->mutable_shape();
    newShape->set_type(bwapi::command::ShapeType::Text);
    newShape->set_ctype(static_cast<bwapi::command::CoordinateType>(coordinateType));
    newShape->set_x1(x);
    newShape->set_y1(y);
    newShape->set_x2(0);
    newShape->set_y2(0);
    newShape->set_extra1(0);
    newShape->set_extra2(textSize);
    newShape->set_color(0);
    newShape->set_issolid(false);
    newShape->set_text(text);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::sendText(const std::string &text, bool toAllies)
  {
    if (!protoClient.isRemaster())
    {
      auto newMessage = std::make_unique<bwapi::message::Message>();
      auto newSendText = newMessage->mutable_command()->mutable_sendtext();
      newSendText->set_text(text);
      newSendText->set_toallies(toAllies);
      protoClient.queueMessage(std::move(newMessage));
    }
    else
    {
      auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
      auto action = newRequest->mutable_action()->add_actions();
      auto chat = action->mutable_action_chat();
      chat->set_message(text);
      chat->set_channel(static_cast<SCRAPIProtocol::ActionChat_Channel>(toAllies + 1));
      protoClient.queueMessage(std::move(newRequest));
    }
  }
  void ProtoClient::printText(const std::string &text)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newPrintf = newMessage->mutable_command()->mutable_printf();
    newPrintf->set_text(text);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::pauseGame()
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    newMessage->mutable_command()->mutable_pausegame();
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::resumeGame()
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    newMessage->mutable_command()->mutable_resumegame();
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::leaveGame()
  {
    if (!protoClient.isRemaster())
    {
      auto newMessage = std::make_unique<bwapi::message::Message>();
      newMessage->mutable_command()->mutable_leavegame();
      protoClient.queueMessage(std::move(newMessage));
    }
    else
    {
      auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
      newRequest->mutable_join_game();
      protoClient.queueMessage(std::move(newRequest));
    }
  }
  void ProtoClient::restartGame()
  {
    if (!protoClient.isRemaster())
    {
      auto newMessage = std::make_unique<bwapi::message::Message>();
      newMessage->mutable_command()->mutable_restartgame();
      protoClient.queueMessage(std::move(newMessage));
    }
    else
    {
      auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
      newRequest->mutable_restart_game();
      protoClient.queueMessage(std::move(newRequest));
    }
  }
  void ProtoClient::setLocalSpeed(int msPerFrame)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newSetLocalSpeed = newMessage->mutable_command()->mutable_setlocalspeed();
    newSetLocalSpeed->set_speed(msPerFrame);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::issueCommand(const Unitset& units, UnitCommand command)
  {
    if (!isRemaster())
    {
      auto newMessage = std::make_unique<bwapi::message::Message>();
      auto newUnitCommand = newMessage->mutable_command()->mutable_unitcommand();
      for (auto unit : units)
      {
        newUnitCommand->add_unitid(unit.getID().id);
      }
      newUnitCommand->set_unitcommandtype(command.getType());
      newUnitCommand->set_targetid(command.getTarget().id);
      newUnitCommand->set_x(command.x);
      newUnitCommand->set_y(command.y);
      newUnitCommand->set_extra(command.extra);
      protoClient.queueMessage(std::move(newMessage));
    }
    else
    {
      auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
      auto requestAction = newRequest->mutable_action();
      auto action = requestAction->add_actions();
      auto actionRaw = action->mutable_action_raw();
      auto actionRawUnitCommand = actionRaw->mutable_unit_command();
      actionRawUnitCommand->set_ability_id(command.getType());
      for (const auto &unit : units)
        actionRawUnitCommand->add_unit_tags(unit.getID().id);
      switch (command.getType())
      {
      case UnitCommandTypes::Attack_Move:

      {
        auto targetPoint = actionRawUnitCommand->mutable_target_world_space_pos();
        targetPoint->set_x(command.x);
        targetPoint->set_y(command.y);
      }
      break;
      case UnitCommandTypes::Attack_Unit:
      case UnitCommandTypes::Gather:
        actionRawUnitCommand->set_target_unit_tag(command.getTarget().id);
        break;
      case UnitCommandTypes::Train:
        actionRawUnitCommand->set_target_unit_tag(command.extra);
      default:
        break;
      }
    }
  }
  void ProtoClient::setAlliance(int playerId, int alliance)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newSetAlliance = newMessage->mutable_command()->mutable_setalliance();
    newSetAlliance->set_playerid(playerId);
    newSetAlliance->set_settings(alliance);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::setVision(int playerId, bool enabled)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newSetVision = newMessage->mutable_command()->mutable_setvision();
    newSetVision->set_playerid(playerId);
    newSetVision->set_settings(enabled ? 1 : 0);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::createUnit(int playerId, UnitType unitType, int x, int y, int count)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newCreateUnit = newMessage->mutable_command()->mutable_createunit();
    newCreateUnit->set_playerid(playerId);
    newCreateUnit->set_unittype(unitType.getID());
    newCreateUnit->set_x(x);
    newCreateUnit->set_y(y);
    newCreateUnit->set_count(count);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::killUnits(const Unitset& units, bool removeInstantly)
  {
    auto newMessage = std::make_unique<bwapi::message::Message>();
    auto newKillUnits = newMessage->mutable_command()->mutable_killunits();
    for (Unit u : units)
      newKillUnits->add_unitid(u.getID().id);
    newKillUnits->set_removeinstantly(removeInstantly);
    protoClient.queueMessage(std::move(newMessage));
  }
  void ProtoClient::createGameRequest(bool battlenetMap, const std::string& map, int playerCount, bool addComputer, Race race)
  {
    if (!protoClient.isRemaster())
      return;
    auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
    auto createGame = newRequest->mutable_create_game();
    if (battlenetMap)
      createGame->set_battlenet_map_name(map);
    else
      createGame->mutable_local_map()->set_map_path(map);
    auto test = static_cast<SCRAPIProtocol::Race>(race.getID());
    for (int i = 0; i < playerCount; i++)
    {
      auto playerSetup = createGame->add_player_setup();
      playerSetup->set_race(i == 0 ? static_cast<SCRAPIProtocol::Race>(race.getID()) : SCRAPIProtocol::Random);
      if (addComputer && i != 0)
        playerSetup->set_type(SCRAPIProtocol::PlayerType::Computer);
    }
    protoClient.queueMessage(std::move(newRequest));
  }
  void ProtoClient::gameInfoRequest()
  {
    if (!protoClient.isRemaster())
      return;
    auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
    newRequest->mutable_game_info();
    protoClient.queueMessage(std::move(newRequest));
  }
  void ProtoClient::observationRequest()
  {
    if (!protoClient.isRemaster())
      return;
    auto newRequest = std::make_unique<SCRAPIProtocol::Request>();
    newRequest->mutable_observation();
    protoClient.queueMessage(std::move(newRequest));
  }
  bool ProtoClient::isRemaster() const
  {
    return protoClient.isRemaster();
  }
}
