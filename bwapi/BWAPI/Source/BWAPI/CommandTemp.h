#pragma once
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Game.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
namespace BWAPI
{
  template <class UnitImpl, class PlayerImpl>
  class CommandTemp
  {
    public :
      CommandTemp(UnitCommand command);
      void execute(int frame);
    private :
      int getUnitID(Unit* unit);
      UnitCommand command;
      int savedExtra;
  };
  template <class UnitImpl, class PlayerImpl>
  CommandTemp<UnitImpl, PlayerImpl>::CommandTemp(UnitCommand command) : command(command)
  {
    savedExtra = -1;
  }
  template <class UnitImpl, class PlayerImpl>
  int CommandTemp<UnitImpl, PlayerImpl>::getUnitID(Unit* unit)
  {
    if (unit==NULL) return -1;
    return unit->getID();
  }
  template <class UnitImpl, class PlayerImpl>
  void CommandTemp<UnitImpl, PlayerImpl>::execute(int frame)
  {
    UnitImpl* unit   = (UnitImpl*)command.unit;
    UnitImpl* target = (UnitImpl*)command.target;
    Position position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType unitType(command.extra);
    UpgradeType upgradeType(command.extra);
    TechType techType(command.extra);
    if (frame>Broodwar->getLatency() && command.type !=UnitCommandTypes::Cancel_Train_Slot)
      return;

    if (command.type == UnitCommandTypes::Attack_Move)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canMove()) return;
      unit->self->order  = Orders::AttackMove.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if (target==NULL || !target->self->exists) return;
      if (!unit->self->exists) return;
      if (!unit->getType().canAttack()) return;
      unit->self->order = Orders::AttackUnit.getID();
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::PlaceBuilding.getID();
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      if (!unit->self->exists) return;
      unit->self->secondaryOrder = Orders::BuildAddon.getID();
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Burrowing.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      unit->self->isIdle = true;
      unit->self->isConstructing = false;
      unit->self->buildType = UnitTypes::None.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (savedExtra==-1)
        savedExtra = unit->self->type;
      unitType = UnitType(savedExtra);

      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (savedExtra==-1)
        savedExtra = unit->self->buildType;
      unitType = UnitType(savedExtra);

      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        p->self->minerals+=unitType.mineralPrice();
        p->self->gas+=unitType.gasPrice();
      }
      if (unitType.isTwoUnitsInOneEgg())
        p->self->supplyUsed[Races::Zerg.getID()]-=unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
      else
        p->self->supplyUsed[Races::Zerg.getID()]-=unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

      unit->self->buildType = UnitTypes::None.getID();
      unit->self->remainingBuildTime = 0;
      unit->self->isMorphing = false;
      unit->self->isConstructing = false;
      unit->self->order = Orders::Nothing.getID();
      unit->self->type = unitType.whatBuilds().first.getID();
      unit->self->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (savedExtra==-1)
        savedExtra = unit->self->tech;
      techType = TechType(savedExtra);

      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      unit->self->tech = TechTypes::None.getID();
      unit->self->isIdle = true;
      unit->self->remainingResearchTime = 0;
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        p->self->minerals += techType.mineralPrice();
        p->self->gas      += techType.gasPrice();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      if (!unit->self->exists) return;
      if (savedExtra==-1)
        savedExtra = unit->self->trainingQueue[unit->self->trainingQueueCount-1];
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        unit->self->trainingQueueCount--;
        if (unit->self->trainingQueueCount<0)
          unit->self->trainingQueueCount=0;
        p->self->minerals += UnitType(savedExtra).mineralPrice();
        p->self->gas += UnitType(savedExtra).gasPrice();
      }
      if (unit->self->trainingQueueCount==0)
      {
        p->self->allUnitCount[savedExtra]--;
        unit->self->isTraining = false;
        unit->self->remainingTrainTime = 0;
        unit->self->isIdle = true;
        p->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (!unit->self->exists) return;
      if (frame>Broodwar->getLatency()+2) return;
      if (savedExtra==-1)
        savedExtra = unit->self->trainingQueue[command.extra];
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        for(int i=command.extra;i<4;i++)
          unit->self->trainingQueue[i]=unit->self->trainingQueue[i+1];
        unit->self->trainingQueueCount--;
        if (unit->self->trainingQueueCount<0)
          unit->self->trainingQueueCount=0;
        p->self->minerals += UnitType(savedExtra).mineralPrice();
        p->self->gas += UnitType(savedExtra).gasPrice();
      }
      if (command.extra==0)
      {
        if (frame<Broodwar->getLatency())
        {
          p->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();
        }
        if (frame<Broodwar->getLatency()+1)
        {
          p->self->allUnitCount[savedExtra]--;
        }

        if (unit->self->trainingQueueCount==0)
        {
          unit->self->isTraining = false;
          unit->self->isIdle = true;
        }
        else
        {
          unit->self->remainingTrainTime=UnitType(unit->self->trainingQueue[0]).buildTime();
          p->self->supplyUsed[unit->getType().getRace().getID()] += UnitType(unit->self->trainingQueue[0]).supplyRequired();
          p->self->allUnitCount[unit->self->trainingQueue[0]]++;
          if (frame==Broodwar->getLatency())
          {
            p->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();
          }
        }
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      if (savedExtra==-1)
        savedExtra = unit->self->upgrade;
      upgradeType = UpgradeType(savedExtra);

      if (!unit->self->exists) return;
      unit->self->order = Orders::Nothing.getID();
      int level = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->self->upgrade = UpgradeTypes::None.getID();
      unit->self->isIdle = true;
      unit->self->remainingUpgradeTime = 0;
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        p->self->minerals += upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        p->self->gas      += upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Cloak.getID();
      if (frame<Broodwar->getLatency())
      {
        if (unit->self->type==UnitTypes::Terran_Ghost.getID())
          unit->self->energy -= BWAPI::TechTypes::Personnel_Cloaking.energyUsed();
        else
          unit->self->energy -= BWAPI::TechTypes::Cloaking_Field.energyUsed();
      }
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Decloak.getID();
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canMove()) return;
      unit->self->order = Orders::Follow.getID();
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      if (!unit->self->exists) return;
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      unit->self->isGathering = true;
      if (target->getType()==UnitTypes::Resource_Mineral_Field)
        unit->self->order = Orders::MoveToMinerals.getID();
      else if (target->getType().isRefinery())
        unit->self->order = Orders::MoveToGas.getID();
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::PlayerGuard.getID();
      unit->self->isConstructing = false;
      unit->self->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canMove()) return;
      unit->self->isMoving = false;
      unit->self->isIdle = false;
      unit->self->order = Orders::HoldPosition.getID();
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::BuildingLand.getID();
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::BuildingLiftOff.getID();
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (!unit->self->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::PickupBunker.getID();
        unit->self->target = getUnitID(target);
      }
      else if (unit->getType() == UnitTypes::Terran_Dropship
            || unit->getType() == UnitTypes::Protoss_Shuttle
            || unit->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->self->order = Orders::PickupTransport.getID();
        unit->self->target = getUnitID(target);
      }
      else if (target->getType() == UnitTypes::Terran_Bunker
            || target->getType() == UnitTypes::Terran_Dropship
            || target->getType() == UnitTypes::Protoss_Shuttle
            || target->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->self->order = Orders::EnterTransport.getID();
        unit->self->target = getUnitID(target);
      }
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (!unit->self->exists) return;
      unit->self->isMorphing = true;
      unit->self->isConstructing = true;
      unit->self->isIdle = false;
      unit->self->buildType = unitType.getID();
      unit->self->remainingBuildTime = unitType.buildTime();
      if (unitType.isBuilding())
      {
        unit->self->order = Orders::ZergBuildingMorph.getID();
        PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
        p->self->minerals -= unitType.mineralPrice();
        p->self->gas      -= unitType.gasPrice();
        p->self->allUnitCount[unitType.getID()]++;
      }
      else
      {
        unit->self->order = Orders::ZergUnitMorph.getID();
        PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
        if (frame<Broodwar->getLatency())
        {
          p->self->minerals -= unitType.mineralPrice();
          p->self->gas      -= unitType.gasPrice();
          p->self->allUnitCount[unitType.getID()]++;
        }
        if (unitType.isTwoUnitsInOneEgg())
          p->self->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
        else
          p->self->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

        if (unitType == UnitTypes::Zerg_Lurker)
          unit->self->type = UnitTypes::Zerg_Lurker_Egg.getID();
        else if (unitType == UnitTypes::Zerg_Devourer ||
                 unitType == UnitTypes::Zerg_Guardian)
          unit->self->type = UnitTypes::Zerg_Cocoon.getID();
        else
          unit->self->type = UnitTypes::Zerg_Egg.getID();
      }
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      if (!unit->self->exists) return;
      if (unit->getType().canMove()==false) return;
      unit->self->order = Orders::Move.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
      unit->self->isMoving = true;
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canMove()) return;
      unit->self->order = Orders::Patrol.getID();
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      if (!unit->self->exists) return;
      if (unit->getType()!=UnitTypes::Terran_SCV) return;
      unit->self->order = Orders::Repair.getID();
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::ResearchTech.getID();
      unit->self->tech = techType.getID();
      unit->self->isIdle = false;
      unit->self->remainingResearchTime = techType.researchTime();
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        p->self->minerals -= techType.mineralPrice();
        p->self->gas      -= techType.gasPrice();
      }
      p->self->isResearching[techType.getID()] = true;
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (!unit->self->exists) return;
      if (unit->self->carryResourceType!=1 && unit->self->carryResourceType!=2) return;
      if (unit->isCarryingGas())
        unit->self->order = Orders::ReturnGas.getID();
      else
        unit->self->order = Orders::ReturnMinerals.getID();
      unit->self->isGathering = true;
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if (!unit->self->exists) return;
      if (unit->getType().canMove()==false) return;
      unit->self->order = Orders::Move.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
      unit->self->isMoving = true;
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      if (!unit->self->exists) return;
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      if (unit->getType().isWorker() && target->getType()==UnitTypes::Resource_Mineral_Field)
      {
        unit->self->isGathering = true;
        unit->self->order = Orders::MoveToMinerals.getID();
      }
      else if (unit->getType().isWorker() && target->getType().isRefinery())
      {
        unit->self->isGathering = true;
        unit->self->order = Orders::MoveToGas.getID();
      }
      else if (unit->getType().isWorker() &&
               target->getType().getRace()==Races::Terran &&
               target->getType().whatBuilds().first==unit->getType() &&
               !target->isCompleted())
      {
        unit->self->order = Orders::ConstructingBuilding.getID();
        unit->self->buildUnit = getUnitID(target);
        target->self->buildUnit = getUnitID(unit);
        unit->self->isConstructing = true;
        target->self->isConstructing = true;
      }
      else if (unit->getType().canAttack() && target->getPlayer() != unit->getPlayer() && !target->getType().isNeutral())
      {
        unit->self->order = Orders::AttackUnit.getID();
      }
      else if (unit->getType().canMove())
        unit->self->order = Orders::Follow.getID();
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canProduce()) return;
      unit->self->order = Orders::RallyPointTile.getID();
      unit->self->rallyPositionX = position.x();
      unit->self->rallyPositionY = position.y();
      unit->self->rallyUnit = -1;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->self->exists) return;
      if (!unit->getType().canProduce()) return;
      if (target == NULL || !target->self->exists) return;
      unit->self->order = Orders::RallyPointUnit.getID();
      unit->self->rallyUnit = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Sieging.getID();
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Stop.getID();
      unit->self->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      if (!unit->self->exists) return;
      if (savedExtra==-1)
        savedExtra = unit->self->trainingQueueCount;
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        unit->self->trainingQueue[unit->self->trainingQueueCount] = unitType.getID();
        unit->self->trainingQueueCount++;
        p->self->minerals -= unitType.mineralPrice();
        p->self->gas      -= unitType.gasPrice();
      }
      if (savedExtra==0)
      {
        p->self->allUnitCount[unitType.getID()]++;
        unit->self->remainingTrainTime = unitType.buildTime();
        p->self->supplyUsed[unitType.getRace().getID()] += unitType.supplyRequired();
      }
      unit->self->isTraining = true;
      unit->self->isIdle = false;
      if (unitType == UnitTypes::Terran_Nuclear_Missile.getID())
        unit->self->secondaryOrder = Orders::Train.getID();
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Unburrowing.getID();
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Unload.getID();
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if (!unit->self->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::Unload.getID();
      }
      else
      {
        unit->self->order = Orders::MoveUnload.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      if (!unit->self->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::Unload.getID();
      }
      else
      {
        unit->self->order = Orders::MoveUnload.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Unsieging.getID();
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Upgrade.getID();
      unit->self->upgrade = upgradeType.getID();
      unit->self->isIdle = false;
      int level = unit->getPlayer()->getUpgradeLevel(upgradeType.getID());
      unit->self->remainingUpgradeTime = upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*level;
      PlayerImpl* p = (PlayerImpl*)unit->getPlayer();
      if (frame<Broodwar->getLatency())
      {
        p->self->minerals -= upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        p->self->gas      -= upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
      p->self->isUpgrading[upgradeType.getID()] = true;
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if (!unit->self->exists) return;
      if (techType == TechTypes::Stim_Packs)
      {
        unit->self->hitPoints -= 10;
        unit->self->stimTimer = 17;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if (!unit->self->exists) return;
      if (techType==TechTypes::Dark_Swarm)
      {
        unit->self->order = Orders::CastDarkSwarm.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Disruption_Web)
      {
        unit->self->order = Orders::CastDisruptionWeb.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::EMP_Shockwave)
      {
        unit->self->order = Orders::CastEMPShockwave.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Ensnare)
      {
        unit->self->order = Orders::CastEnsnare.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Maelstrom)
      {
        unit->self->order = Orders::CastMaelstrom.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Nuclear_Strike)
      {
        unit->self->order = Orders::CastNuclearStrike.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Plague)
      {
        unit->self->order = Orders::CastPlague.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Psionic_Storm)
      {
        unit->self->order = Orders::CastPsionicStorm.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Recall)
      {
        unit->self->order = Orders::CastRecall.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Scanner_Sweep)
      {
        unit->self->order = Orders::CastScannerSweep.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Spider_Mines)
      {
        unit->self->order = Orders::PlaceMine.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Stasis_Field)
      {
        unit->self->order = Orders::CastStasisField.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if (!unit->self->exists) return;
      if (techType==TechTypes::Consume)
      {
        unit->self->order = Orders::CastConsume.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Defensive_Matrix)
      {
        unit->self->order = Orders::CastDefensiveMatrix.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Feedback)
      {
        unit->self->order = Orders::CastFeedback.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Hallucination)
      {
        unit->self->order = Orders::CastHallucination.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Healing)
      {
        unit->self->order = Orders::HealMove.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Infestation)
      {
        unit->self->order = Orders::CastInfestation.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Irradiate)
      {
        unit->self->order = Orders::CastIrradiate.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Lockdown)
      {
        unit->self->order = Orders::CastLockdown.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Mind_Control)
      {
        unit->self->order = Orders::CastMindControl.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Optical_Flare)
      {
        unit->self->order = Orders::CastOpticalFlare.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Parasite)
      {
        unit->self->order = Orders::CastParasite.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Restoration)
      {
        unit->self->order = Orders::CastRestoration.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Spawn_Broodlings)
      {
        unit->self->order = Orders::CastSpawnBroodlings.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Yamato_Gun)
      {
        unit->self->order = Orders::FireYamatoGun.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Archon_Warp)
      {
        unit->self->order = Orders::ArchonWarp.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Dark_Archon_Meld)
      {
        unit->self->order = Orders::DarkArchonMeld.getID();
        unit->self->orderTarget = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
    }
  }


};
