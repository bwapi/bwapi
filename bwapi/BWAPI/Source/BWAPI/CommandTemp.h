#pragma once
/* Note: This should be moved to a cpp */

#include <BW/UnitID.h>
#include <BW/TechID.h>
#include <BW/OrderID.h>
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
      int savedExtra2;
      PlayerImpl* player;
  };
  template <class UnitImpl, class PlayerImpl>
  CommandTemp<UnitImpl, PlayerImpl>::CommandTemp(UnitCommand command) : command(command)
  {
    savedExtra  = -1;
    savedExtra2 = -1;
    player      = NULL;
  }
  template <class UnitImpl, class PlayerImpl>
  int CommandTemp<UnitImpl, PlayerImpl>::getUnitID(Unit* unit)
  {
    if ( !unit )
      return -1;
    return unit->getID();
  }
  template <class UnitImpl, class PlayerImpl>
  void CommandTemp<UnitImpl, PlayerImpl>::execute(int frame)
  {
    UnitImpl* unit   = (UnitImpl*)command.unit;
    UnitImpl* target = (UnitImpl*)command.target;

    Position     position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType     unitType(command.extra);
    UpgradeType  upgradeType(command.extra);
    TechType     techType(command.extra);

    if ( !player )
    {
      if ( unit )
        player = (PlayerImpl*)unit->getPlayer();
      else
        player = (PlayerImpl*)Broodwar->self();
    }

    if (frame > Broodwar->getLatency() &&
        command.type != UnitCommandTypes::Cancel_Construction &&
        command.type != UnitCommandTypes::Cancel_Train_Slot &&
        command.type != UnitCommandTypes::Cancel_Morph &&
        command.type != UnitCommandTypes::Morph)
      return;

    if (command.type == UnitCommandTypes::Attack_Move)
    {
      if ( !unit->self->exists )
        return;
      if ( !unit->getType().canMove() )
        return;
      unit->self->order           = Orders::AttackMove.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if ( !target || !target->self->exists)
        return;
      if (!unit->self->exists)
        return;
      if (!unit->getType().canAttack())
        return;
      unit->self->order = Orders::AttackUnit.getID();
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::PlaceBuilding.getID();
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      if (!unit->self->exists)
        return;
      unit->self->secondaryOrder = Orders::BuildAddon.getID();
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Burrowing.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      if (!unit->self->exists)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->buildType;
      unitType = UnitType(savedExtra);
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals += (int)(unitType.mineralPrice() * 0.75);
        player->self->gas      += (int)(unitType.gasPrice()     * 0.75);
      }
      unit->self->remainingBuildTime = 0;
      unit->self->isConstructing     = false;
      unit->self->order              = Orders::Nothing.getID();
      unit->self->isIdle             = true;
      unit->self->buildType          = UnitTypes::None.getID();
      unit->self->buildUnit          = -1;
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->type;
      if (savedExtra2 == -1)
        savedExtra2 = unit->self->buildUnit;
      unitType = UnitType(savedExtra);
      if (frame > Broodwar->getLatency() + 1)
        return;
      if (unitType.getRace() == Races::Terran)
      {
        UnitImpl* builder = (UnitImpl*)Broodwar->getUnit(savedExtra2);
        if ( builder && builder->exists())
        {
          builder->self->buildUnit      = -1;
          builder->self->buildType      = UnitTypes::None.getID();
          builder->self->isConstructing = false;
          builder->self->order          = Orders::ResetCollision.getID();
        }
      }
      if (frame > Broodwar->getLatency())
        return;
      if (!unit->self->exists)
        return;
      unit->self->buildUnit = -1;
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals += (int)(unitType.mineralPrice() * 0.75);
        player->self->gas      += (int)(unitType.gasPrice()     * 0.75);
      }
      unit->self->remainingBuildTime = 0;
      unit->self->isConstructing     = false;
      if (unitType.getRace() == Races::Zerg)
      {
        unit->self->type       = unitType.whatBuilds().first.getID();
        unit->self->buildType  = UnitTypes::None.getID();
        unit->self->isMorphing = false;
        unit->self->isIdle     = true;
        if (frame < Broodwar->getLatency())
          player->self->supplyUsed[unitType.getRace().getID()] += unitType.whatBuilds().first.supplyRequired();

        if (unitType.whatBuilds().first.isBuilding())
          unit->self->order = Orders::Nothing.getID();
        else
          unit->self->order = Orders::ResetCollision.getID();
      }
      else
      {
        unit->self->order       = Orders::Die.getID();
        unit->self->isCompleted = false;
        unit->self->isIdle      = false;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->buildType;
      unitType = UnitType(savedExtra);
      if (frame > Broodwar->getLatency() + 12)
        return;
      if (frame < Broodwar->getLatency())
      {
        if (unitType.whatBuilds().first.isBuilding())
        {
          player->self->minerals += (int)(unitType.mineralPrice()*0.75);
          player->self->gas      += (int)(unitType.gasPrice()*0.75);
        }
        else
        {
          player->self->minerals += unitType.mineralPrice();
          player->self->gas      += unitType.gasPrice();
        }
      }
      if (frame <= Broodwar->getLatency())
      {
        if (unitType.isTwoUnitsInOneEgg())
          player->self->supplyUsed[Races::Zerg.getID()] -= unitType.supplyRequired() * 2 - unitType.whatBuilds().first.supplyRequired();
        else
          player->self->supplyUsed[Races::Zerg.getID()] -= unitType.supplyRequired() - unitType.whatBuilds().first.supplyRequired();
      }
      unit->self->buildType          = UnitTypes::None.getID();
      unit->self->remainingBuildTime = 0;
      unit->self->isMorphing         = false;
      unit->self->isConstructing     = false;
      unit->self->isCompleted        = true;
      unit->self->isIdle             = true;
      unit->self->type               = unitType.whatBuilds().first.getID();
      if (unitType.whatBuilds().first.isBuilding())
        unit->self->order = Orders::Nothing.getID();
      else
        unit->self->order = Orders::PlayerGuard.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->tech;
      techType = TechType(savedExtra);

      if (!unit->self->exists)
        return;
      unit->self->order                 = Orders::Nothing.getID();
      unit->self->tech                  = TechTypes::None.getID();
      unit->self->isIdle                = true;
      unit->self->remainingResearchTime = 0;
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals += techType.mineralPrice();
        player->self->gas      += techType.gasPrice();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      if (!unit->self->exists)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueue[unit->self->trainingQueueCount - 1];
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame < Broodwar->getLatency() - 2 && Broodwar->getLatency() > 2))
      {
        unit->self->trainingQueueCount--;
        if (unit->self->trainingQueueCount < 0)
          unit->self->trainingQueueCount = 0;
        player->self->minerals += UnitType(savedExtra).mineralPrice();
        player->self->gas      += UnitType(savedExtra).gasPrice();
      }
      if (unit->self->trainingQueueCount == 0)
      {
        player->self->allUnitCount[savedExtra]--;
        unit->self->isTraining         = false;
        unit->self->remainingTrainTime = 0;
        unit->self->isIdle             = true;
        player->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (!unit->self->exists)
        return;
      if (frame > Broodwar->getLatency() + 2)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueue[command.extra];
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame < Broodwar->getLatency()-2 && Broodwar->getLatency() > 2))
      {
        for(int i = command.extra; i < 4; ++i)
          unit->self->trainingQueue[i] = unit->self->trainingQueue[i+1];
        unit->self->trainingQueueCount--;
        if (unit->self->trainingQueueCount < 0)
          unit->self->trainingQueueCount = 0;
        player->self->minerals += UnitType(savedExtra).mineralPrice();
        player->self->gas      += UnitType(savedExtra).gasPrice();
      }
      if (command.extra == 0)
      {
        if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
            (frame < Broodwar->getLatency()-1 && Broodwar->getLatency() > 2))
          player->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();

        if ((frame<=Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
            (frame < Broodwar->getLatency()-1 && Broodwar->getLatency() > 2))
          player->self->allUnitCount[savedExtra]--;

        if (unit->self->trainingQueueCount == 0)
        {
          unit->self->isTraining = false;
          unit->self->isIdle     = true;
        }
        else
        {
          unit->self->remainingTrainTime = UnitType(unit->self->trainingQueue[0]).buildTime();
          player->self->supplyUsed[unit->getType().getRace().getID()] += UnitType(unit->self->trainingQueue[0]).supplyRequired();
          player->self->allUnitCount[unit->self->trainingQueue[0]]++;
          if ((frame == Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
              (frame == Broodwar->getLatency()+1 && Broodwar->getLatency() > 2) )
            player->self->supplyUsed[unit->getType().getRace().getID()] -= UnitType(savedExtra).supplyRequired();
        }
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->upgrade;
      upgradeType = UpgradeType(savedExtra);

      if (!unit->self->exists)
        return;

      unit->self->order                = Orders::Nothing.getID();
      int level                        = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->self->upgrade              = UpgradeTypes::None.getID();
      unit->self->isIdle               = true;
      unit->self->remainingUpgradeTime = 0;

      if (frame < Broodwar->getLatency())
      {
        player->self->minerals += upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        player->self->gas      += upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Cloak.getID();
      if (frame < Broodwar->getLatency())
      {
        if (unit->self->type == UnitTypes::Terran_Ghost.getID())
          unit->self->energy -= BWAPI::TechTypes::Personnel_Cloaking.energyUsed();
        else
          unit->self->energy -= BWAPI::TechTypes::Cloaking_Field.energyUsed();
      }
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Decloak.getID();
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      if (!unit->self->exists)
        return;
      if (!unit->getType().canMove())
        return;
      unit->self->order    = Orders::Follow.getID();
      unit->self->target   = getUnitID(target);
      unit->self->isIdle   = false;
      unit->self->isMoving = true;
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      if (!unit->self->exists)
        return;
      unit->self->target      = getUnitID(target);
      unit->self->isIdle      = false;
      unit->self->isMoving    = true;
      unit->self->isGathering = true;
      if (target->getType() == UnitTypes::Resource_Mineral_Field)
        unit->self->order = Orders::MoveToMinerals.getID();
      else if (target->getType().isRefinery())
        unit->self->order = Orders::MoveToGas.getID();
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      if (!unit->self->exists)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->buildUnit;
      if (frame > Broodwar->getLatency())
        return;
      UnitImpl* buildUnit = (UnitImpl*)Broodwar->getUnit(savedExtra);
      if ( buildUnit )
        buildUnit->self->buildUnit = -1;

      unit->self->buildUnit      = -1;
      unit->self->buildType      = UnitTypes::None.getID();
      unit->self->order          = Orders::ResetCollision.getID();
      unit->self->isConstructing = false;
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      if (!unit->self->exists)
        return;
      if (!unit->getType().canMove())
        return;
      unit->self->isMoving = false;
      unit->self->isIdle   = false;
      unit->self->order    = Orders::HoldPosition.getID();
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      if (!unit->self->exists)
        return;
      unit->self->order  = Orders::BuildingLand.getID();
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      if (!unit->self->exists)
        return;
      unit->self->order  = Orders::BuildingLiftOff.getID();
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (!unit->self->exists)
        return;
      switch (unit->getType().getID())
      {
      case BW::UnitID::Terran_Bunker:
        unit->self->order  = Orders::PickupBunker.getID();
        unit->self->target = getUnitID(target);
        break;
      case BW::UnitID::Terran_Dropship:
      case BW::UnitID::Protoss_Shuttle:
      case BW::UnitID::Zerg_Overlord:
        unit->self->order  = Orders::PickupTransport.getID();
        unit->self->target = getUnitID(target);
        break;
      default:
        if (target->getType() == UnitTypes::Terran_Bunker   ||
            target->getType() == UnitTypes::Terran_Dropship ||
            target->getType() == UnitTypes::Protoss_Shuttle ||
            target->getType() == UnitTypes::Zerg_Overlord)
        {
          unit->self->order  = Orders::EnterTransport.getID();
          unit->self->target = getUnitID(target);
        }
        break;
      }
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (!unit->self->exists)
        return;
      if (frame > Broodwar->getLatency() + 1)
        return;
      unit->self->isMorphing     = true;
      unit->self->isConstructing = true;
      unit->self->isCompleted    = false;
      unit->self->isIdle         = false;
      unit->self->buildType      = unitType.getID();
      if (unit->self->remainingBuildTime < 50)
        unit->self->remainingBuildTime = unitType.buildTime();
      if (frame > Broodwar->getLatency())
        return;
      if (unitType.isBuilding())
      {
        unit->self->order       = Orders::ZergBuildingMorph.getID();
        player->self->minerals -= unitType.mineralPrice();
        player->self->gas      -= unitType.gasPrice();
        player->self->allUnitCount[unitType.getID()]++;
      }
      else
      {
        unit->self->order = Orders::ZergUnitMorph.getID();
        if (frame < Broodwar->getLatency())
        {
          player->self->minerals -= unitType.mineralPrice();
          player->self->gas      -= unitType.gasPrice();
          player->self->allUnitCount[unitType.getID()]++;
        }
        if (unitType.isTwoUnitsInOneEgg())
          player->self->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
        else
          player->self->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

        switch ( unitType.getID() )
        {
        case BW::UnitID::Zerg_Lurker:
          unit->self->type = BW::UnitID::Zerg_LurkerEgg;
          break;
        case BW::UnitID::Zerg_Devourer:
        case BW::UnitID::Zerg_Guardian:
          unit->self->type = BW::UnitID::Zerg_Cocoon;
          break;
        default:
          unit->self->type = BW::UnitID::Zerg_Egg;
          break;
        }
      }
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      if (!unit->self->exists) 
        return;
      if (!unit->getType().canMove())
        return;
      unit->self->order           = Orders::Move.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
      unit->self->isMoving        = true;
      unit->self->isIdle          = false;
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      if (!unit->self->exists) 
        return;
      if (!unit->getType().canMove()) 
        return;
      unit->self->order           = Orders::Patrol.getID();
      unit->self->isIdle          = false;
      unit->self->isMoving        = true;
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      if (!unit->self->exists)
        return;
      if (unit->getType() != UnitTypes::Terran_SCV)
        return;
      unit->self->order  = Orders::Repair.getID();
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      if (!unit->self->exists)
        return;
      unit->self->order                 = Orders::ResearchTech.getID();
      unit->self->tech                  = techType.getID();
      unit->self->isIdle                = false;
      unit->self->remainingResearchTime = techType.researchTime();
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals -= techType.mineralPrice();
        player->self->gas      -= techType.gasPrice();
      }
      player->self->isResearching[techType.getID()] = true;
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (!unit->self->exists)
        return;
      if (unit->self->carryResourceType == 0)
        return;
      if (unit->isCarryingGas())
        unit->self->order = Orders::ReturnGas.getID();
      else
        unit->self->order = Orders::ReturnMinerals.getID();
      unit->self->isGathering = true;
      unit->self->isIdle      = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if (!unit->self->exists)
        return;
      if (!unit->getType().canMove())
        return;
      unit->self->order           = Orders::Move.getID();
      unit->self->targetPositionX = position.x();
      unit->self->targetPositionY = position.y();
      unit->self->isMoving        = true;
      unit->self->isIdle          = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      if (!unit->self->exists)
        return;
      unit->self->target   = getUnitID(target);
      unit->self->isIdle   = false;
      unit->self->isMoving = true;
      if (unit->getType().isWorker() && target->getType() == UnitTypes::Resource_Mineral_Field)
      {
        unit->self->isGathering = true;
        unit->self->order       = Orders::MoveToMinerals.getID();
      }
      else if (unit->getType().isWorker() && target->getType().isRefinery())
      {
        unit->self->isGathering = true;
        unit->self->order       = Orders::MoveToGas.getID();
      }
      else if (unit->getType().isWorker() &&
               target->getType().getRace() == Races::Terran &&
               target->getType().whatBuilds().first == unit->getType() &&
               !target->isCompleted())
      {
        unit->self->order            = Orders::ConstructingBuilding.getID();
        unit->self->buildUnit        = getUnitID(target);
        target->self->buildUnit      = getUnitID(unit);
        unit->self->isConstructing   = true;
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
      if (!unit->self->exists)
        return;
      if (!unit->getType().canProduce())
        return;
      unit->self->order          = Orders::RallyPointTile.getID();
      unit->self->rallyPositionX = position.x();
      unit->self->rallyPositionY = position.y();
      unit->self->rallyUnit      = -1;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->self->exists)
        return;
      if (!unit->getType().canProduce())
        return;
      if ( !target || !target->self->exists )
        return;
      unit->self->order     = Orders::RallyPointUnit.getID();
      unit->self->rallyUnit = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Sieging.getID();
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      if (!unit->self->exists)
        return;
      unit->self->order  = Orders::Stop.getID();
      unit->self->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      if (!unit->self->exists)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueueCount;
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame < Broodwar->getLatency() - 2 && Broodwar->getLatency() > 2))
      {
        unit->self->trainingQueue[unit->self->trainingQueueCount] = unitType.getID();
        unit->self->trainingQueueCount++;
        player->self->minerals -= unitType.mineralPrice();
        player->self->gas      -= unitType.gasPrice();
      }
      if ( ((frame <= Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
           (frame < Broodwar->getLatency() - 1 && Broodwar->getLatency() > 2)) &&
           savedExtra == 0 )
      {
        player->self->allUnitCount[unitType.getID()]++;
        unit->self->remainingTrainTime = unitType.buildTime();
        player->self->supplyUsed[unitType.getRace().getID()] += unitType.supplyRequired();
      }
      unit->self->isTraining = true;
      unit->self->isIdle     = false;
      if (unitType == UnitTypes::Terran_Nuclear_Missile.getID())
        unit->self->secondaryOrder = Orders::Train.getID();
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Unburrowing.getID();
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Unload.getID();
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if (!unit->self->exists)
        return;
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
      if (!unit->self->exists)
        return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::Unload.getID();
      }
      else
      {
        unit->self->order           = Orders::MoveUnload.getID();
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      if (!unit->self->exists)
        return;
      unit->self->order = Orders::Unsieging.getID();
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      if (!unit->self->exists)
        return;
      unit->self->order   = Orders::Upgrade.getID();
      unit->self->upgrade = upgradeType.getID();
      unit->self->isIdle  = false;
      int level           = unit->getPlayer()->getUpgradeLevel(upgradeType.getID());
      unit->self->remainingUpgradeTime = upgradeType.upgradeTimeBase() + upgradeType.upgradeTimeFactor()*level;
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals -= upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        player->self->gas      -= upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
      player->self->isUpgrading[upgradeType.getID()] = true;
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if (!unit->self->exists)
        return;
      if (techType == TechTypes::Stim_Packs)
      {
        unit->self->hitPoints -= 10;
        unit->self->stimTimer = 17;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if (!unit->self->exists)
        return;
      int techOrder = 0;
      switch ( techType.getID() )
      {
      case BW::TechID::DarkSwarm:
        techOrder = BW::OrderID::DarkSwarm;
        break;
      case BW::TechID::DisruptionWeb:
        techOrder = BW::OrderID::CastDisruptionWeb;
        break;
      case BW::TechID::EMPShockwave:
        techOrder = BW::OrderID::EmpShockwave;
        break;
      case BW::TechID::Ensnare:
        techOrder = BW::OrderID::Ensnare;
        break;
      case BW::TechID::Maelstorm:
        techOrder = BW::OrderID::CastMaelstrom;
        break;
      case BW::TechID::NuclearStrike:
        techOrder = BW::OrderID::NukeGround;
        break;
      case BW::TechID::Plague:
        techOrder = BW::OrderID::Plague;
        break;
      case BW::TechID::PsionicStorm:
        techOrder = BW::OrderID::PsiStorm;
        break;
      case BW::TechID::Recall:
        techOrder = BW::OrderID::Teleport;
        break;
      case BW::TechID::ScannerSweep:
        techOrder = BW::OrderID::Scanner;
        break;
      case BW::TechID::SpiderMines:
        techOrder = BW::OrderID::PlaceMine;
        break;
      case BW::TechID::StasisField:
        techOrder = BW::OrderID::StasisField;
        break;
      }
      if ( techOrder != 0 )
      {
        unit->self->order           = techOrder;
        unit->self->targetPositionX = position.x();
        unit->self->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if (!unit->self->exists)
        return;
      int techOrder = 0;
      switch ( techType.getID() )
      {
      case BW::TechID::Consume:
        techOrder = BW::OrderID::Consume;
        break;
      case BW::TechID::DefensiveMatrix:
        techOrder = BW::OrderID::DefensiveMatrix;
        break;
      case BW::TechID::Feedback:
        techOrder = BW::OrderID::CastFeedback;
        break;
      case BW::TechID::Hallucination:
        techOrder = BW::OrderID::Hallucination1;
        break;
      case BW::TechID::Healing:
        techOrder = BW::OrderID::HealMove;
        break;
      case BW::TechID::Infestation:
        techOrder = BW::OrderID::InfestMine2;
        break;
      case BW::TechID::Irradiate:
        techOrder = BW::OrderID::Irradiate;
        break;
      case BW::TechID::Lockdown:
        techOrder = BW::OrderID::MagnaPulse;
        break;
      case BW::TechID::MindControl:
        techOrder = BW::OrderID::CastMindControl;
        break;
      case BW::TechID::OpticalFlare:
        techOrder = BW::OrderID::CastOpticalFlare;
        break;
      case BW::TechID::Parasite:
        techOrder = BW::OrderID::CastParasite;
        break;
      case BW::TechID::Restoration:
        techOrder = BW::OrderID::Restoration;
        break;
      case BW::TechID::SpawnBroodlings:
        techOrder = BW::OrderID::SummonBroodlings;
        break;
      case BW::TechID::YamatoGun:
        techOrder = BW::OrderID::FireYamatoGun1;
        break;
      case BW::TechID::ArchonWarp:
        techOrder = BW::OrderID::WarpingArchon;
        break;
      case BW::TechID::DarkArchonMeld:
        techOrder = BW::OrderID::WarpingDarkArchon;
        break;
      }
      if ( techOrder != 0 )
      {
        unit->self->order           = techOrder;
        unit->self->orderTarget     = getUnitID(target);
        unit->self->targetPositionX = target->getPosition().x();
        unit->self->targetPositionY = target->getPosition().y();
      }
    }
  }

};
