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
  {}
  Client::~Client()
  {
    this->disconnect();
  }
  bool Client::isConnected() const
  {
    return protoClient.isConnected();
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
        return true;
    }
    return protoClient.isConnected();
  }
  void Client::disconnect()
  {
    if (!connected)
      return;
    protoClient.disconnect();

  }
  void Client::queueMessage(std::unique_ptr<bwapi::message::Message> message)
  {
    protoClient.queueMessage(std::move(message));
  }
  void Client::update(Game& game)
  {
    game.clearEvents();
    game.flush();
    protoClient.transmitMessages();
    protoClient.receiveMessages();
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
            game.gameData->isInGame = gameUpdate.isingame();
            game.gameData->randomSeed = gameUpdate.randomseed();
          }
          for (auto &p : gameMessage.players())
          {
            auto fillPlayerData = [](PlayerData& playerData, const bwapi::data::Player& p) {
              auto &allUnitCount = p.allunitcount();
              auto &allCompletedUnitCount = p.completedunitcount();
              auto &deadUnitCount = p.deadunitcount();
              auto &isUnitAvailable = p.isunitavailable();
              auto &killedUnitCount = p.killedunitcount();
              auto &visibleUnitCount = p.visibleunitcount();
              for (int i = 0; i < 234; i++)
              {
                playerData.allUnitCount[i] = allUnitCount[i];
                playerData.completedUnitCount[i] = allCompletedUnitCount[i];
                playerData.deadUnitCount[i] = deadUnitCount[i];
                playerData.isUnitAvailable[i] = isUnitAvailable[i];
                playerData.killedUnitCount[i] = killedUnitCount[i];
                playerData.visibleUnitCount[i] = visibleUnitCount[i];
              }
              playerData.color = Color{ p.color() };
              playerData.customScore = p.customscore();
              playerData.gas = p.gas();
              playerData.gatheredGas = p.gatheredgas();
              playerData.gatheredMinerals = p.gatheredminerals();
              auto &hasResearched = p.hasresearched();
              auto &isResearchAvailable = p.isresearchavailable();
              auto &isResearching = p.isresearching();
              for (int i = 0; i < 47; i++)
              {
                playerData.hasResearched[i] = hasResearched[i];
                playerData.isResearchAvailable[i] = isResearchAvailable[i];
                playerData.isResearching[i] = isResearching[i];
              }
              auto &isAlly = p.isally();
              auto &isEnemy = p.isenemy();
              for (int i = 0; i < 12; i++)
              {
                playerData.isAlly[i] = isAlly[i];
                playerData.isEnemy[i] = isEnemy[i];
              }
              playerData.isDefeated = p.isdefeated();
              playerData.isNeutral = p.isneutral();     
              playerData.isParticipating = p.isparticipating();
              auto &isUpgrading = p.isupgrading();
              auto &maxUpgradeLevel = p.maxupgradelevel();
              auto &upgradeLevel = p.upgradelevel();
              for (int i = 0; i < 63; i++)
              {
                playerData.isUpgrading[i] = isUpgrading[i];
                playerData.maxUpgradeLevel[i] = maxUpgradeLevel[i];
                playerData.upgradeLevel[i] = upgradeLevel[i];
              }
              playerData.isVictorious = p.isvictorious();
              playerData.leftGame = p.leftgame();
              playerData.minerals = p.minerals();
              playerData.name = p.name();
              playerData.race = Race{ p.race() };
              playerData.refundedGas = p.refundedgas();
              playerData.refundedMinerals = p.refundedminerals();
              playerData.repairedGas = p.repairedgas();
              playerData.repairedMinerals = p.repairedminerals();
              playerData.startLocationX = p.startlocationx();
              playerData.startLocationY = p.startlocationy();
              auto &supplyTotal = p.supplytotal();
              auto &supplyUsed = p.supplyused();
              for (int i = 0; i < 3; i++)
              {
                playerData.supplyTotal[i] = supplyTotal[i];
                playerData.supplyUsed[i] = supplyUsed[i];
              }
              playerData.totalBuildingScore = p.totalbuildingscore();
              playerData.totalKillScore = p.totalkillscore();
              playerData.totalRazingScore = p.totalrazingscore();
              playerData.totalUnitScore = p.totalunitscore();
              playerData.type = PlayerType{ p.type() };
            };
            auto playerID = PlayerID{ p.id() };
            auto itr = players.find(playerID);
            PlayerData * playerData;
            if (itr == players.end())
            {
              auto &playerData = *players.emplace(game, playerID, ForceID{ p.force() }).first;
              fillPlayerData(const_cast<PlayerData&>(playerData), p);
              game.updatePlayer(playerData);
            }
            else
            {
              PlayerData playerData = *itr;
              fillPlayerData(playerData, p);
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
              //unitData.clearanceLevel <-- how does this still exist?
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
              //unitData.isVisible = u.isvisible();
              unitData.killCount = u.killcount();
              for (auto &l : u.larva())
                unitData.larva.push_back(UnitID{ l });
              unitData.lastAttackerPlayer = PlayerID{ u.lastattackerplayer() };
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
              for(auto &tqu : u.trainingqueue())
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
        }
      }
      else if (message->has_event())
      {
        Event e2;
        auto e = message->event();
        if (e.has_matchend())
        {
          e2.setType(EventType::MatchEnd);
          e2.setWinner(e.matchend().winner());
          game.addEvent(e2);
        }
        else if (e.has_sendtext())
        {
          e2.setType(EventType::SendText);
          e2.setText(e.sendtext().text().c_str());
          game.addEvent(e2);
        }
        else if (e.has_savegame())
        {
          e2.setType(EventType::SaveGame);
          e2.setText(e.savegame().text().c_str());
          game.addEvent(e2);
        }
        else if (e.has_playerleft())
        {
          e2.setType(EventType::PlayerLeft);
          e2.setPlayer(game.getPlayer(PlayerID{ e.playerleft().player() }));
          game.addEvent(e2);
        }
        else if (e.has_receivetext())
        {
          e2.setType(EventType::ReceiveText);
          e2.setText(e.receivetext().text().c_str());
          game.addEvent(e2);
        }
        else if (e.has_unitdiscover())
        {
          e2.setType(EventType::UnitDiscover);
          e2.setUnit(game.getUnit(UnitID{ e.unitdiscover().unit() }));
          game.addEvent(e2);
        }
      }
      else if (message->has_endofqueue())
        break;
    }
    game.update();
  }
  void Client::onMatchFrame(Game& game)
  {
  }
  void Client::clearAll()
  {
  }
  void Client::onMatchStart(Game& game)
  {
  }
  void Client::initForces(Game& game)
  {
  }
  void Client::initPlayers(Game& game)
  {
  }
  void Client::initInitialUnits(Game& game)
  {
  }
  void Client::initRegions(Game& game)
  {
  }
  void Client::onMatchEnd(Game& game)
  {
  }

  void Client::initGame(Game& game)
  {
  }
  void Client::updateGame(Game& game)
  {
  }
}
