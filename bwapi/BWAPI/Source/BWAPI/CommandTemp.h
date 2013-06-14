#pragma once
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
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
  protected:
    virtual ~CommandTemp() {};
  private :
    int getUnitID(Unit unit);
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
    player      = nullptr;
  }
  template <class UnitImpl, class PlayerImpl>
  int CommandTemp<UnitImpl, PlayerImpl>::getUnitID(Unit unit)
  {
    if ( !unit )
      return -1;
    return unit->getID();
  }
  template <class UnitImpl, class PlayerImpl>
  void CommandTemp<UnitImpl, PlayerImpl>::execute(int frame)
  {
    // Immediately return if latency compensation is disabled or if the command was queued
    if ( !Broodwar->isLatComEnabled() || command.isQueued() ) return;
    UnitImpl* unit   = static_cast<UnitImpl*>(command.unit);
    UnitImpl* target = static_cast<UnitImpl*>(command.target);

    Position     position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType     unitType(command.extra);
    UpgradeType  upgradeType(command.extra);
    TechType     techType(command.extra);

    // Get the player (usually the unit's owner)
    if ( !player )
      player = static_cast<PlayerImpl*>(unit ? unit->getPlayer() : Broodwar->self());

    // Latency test
    if (frame > Broodwar->getLatency() &&
        command.type != UnitCommandTypes::Cancel_Construction &&
        command.type != UnitCommandTypes::Cancel_Train_Slot &&
        command.type != UnitCommandTypes::Cancel_Morph &&
        command.type != UnitCommandTypes::Train &&
        command.type != UnitCommandTypes::Gather &&
        command.type != UnitCommandTypes::Stop &&
        command.type != UnitCommandTypes::Return_Cargo &&
        command.type != UnitCommandTypes::Right_Click_Position &&
        command.type != UnitCommandTypes::Morph)
      return;

    // Existence test
    switch ( command.type )
    {
    case UnitCommandTypes::Enum::Cancel_Construction:
    case UnitCommandTypes::Enum::Cancel_Research:
    case UnitCommandTypes::Enum::Cancel_Upgrade:
      break;
    default:
      if ( !unit->self->exists )
        return;
      break;
    }

    // Move test
    switch ( command.type )
    {
    case UnitCommandTypes::Enum::Follow:
    case UnitCommandTypes::Enum::Hold_Position:
    case UnitCommandTypes::Enum::Move:
    case UnitCommandTypes::Enum::Patrol:
    case UnitCommandTypes::Enum::Right_Click_Position:
    case UnitCommandTypes::Enum::Attack_Move:
      if (!unit->getType().canMove())
        return;
      break;
    }

    // Apply command changes
    if (command.type == UnitCommandTypes::Attack_Move)
    {
      unit->self->order           = Orders::AttackMove;
      unit->self->targetPositionX = position.x;
      unit->self->targetPositionY = position.y;
      unit->self->orderTargetPositionX = position.x;
      unit->self->orderTargetPositionY = position.y;
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if ( !target || !target->self->exists)
        return;
      if (!unit->getType().canAttack())
        return;
      unit->self->order = Orders::AttackUnit;
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      unit->self->order = Orders::PlaceBuilding;
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      unit->self->secondaryOrder = Orders::BuildAddon;
      unit->self->isConstructing = true;
      unit->self->isIdle         = false;
      unit->self->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      unit->self->order = Orders::Burrowing;
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
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
      unit->self->order              = Orders::Nothing;
      unit->self->isIdle             = true;
      unit->self->buildType          = UnitTypes::None;
      unit->self->buildUnit          = -1;
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->type;
      if (savedExtra2 == -1)
        savedExtra2 = unit->self->buildUnit;
      unitType = UnitType(savedExtra);
      if ((frame > Broodwar->getLatency() + 1 && Broodwar->getLatency() == 2) || 
          (frame > Broodwar->getLatency() + 2 && Broodwar->getLatency() > 2))
        return;
      if (unitType.getRace() == Races::Terran)
      {
        UnitImpl* builder = static_cast<UnitImpl*>( Broodwar->getUnit(savedExtra2) );
        if ( builder && builder->exists())
        {
          builder->self->buildUnit      = -1;
          builder->self->buildType      = UnitTypes::None;
          builder->self->isConstructing = false;
          builder->self->order          = Orders::ResetCollision;
        }
      }
      if (frame > Broodwar->getLatency())
        return;
      if (!unit->self->exists)
        return;
      unit->self->buildUnit = -1;
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency()==2) ||
          (frame <=Broodwar->getLatency() && Broodwar->getLatency()>2))
      {
        player->self->minerals += (int)(unitType.mineralPrice() * 0.75);
        player->self->gas      += (int)(unitType.gasPrice()     * 0.75);
      }
      unit->self->remainingBuildTime = 0;
      unit->self->isConstructing     = false;
      if (unitType.getRace() == Races::Zerg)
      {
        unit->self->type       = unitType.whatBuilds().first;
        unit->self->buildType  = UnitTypes::None;
        unit->self->isMorphing = false;
        unit->self->isIdle     = true;

        UnitType whatBuilds = unitType.whatBuilds().first;
        if (frame < Broodwar->getLatency())
          player->self->supplyUsed[unitType.getRace()] += whatBuilds.supplyRequired();

        if (whatBuilds.isBuilding())
        {
          unit->self->order = Orders::Nothing;
        }
        else
        {
          unit->self->order = Orders::ResetCollision;
        }
      }
      else
      {
        unit->self->order       = Orders::Die;
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
      if (frame<=Broodwar->getLatency())
      {
        if (unitType.isTwoUnitsInOneEgg())
          player->self->supplyUsed[Races::Zerg] -= unitType.supplyRequired() * 2 - unitType.whatBuilds().first.supplyRequired();
        else
          player->self->supplyUsed[Races::Zerg] -= unitType.supplyRequired() - unitType.whatBuilds().first.supplyRequired();
      }
      unit->self->buildType          = UnitTypes::None;
      unit->self->remainingBuildTime = 0;
      unit->self->isMorphing         = false;
      unit->self->isConstructing     = false;
      unit->self->isCompleted        = true;
      unit->self->isIdle             = true;
      unit->self->type               = unitType.whatBuilds().first;
      if (unitType.whatBuilds().first.isBuilding())
        unit->self->order = Orders::Nothing;
      else
        unit->self->order = Orders::PlayerGuard;
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->tech;
      techType = TechType(savedExtra);

      if (!unit->self->exists)
        return;
      unit->self->order                 = Orders::Nothing;
      unit->self->tech                  = TechTypes::None;
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
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueue[unit->self->trainingQueueCount - 1];
      if (savedExtra2 == -1)
        savedExtra2 = unit->self->buildUnit;
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame <= Broodwar->getLatency()+1 && Broodwar->getLatency() > 2))
      {
        unit->self->trainingQueueCount--;
        if (unit->self->trainingQueueCount < 0)
          unit->self->trainingQueueCount = 0;
        player->self->minerals += UnitType(savedExtra).mineralPrice();
        player->self->gas      += UnitType(savedExtra).gasPrice();
      }
      if (unit->self->trainingQueueCount == 0)
      {
        unit->self->buildUnit          = -1;
        unit->self->isTraining         = false;
        unit->self->remainingTrainTime = 0;
        unit->self->isIdle             = true;
        player->self->supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (frame > Broodwar->getLatency() + 2)
        return;
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueue[command.extra];
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame <= Broodwar->getLatency()+1 && Broodwar->getLatency() > 2))
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
        unit->self->buildUnit = -1;
        if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
            (frame <= Broodwar->getLatency()-1 && Broodwar->getLatency() > 2))
        {
          player->self->supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
        }

        if (unit->self->trainingQueueCount == 0)
        {
          unit->self->isTraining = false;
          unit->self->isIdle     = true;
        }
        else
        {
          unit->self->remainingTrainTime = UnitType(unit->self->trainingQueue[0]).buildTime();  // @TODO: fix to real build time
          player->self->supplyUsed[unit->getType().getRace()] += UnitType(unit->self->trainingQueue[0]).supplyRequired();
          if ((frame == Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
              (frame == Broodwar->getLatency()+1 && Broodwar->getLatency() > 2) )
          {
            player->self->supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
          }
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

      unit->self->order                = Orders::Nothing;
      int level                        = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->self->upgrade              = UpgradeTypes::None;
      unit->self->isIdle               = true;
      unit->self->remainingUpgradeTime = 0;

      if (frame < Broodwar->getLatency())
      {
        player->self->minerals += upgradeType.mineralPrice(level+1);
        player->self->gas      += upgradeType.gasPrice(level+1);
      }
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      unit->self->order = Orders::Cloak;
      if (frame < Broodwar->getLatency())
        unit->self->energy -= unit->getType().cloakingTech().energyCost();
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      unit->self->order = Orders::Decloak;
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      unit->self->order    = Orders::Follow;
      unit->self->target   = getUnitID(target);
      unit->self->isIdle   = false;
      unit->self->isMoving = true;
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      if ((frame<=Broodwar->getLatency()   && Broodwar->getLatency()==2) ||
          (frame<=Broodwar->getLatency()+1 && Broodwar->getLatency()>2))
      {
        unit->self->target      = getUnitID(target);
        unit->self->isIdle      = false;
        unit->self->isMoving    = true;
        unit->self->isGathering = true;
        if ( target->getType().isMineralField() )
          unit->self->order = Orders::MoveToMinerals;
        else if ( target->getType().isRefinery() )
          unit->self->order = Orders::MoveToGas;
      }
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->buildUnit;
      if (frame > Broodwar->getLatency())
        return;
      UnitImpl* buildUnit = static_cast<UnitImpl*>( Broodwar->getUnit(savedExtra) );
      if ( buildUnit )
        buildUnit->self->buildUnit = -1;

      unit->self->buildUnit      = -1;
      unit->self->buildType      = UnitTypes::None;
      unit->self->order          = Orders::ResetCollision;
      unit->self->isConstructing = false;
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      unit->self->isMoving = false;
      unit->self->isIdle   = false;
      unit->self->order    = Orders::HoldPosition;
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      unit->self->order  = Orders::BuildingLand;
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      unit->self->order  = Orders::BuildingLiftOff;
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order  = Orders::PickupBunker;
        unit->self->target = getUnitID(target);
      }
      else if ( unit->getType().spaceProvided() )
      {
        unit->self->order  = Orders::PickupTransport;
        unit->self->target = getUnitID(target);
      }
      else if ( target->getType().spaceProvided() )
      {
        unit->self->order  = Orders::EnterTransport;
        unit->self->target = getUnitID(target);
      }
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (frame > Broodwar->getLatency()+1)
        return;
      unit->self->isMorphing     = true;
      unit->self->isConstructing = true;
      unit->self->isCompleted    = false;
      unit->self->isIdle         = false;
      unit->self->buildType      = unitType;
      if (unit->self->remainingBuildTime < 50)
        unit->self->remainingBuildTime = unitType.buildTime();  // @TODO: Fix to real build time
      if (frame > Broodwar->getLatency())
        return;
      if (unitType.isBuilding())
      {
        unit->self->order       = Orders::ZergBuildingMorph;
        player->self->minerals -= unitType.mineralPrice();
        player->self->gas      -= unitType.gasPrice();
        unit->self->type        = unitType;
      }
      else
      {
        unit->self->order = Orders::ZergUnitMorph;
        if (frame < Broodwar->getLatency())
        {
          player->self->minerals -= unitType.mineralPrice();
          player->self->gas      -= unitType.gasPrice();
        }
        if (unitType.isTwoUnitsInOneEgg())
          player->self->supplyUsed[Races::Zerg] += unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
        else
          player->self->supplyUsed[Races::Zerg] += unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

        if (unitType == UnitTypes::Zerg_Lurker)
          unit->self->type = UnitTypes::Zerg_Lurker_Egg;
        else if (unitType == UnitTypes::Zerg_Devourer ||
                 unitType == UnitTypes::Zerg_Guardian)
          unit->self->type = UnitTypes::Zerg_Cocoon;
        else
          unit->self->type = UnitTypes::Zerg_Egg;
      }
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      unit->self->order                 = Orders::Move;
      unit->self->targetPositionX       = position.x;
      unit->self->targetPositionY       = position.y;
      unit->self->orderTargetPositionX  = position.x;
      unit->self->orderTargetPositionY  = position.y;
      unit->self->isMoving              = true;
      unit->self->isIdle                = false;
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      unit->self->order           = Orders::Patrol;
      unit->self->isIdle          = false;
      unit->self->isMoving        = true;
      unit->self->targetPositionX = position.x;
      unit->self->targetPositionY = position.y;
      unit->self->orderTargetPositionX = position.x;
      unit->self->orderTargetPositionY = position.y;
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      if (unit->getType() != UnitTypes::Terran_SCV)
        return;
      unit->self->order  = Orders::Repair;
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      unit->self->order                 = Orders::ResearchTech;
      unit->self->tech                  = techType;
      unit->self->isIdle                = false;
      unit->self->remainingResearchTime = techType.researchTime();  // @TODO: Fix to real time
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals -= techType.mineralPrice();
        player->self->gas      -= techType.gasPrice();
      }
      player->self->isResearching[techType] = true;
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (unit->self->carryResourceType == 0)
        return;
      if ((frame<=Broodwar->getLatency()   && Broodwar->getLatency()==2) ||
          (frame<=Broodwar->getLatency()+1 && Broodwar->getLatency()>2))
      {
        if (unit->isCarryingGas())
          unit->self->order = Orders::ReturnGas;
        else
          unit->self->order = Orders::ReturnMinerals;
        unit->self->isGathering = true;
        unit->self->isIdle      = false;
      }
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if ((frame<=Broodwar->getLatency()   && Broodwar->getLatency()==2) ||
          (frame<=Broodwar->getLatency()+1 && Broodwar->getLatency()>2))
      {
        unit->self->order           = Orders::Move;
        unit->self->targetPositionX = position.x;
        unit->self->targetPositionY = position.y;
        unit->self->orderTargetPositionX = position.x;
        unit->self->orderTargetPositionY = position.y;
        unit->self->isMoving        = true;
        unit->self->isIdle          = false;
      }
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      unit->self->target   = getUnitID(target);
      unit->self->isIdle   = false;
      unit->self->isMoving = true;
      if (unit->getType().isWorker() && target->getType().isMineralField() )
      {
        unit->self->isGathering = true;
        unit->self->order       = Orders::MoveToMinerals;
      }
      else if (unit->getType().isWorker() && target->getType().isRefinery() )
      {
        unit->self->isGathering = true;
        unit->self->order       = Orders::MoveToGas;
      }
      else if (unit->getType().isWorker() &&
               target->getType().getRace() == Races::Terran &&
               target->getType().whatBuilds().first == unit->getType() &&
               !target->isCompleted())
      {
        unit->self->order            = Orders::ConstructingBuilding;
        unit->self->buildUnit        = getUnitID(target);
        target->self->buildUnit      = getUnitID(unit);
        unit->self->isConstructing   = true;
        target->self->isConstructing = true;
      }
      else if ( unit->getType().canAttack() && target->getPlayer() != unit->getPlayer() && !target->getType().isNeutral() )
      {
        unit->self->order = Orders::AttackUnit;
      }
      else if ( unit->getType().canMove() )
      {
        unit->self->order = Orders::Follow;
      }
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      if (!unit->getType().canProduce())
        return;
      unit->self->order          = Orders::RallyPointTile;
      unit->self->rallyPositionX = position.x;
      unit->self->rallyPositionY = position.y;
      unit->self->rallyUnit      = -1;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->getType().canProduce())
        return;
      if ( !target || !target->self->exists )
        return;
      unit->self->order     = Orders::RallyPointUnit;
      unit->self->rallyUnit = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      unit->self->order = Orders::Sieging;
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      if ((frame<=Broodwar->getLatency()   && Broodwar->getLatency()==2) ||
          (frame<=Broodwar->getLatency()+1 && Broodwar->getLatency()>2))
      {
        unit->self->order  = Orders::Stop;
        unit->self->isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      if (savedExtra == -1)
        savedExtra = unit->self->trainingQueueCount;
      if ((frame < Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame <= Broodwar->getLatency() && Broodwar->getLatency() > 2))
      {
        unit->self->trainingQueue[unit->self->trainingQueueCount++] = unitType;
        player->self->minerals -= unitType.mineralPrice();
        player->self->gas      -= unitType.gasPrice();
      }
      if ((frame <= Broodwar->getLatency() && Broodwar->getLatency() == 2) ||
          (frame <= Broodwar->getLatency()+1 && Broodwar->getLatency() > 2))
      {
        if (savedExtra == 0)
        {
          unit->self->remainingTrainTime = unitType.buildTime();  // @TODO: fix to real build time
          player->self->supplyUsed[unitType.getRace()] += unitType.supplyRequired();
        }
      }
      if (frame <= Broodwar->getLatency())
      {
        unit->self->isTraining = true;
        unit->self->isIdle     = false;
        if ( unitType == UnitTypes::Terran_Nuclear_Missile )
          unit->self->secondaryOrder = Orders::Train;
      }
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      unit->self->order = Orders::Unburrowing;
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      unit->self->order  = Orders::Unload;
      unit->self->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::Unload;
      }
      else
      {
        unit->self->order                 = Orders::MoveUnload;
        unit->self->targetPositionX       = position.x;
        unit->self->targetPositionY       = position.y;
        unit->self->orderTargetPositionX  = position.x;
        unit->self->orderTargetPositionY  = position.y;
      }
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      unit->self->order                 = Orders::MoveUnload;
      unit->self->targetPositionX       = position.x;
      unit->self->targetPositionY       = position.y;
      unit->self->orderTargetPositionX  = position.x;
      unit->self->orderTargetPositionY  = position.y;
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      unit->self->order = Orders::Unsieging;
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      unit->self->order   = Orders::Upgrade;
      unit->self->upgrade = upgradeType;
      unit->self->isIdle  = false;
      int level           = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->self->remainingUpgradeTime = upgradeType.upgradeTime(level+1);  // @TODO: Fix to real time
      if (frame < Broodwar->getLatency())
      {
        player->self->minerals -= upgradeType.mineralPrice(level+1);
        player->self->gas      -= upgradeType.gasPrice(level+1);
      }
      player->self->isUpgrading[upgradeType] = true;
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if ( techType == TechTypes::Stim_Packs && unit->self->hitPoints > 10 )
      {
        unit->self->hitPoints -= 10;
        unit->self->stimTimer = 17;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if ( !techType.targetsPosition() )
        return;
      unit->self->order                 = techType.getOrder();;
      unit->self->targetPositionX       = position.x;
      unit->self->targetPositionY       = position.y;
      unit->self->orderTargetPositionX  = position.x;
      unit->self->orderTargetPositionY  = position.y;
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if ( !techType.targetsUnit() )
        return;
      unit->self->order                 = techType.getOrder();
      unit->self->orderTarget           = getUnitID(target);
      unit->self->targetPositionX       = target->getPosition().x;
      unit->self->targetPositionY       = target->getPosition().y;
      unit->self->orderTargetPositionX  = target->getPosition().x;
      unit->self->orderTargetPositionY  = target->getPosition().y;
    }
  }

};
