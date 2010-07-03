#include "CommandGeneral.h"
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Game.h>
#include <BWAPI/GameImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandGeneral::CommandGeneral(UnitCommand command) : command(command)
  {
    startFrame = Broodwar->getFrameCount();
    savedExtra = -1;
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandGeneral::execute()
  {
    UnitImpl* unit = (UnitImpl*)command.unit;
    UnitImpl* target = (UnitImpl*)command.target;
    Position position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType unitType(command.extra);
    UpgradeType upgradeType(command.extra);
    TechType techType(command.extra);

    if (command.type == UnitCommandTypes::Attack_Position)
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
      if (!unit->_exists) return;
      if (!unit->getType().canAttack()) return;
      unit->self->order = Orders::AttackUnit.getID();
      unit->self->target = BroodwarImpl.server.getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      if (!unit->self->exists) return;
      if (unit->getType() == UnitTypes::Zerg_Drone)
        unit->self->order = Orders::DroneLand.getID();
      else if (unit->getType() == UnitTypes::Protoss_Probe)
        unit->self->order = Orders::BuildProtoss1.getID();
      else if (unit->getType() == UnitTypes::Terran_SCV)
        unit->self->order = Orders::BuildTerran.getID();
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
      unit->self->order = Orders::Burrow.getID();
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

      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->spend(-unitType.mineralPrice(), -unitType.gasPrice());
        //-p->planToMake(toMorph);
      }
      if (unitType.isTwoUnitsInOneEgg())
        p->useSupplies(-(unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired()), (u8)Races::Zerg.getID());
      else
        p->useSupplies(-(unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired()), (u8)Races::Zerg.getID());

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
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(-techType.mineralPrice(),-techType.gasPrice());
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      if (!unit->self->exists) return;
      unit->self->trainingQueueCount--;
      if (unit->self->trainingQueueCount<0)
        unit->self->trainingQueueCount=0;
      if (unit->self->trainingQueueCount==0)
      {
        unit->self->isTraining = false;
        unit->self->order = Orders::Nothing.getID();
        unit->self->isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (!unit->self->exists) return;
      for(int i=command.extra;i<4;i++)
        unit->self->trainingQueue[i]=unit->self->trainingQueue[i+1];
      unit->self->trainingQueueCount--;
      if (unit->self->trainingQueueCount<0)
        unit->self->trainingQueueCount=0;
      if (unit->self->trainingQueueCount==0)
      {
        unit->self->isTraining = false;
        unit->self->order = Orders::Nothing.getID();
        unit->self->isIdle = true;
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
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(-upgradeType.mineralPriceBase()-upgradeType.mineralPriceFactor()*level, -upgradeType.gasPriceBase()-upgradeType.gasPriceFactor()*level);
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Cloak.getID();
      if (unit->self->type==UnitTypes::Terran_Ghost.getID())
        unit->self->energy -= BWAPI::TechTypes::Personnel_Cloaking.energyUsed();
      else
        unit->self->energy -= BWAPI::TechTypes::Cloaking_Field.energyUsed();
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
      unit->self->target = BroodwarImpl.server.getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
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
      if (unit->getType()==UnitTypes::Protoss_Carrier)
        unit->self->order = Orders::CarrierHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Zerg_Queen)
        unit->self->order = Orders::QueenHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Zerg_Infested_Terran || unit->getType()==UnitTypes::Zerg_Scourge)
        unit->self->order = Orders::SuicideHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Terran_Medic)
        unit->self->order = Orders::MedicHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Protoss_Reaver)
        unit->self->order = Orders::ReaverHoldPosition.getID();
      else
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
      executors[0]->self->order = Orders::BuildingLiftOff.getID();
      executors[0]->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (!unit->self->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        executors[0]->self->order = Orders::PickupBunker.getID();
        executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
      }
      else if (unit->getType() == UnitTypes::Terran_Dropship
            || unit->getType() == UnitTypes::Protoss_Shuttle
            || unit->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->self->order = Orders::PickupTransport.getID();
        unit->self->target = BroodwarImpl.server.getUnitID(target);
      }
      else if (target->getType() == UnitTypes::Terran_Bunker
            || target->getType() == UnitTypes::Terran_Dropship
            || target->getType() == UnitTypes::Protoss_Shuttle
            || target->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->self->order = Orders::EnterTransport.getID();
        unit->self->target = BroodwarImpl.server.getUnitID(target);
      }
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (!unit->self->exists) return;
      unit->self->buildType = unitType.getID();
      unit->self->remainingBuildTime = unitType.buildTime();
      unit->self->isMorphing = true;
      unit->self->isConstructing = true;
      unit->self->isIdle = false;
      if (unitType.isBuilding())
      {
        unit->self->order = Orders::ZergBuildingMorph.getID();
        PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
        p->spend(unitType.mineralPrice(), unitType.gasPrice());
        p->planToMake(unitType);
      }
      else
      {
        unit->self->order = Orders::ZergUnitMorph.getID();
        PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
        if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        {
          p->spend(unitType.mineralPrice(), unitType.gasPrice());
          p->planToMake(unitType);
        }
        if (unitType.isTwoUnitsInOneEgg())
          p->useSupplies(unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired(), (u8)unitType.getRace().getID());
        else
          p->useSupplies(unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired(), (u8)unitType.getRace().getID());

        if (unitType == UnitTypes::Zerg_Lurker)
          unit->self->type = UnitTypes::Zerg_Lurker_Egg.getID();
        else if (unitType == UnitTypes::Zerg_Devourer ||
                 unitType == UnitTypes::Zerg_Guardian)
          unit->self->type = UnitTypes::Zerg_Cocoon.getID();
        else
          unit->self->type = UnitTypes::Zerg_Egg.getID();
      }
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
      unit->self->order = Orders::Repair1.getID();
      unit->self->target = BroodwarImpl.server.getUnitID(target);
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::ResearchTech.getID();
      unit->self->tech = techType.getID();
      unit->self->isIdle = false;
      unit->self->remainingResearchTime = techType.researchTime();
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(techType.mineralPrice(),techType.gasPrice());
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (!unit->self->exists) return;
      if (unit->self->carryResourceType!=1 && unit->self->carryResourceType!=2) return;
      if (unit->isCarryingGas())
        unit->self->order = Orders::ReturnGas.getID();
      else
        unit->self->order = Orders::ReturnMinerals.getID();
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::SiegeMode.getID();
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
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        unit->self->trainingQueue[unit->self->trainingQueueCount] = unitType.getID();
        unit->self->trainingQueueCount++;
        p->spend(unitType.mineralPrice(), unitType.gasPrice());
        p->planToMake(unitType);
      }
      unit->self->remainingTrainTime = unitType.buildTime();
      unit->self->isTraining = true;
      unit->self->isIdle = false;
      if (unitType == BW::UnitID::Terran_NuclearMissile)
        unit->self->secondaryOrder = Orders::Train.getID();
      p->useSupplies(unitType.supplyRequired(), (u8)unitType.getRace().getID());
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Unburrow.getID();
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Unload.getID();
      unit->self->target = BroodwarImpl.server.getUnitID(target);
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
      unit->self->order = Orders::TankMode.getID();
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      if (!unit->self->exists) return;
      unit->self->order = Orders::Upgrade.getID();
      unit->self->upgrade = upgradeType.getID();
      unit->self->isIdle = false;
      int level = unit->getPlayer()->getUpgradeLevel(upgradeType.getID());
      unit->self->remainingUpgradeTime = upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*level;
      PlayerImpl* p = static_cast<PlayerImpl*>(unit->getPlayer());
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        p->spend(upgradeType.mineralPriceBase()+upgradeType.mineralPriceFactor()*level, upgradeType.gasPriceBase()+upgradeType.gasPriceFactor()*level);
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if (!unit->self->exists) return;
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if (!unit->self->exists) return;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandGeneral::getType()
  {
    return command.type.getID();
  }
  //----------------------------------------------------------------------------------------------------------
};
