#include <BWAPI.h>
#include "../Network/BWAPIFrontendClient/ProtoClient.h"

#include <iostream>

using namespace BWAPI;

//This is copied out of ProtoClient class, but with the receive call taken out,
//and modified to tkae a generic BWAPIProtoClient object to update from.
void update(Game& game, BWAPIProtoClient& protoClient);

//These are needed for the update function.
std::set<PlayerData, IDCompare> players;
std::set<UnitData, IDCompare> units;
std::set<RegionData, IDCompare> regions;
std::set<BulletData, IDCompare> bullets;
std::set<ForceData, IDCompare> forces;

int main()
{
  BWAPIProtoClient connectionToClient;
  BWAPIProtoClient connectionToServer;
  ProtoClient dummyClient;
  Game broodwar(dummyClient);
  std::cout << "Connecting to server..." << std::endl;
  while (!connectionToServer.isConnected())
  {
    connectionToServer.lookForServer(0, "x", false);
  }
  std::cout << "Connected to server!" << std::endl;

  std::cout << "Waiting for client to connect." << std::endl;
  connectionToClient.initListen();
  while (!connectionToClient.isConnected())
  {
    connectionToClient.checkForConnection(0, "x", "x");
    //Keep game running by sending/receiving server messages and clearing them out
    connectionToServer.receiveMessages();
    while (connectionToServer.messageQueueSize())
      connectionToServer.getNextMessage();
    connectionToServer.transmitMessages();
  }
  connectionToClient.stopListen();

  while (true)
  {
    //Get messages from server and load them up for sending to client.
    connectionToServer.receiveMessages();

    BWAPIProtoClient tempMessagesFromServer;

    //Transfer the messages over to the Client connection while making a copy into
    //tempMessagesFromServer for updating our Game. We can use this opportunity to intercept
    //any information we don't want to go to the bot that might come from the backend server.
    //For example, if we want to hide undetected cloaked/burrowed units, or if we want to
    //hide some map information.
    while (connectionToServer.messageQueueSize())
    {
      auto nextMessage = connectionToServer.getNextMessage();
      auto copiedMessage = std::make_unique<bwapi::message::Message>();
      copiedMessage->CopyFrom(*nextMessage);
      connectionToClient.queueMessage(std::move(nextMessage));
      tempMessagesFromServer.queueMessage(std::move(copiedMessage));
    }
    //Send desired messages to Client. If we wanted to intercept and modify any messages,
    //we could do so in the above while loop.
    connectionToClient.transmitMessages();

    //While the bot runs it's code, we will update the tournament client's copy of game.
    //Any logic for camera control can be placed here too, as it will run in parallel to
    //the bot (unless the bot's code finishes before this).
    update(broodwar, tempMessagesFromServer);

    //Recieve the bot's messages intended for the server.
    connectionToClient.receiveMessages();

    //Transfer the messages over to the Server connection. We can use this opportunity to intercept
    //any commands we want to prevent the bot from calling, such as camera controls if we are
    //running an auto obs.
    while (connectionToClient.messageQueueSize())
      connectionToServer.queueMessage(connectionToClient.getNextMessage());

    //Finally, submit the bot commands we allowed through along with any new commands from the
    //tournament.
    connectionToServer.transmitMessages();
  }

  return 0;
}

void update(Game& game, BWAPIProtoClient& protoClient)
{
  game.clearEvents();

  while (protoClient.messageQueueSize())
  {
    auto message = protoClient.getNextMessage();
    if (message->has_frameupdate())
    {
      //update game here
      if (message->frameupdate().has_game())
      {
        auto& gameMessage = message->frameupdate().game();
        if (gameMessage.has_gamedata())
        {
          auto& gameUpdate = gameMessage.gamedata();
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
          if (gameUpdate.has_staticmap())
          {
            auto& staticMap = gameUpdate.staticmap();
            game.gameData->map.size = TilePosition{ staticMap.size().x(), staticMap.size().y() };
            for (auto& isWalkable : staticMap.iswalkable())
              game.gameData->map.isWalkable[isWalkable.x()][isWalkable.y()] = isWalkable.value();

            auto groundHeight = staticMap.groundheight().begin();
            auto isBuildable = staticMap.isbuildable().begin();
            auto mapTile = staticMap.maptileregionid().begin();
            while (groundHeight != staticMap.groundheight().end())
            {
              game.gameData->map.groundHeight[groundHeight->x()][groundHeight->y()] = groundHeight->value();
              game.gameData->map.isBuildable[isBuildable->x()][isBuildable->y()] = isBuildable->value();
              game.gameData->map.mapTileRegionId[mapTile->x()][mapTile->y()] = static_cast<unsigned short>(mapTile->value());
              groundHeight++;
              isBuildable++;
              mapTile++;
            }
            game.gameData->map.mapHash = staticMap.maphash();

            // TODO: These uint32 are being implicitly cast to unsigned short, they need a proper explicit cast/conversion
            std::copy(staticMap.mapsplittilesminitilemask().begin(), staticMap.mapsplittilesminitilemask().end(), std::begin(game.gameData->map.mapSplitTilesMiniTileMask));
            std::copy(staticMap.mapsplittilesregion1().begin(), staticMap.mapsplittilesregion1().end(), std::begin(game.gameData->map.mapSplitTilesRegion1));
            std::copy(staticMap.mapsplittilesregion2().begin(), staticMap.mapsplittilesregion2().end(), std::begin(game.gameData->map.mapSplitTilesRegion2));

            game.gameData->map.tileset = staticMap.tileset();
            game.gameData->mapName = staticMap.mapname();
            game.gameData->mapFileName = staticMap.mapfilename();
            game.gameData->mapPath = staticMap.mappath();

            for (auto &sp : staticMap.startpositions())
              game.gameData->startPositions.push_back(TilePosition{ sp.x(), sp.y() });
          }
          if (gameUpdate.has_map())
          {
            auto& map = gameUpdate.map();
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
            //unitData.lastCommand = UnitCommand{ u.lastcommand() };
            unitData.lastCommandFrame = u.lastcommandframe();
            unitData.lastHitPoints = u.lasthitpoints();
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
      auto& e = message->event();
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
        auto& target = e.nukedetect().target();
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
  game.update();
}