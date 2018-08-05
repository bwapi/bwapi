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
    CommandTemp(Game &game, const UnitCommand& command);
    void execute(int frame);
  protected:
    virtual ~CommandTemp() {};
  private:
    Game &game;
    static int getUnitID(Unit unit);
    UnitCommand command;
    int savedExtra = -1;
    int savedExtra2 = -1;
    PlayerImpl* player = nullptr;
  };
  template <class UnitImpl, class PlayerImpl>
  CommandTemp<UnitImpl, PlayerImpl>::CommandTemp(Game &game, const UnitCommand& command): game(game), command(command)
  {}
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
    if ( !game.isLatComEnabled() || command.isQueued() ) return;
    UnitImpl* unit   = static_cast<UnitImpl*>(command.unit);
    UnitImpl* target = static_cast<UnitImpl*>(command.target);

    Position     position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType     unitType(command.extra);
    UpgradeType  upgradeType(command.extra);
    TechType     techType(command.extra);

    // Get the player (usually the unit's owner)
    if ( !player )
      player = static_cast<PlayerImpl*>(unit ? unit->getPlayer() : game.self());

    // Latency test
    if (frame > game.getLatencyFrames() &&
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
      if ( !unit->data.exists )
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
      unit->data.order           = Orders::AttackMove;
      unit->data.targetPositionX = position.x;
      unit->data.targetPositionY = position.y;
      unit->data.orderTargetPositionX = position.x;
      unit->data.orderTargetPositionY = position.y;
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if ( !target || !target->data.exists)
        return;
      if (!unit->getType().canAttack())
        return;
      unit->data.order = Orders::AttackUnit;
      unit->data.target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      unit->data.order = Orders::PlaceBuilding;
      unit->data.isConstructing = true;
      unit->data.isIdle         = false;
      unit->data.buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      unit->data.secondaryOrder = Orders::BuildAddon;
      unit->data.isConstructing = true;
      unit->data.isIdle         = false;
      unit->data.buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      unit->data.order = Orders::Burrowing;
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.buildType;
      unitType = UnitType(savedExtra);
      if (frame < game.getLatencyFrames())
      {
        player->data.minerals += (int)(unitType.mineralPrice() * 0.75);
        player->data.gas      += (int)(unitType.gasPrice()     * 0.75);
      }
      unit->data.remainingBuildTime = 0;
      unit->data.isConstructing     = false;
      unit->data.order              = Orders::Nothing;
      unit->data.isIdle             = true;
      unit->data.buildType          = UnitTypes::None;
      unit->data.buildUnit          = -1;
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.type;
      if (savedExtra2 == -1)
        savedExtra2 = unit->data.buildUnit;
      unitType = UnitType(savedExtra);
      if ((frame > game.getLatencyFrames() + 1 && game.getLatencyFrames() == 2) || 
          (frame > game.getLatencyFrames() + 2 && game.getLatencyFrames() > 2))
        return;
      if (unitType.getRace() == Races::Terran)
      {
        UnitImpl* builder = static_cast<UnitImpl*>( game.getUnit(savedExtra2) );
        if ( builder && builder->exists())
        {
          builder->data.buildUnit      = -1;
          builder->data.buildType      = UnitTypes::None;
          builder->data.isConstructing = false;
          builder->data.order          = Orders::ResetCollision;
        }
      }
      if (frame > game.getLatencyFrames())
        return;
      if (!unit->data.exists)
        return;
      unit->data.buildUnit = -1;
      if ((frame < game.getLatencyFrames() && game.getLatencyFrames()==2) ||
          (frame <=game.getLatencyFrames() && game.getLatencyFrames()>2))
      {
        player->data.minerals += (int)(unitType.mineralPrice() * 0.75);
        player->data.gas      += (int)(unitType.gasPrice()     * 0.75);
      }
      unit->data.remainingBuildTime = 0;
      unit->data.isConstructing     = false;
      if (unitType.getRace() == Races::Zerg)
      {
        unit->data.type       = unitType.whatBuilds().first;
        unit->data.buildType  = UnitTypes::None;
        unit->data.isMorphing = false;
        unit->data.isIdle     = true;

        UnitType whatBuilds = unitType.whatBuilds().first;
        if (frame < game.getLatencyFrames())
          player->data.supplyUsed[unitType.getRace()] += whatBuilds.supplyRequired();

        if (whatBuilds.isBuilding())
        {
          unit->data.order = Orders::Nothing;
        }
        else
        {
          unit->data.order = Orders::ResetCollision;
        }
      }
      else
      {
        unit->data.order       = Orders::Die;
        unit->data.isCompleted = false;
        unit->data.isIdle      = false;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.buildType;
      unitType = UnitType(savedExtra);
      if (frame > game.getLatencyFrames() + 12)
        return;
      if (frame < game.getLatencyFrames())
      {
        if (unitType.whatBuilds().first.isBuilding())
        {
          player->data.minerals += (int)(unitType.mineralPrice()*0.75);
          player->data.gas      += (int)(unitType.gasPrice()*0.75);
        }
        else
        {
          player->data.minerals += unitType.mineralPrice();
          player->data.gas      += unitType.gasPrice();
        }
      }
      if (frame<=game.getLatencyFrames())
      {
        if (unitType.isTwoUnitsInOneEgg())
          player->data.supplyUsed[Races::Zerg] -= unitType.supplyRequired() * 2 - unitType.whatBuilds().first.supplyRequired();
        else
          player->data.supplyUsed[Races::Zerg] -= unitType.supplyRequired() - unitType.whatBuilds().first.supplyRequired();
      }
      unit->data.buildType          = UnitTypes::None;
      unit->data.remainingBuildTime = 0;
      unit->data.isMorphing         = false;
      unit->data.isConstructing     = false;
      unit->data.isCompleted        = true;
      unit->data.isIdle             = true;
      unit->data.type               = unitType.whatBuilds().first;
      if (unitType.whatBuilds().first.isBuilding())
        unit->data.order = Orders::Nothing;
      else
        unit->data.order = Orders::PlayerGuard;
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.tech;
      techType = TechType(savedExtra);

      if (!unit->data.exists)
        return;
      unit->data.order                 = Orders::Nothing;
      unit->data.tech                  = TechTypes::None;
      unit->data.isIdle                = true;
      unit->data.remainingResearchTime = 0;
      if (frame < game.getLatencyFrames())
      {
        player->data.minerals += techType.mineralPrice();
        player->data.gas      += techType.gasPrice();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.trainingQueue[unit->data.trainingQueueCount - 1];
      if (savedExtra2 == -1)
        savedExtra2 = unit->data.buildUnit;
      if ((frame < game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
          (frame <= game.getLatencyFrames()+1 && game.getLatencyFrames() > 2))
      {
        unit->data.trainingQueueCount--;
        if (unit->data.trainingQueueCount < 0)
          unit->data.trainingQueueCount = 0;
        player->data.minerals += UnitType(savedExtra).mineralPrice();
        player->data.gas      += UnitType(savedExtra).gasPrice();
      }
      if (unit->data.trainingQueueCount == 0)
      {
        unit->data.buildUnit          = -1;
        unit->data.isTraining         = false;
        unit->data.remainingTrainTime = 0;
        unit->data.isIdle             = true;
        player->data.supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (frame > game.getLatencyFrames() + 2)
        return;
      if (savedExtra == -1)
        savedExtra = unit->data.trainingQueue[command.extra];
      if ((frame < game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
          (frame <= game.getLatencyFrames()+1 && game.getLatencyFrames() > 2))
      {
        for(int i = command.extra; i < 4; ++i)
          unit->data.trainingQueue[i] = unit->data.trainingQueue[i+1];
        unit->data.trainingQueueCount--;
        if (unit->data.trainingQueueCount < 0)
          unit->data.trainingQueueCount = 0;
        player->data.minerals += UnitType(savedExtra).mineralPrice();
        player->data.gas      += UnitType(savedExtra).gasPrice();
      }
      if (command.extra == 0)
      {
        unit->data.buildUnit = -1;
        if ((frame < game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
            (frame <= game.getLatencyFrames()-1 && game.getLatencyFrames() > 2))
        {
          player->data.supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
        }

        if (unit->data.trainingQueueCount == 0)
        {
          unit->data.isTraining = false;
          unit->data.isIdle     = true;
        }
        else
        {
          unit->data.remainingTrainTime = UnitType(unit->data.trainingQueue[0]).buildTime();  // @TODO: fix to real build time
          player->data.supplyUsed[unit->getType().getRace()] += UnitType(unit->data.trainingQueue[0]).supplyRequired();
          if ((frame == game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
              (frame == game.getLatencyFrames()+1 && game.getLatencyFrames() > 2) )
          {
            player->data.supplyUsed[unit->getType().getRace()] -= UnitType(savedExtra).supplyRequired();
          }
        }
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.upgrade;
      upgradeType = UpgradeType(savedExtra);

      if (!unit->data.exists)
        return;

      unit->data.order                = Orders::Nothing;
      int level                        = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->data.upgrade              = UpgradeTypes::None;
      unit->data.isIdle               = true;
      unit->data.remainingUpgradeTime = 0;

      if (frame < game.getLatencyFrames())
      {
        player->data.minerals += upgradeType.mineralPrice(level+1);
        player->data.gas      += upgradeType.gasPrice(level+1);
      }
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      unit->data.order = Orders::Cloak;
      if (frame < game.getLatencyFrames())
        unit->data.energy -= unit->getType().cloakingTech().energyCost();
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      unit->data.order = Orders::Decloak;
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      unit->data.order    = Orders::Follow;
      unit->data.target   = getUnitID(target);
      unit->data.isIdle   = false;
      unit->data.isMoving = true;
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      if ((frame<=game.getLatencyFrames()   && game.getLatencyFrames()==2) ||
          (frame<=game.getLatencyFrames()+1 && game.getLatencyFrames()>2))
      {
        unit->data.target      = getUnitID(target);
        unit->data.isIdle      = false;
        unit->data.isMoving    = true;
        unit->data.isGathering = true;
        if ( target->getType().isMineralField() )
          unit->data.order = Orders::MoveToMinerals;
        else if ( target->getType().isRefinery() )
          unit->data.order = Orders::MoveToGas;
      }
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.buildUnit;
      if (frame > game.getLatencyFrames())
        return;
      UnitImpl* buildUnit = static_cast<UnitImpl*>( game.getUnit(savedExtra) );
      if ( buildUnit )
        buildUnit->data.buildUnit = -1;

      unit->data.buildUnit      = -1;
      unit->data.buildType      = UnitTypes::None;
      unit->data.order          = Orders::ResetCollision;
      unit->data.isConstructing = false;
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      unit->data.isMoving = false;
      unit->data.isIdle   = false;
      unit->data.order    = Orders::HoldPosition;
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      unit->data.order  = Orders::BuildingLand;
      unit->data.isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      unit->data.order  = Orders::BuildingLiftOff;
      unit->data.isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->data.order  = Orders::PickupBunker;
        unit->data.target = getUnitID(target);
      }
      else if ( unit->getType().spaceProvided() )
      {
        unit->data.order  = Orders::PickupTransport;
        unit->data.target = getUnitID(target);
      }
      else if ( target->getType().spaceProvided() )
      {
        unit->data.order  = Orders::EnterTransport;
        unit->data.target = getUnitID(target);
      }
      unit->data.isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (frame > game.getLatencyFrames()+1)
        return;
      unit->data.isMorphing     = true;
      unit->data.isConstructing = true;
      unit->data.isCompleted    = false;
      unit->data.isIdle         = false;
      unit->data.buildType      = unitType;
      if (unit->data.remainingBuildTime < 50)
        unit->data.remainingBuildTime = unitType.buildTime();  // @TODO: Fix to real build time
      if (frame > game.getLatencyFrames())
        return;
      if (unitType.isBuilding())
      {
        unit->data.order       = Orders::ZergBuildingMorph;
        player->data.minerals -= unitType.mineralPrice();
        player->data.gas      -= unitType.gasPrice();
        unit->data.type        = unitType;
      }
      else
      {
        unit->data.order = Orders::ZergUnitMorph;
        if (frame < game.getLatencyFrames())
        {
          player->data.minerals -= unitType.mineralPrice();
          player->data.gas      -= unitType.gasPrice();
        }
        if (unitType.isTwoUnitsInOneEgg())
          player->data.supplyUsed[Races::Zerg] += unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
        else
          player->data.supplyUsed[Races::Zerg] += unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

        if (unitType == UnitTypes::Zerg_Lurker)
          unit->data.type = UnitTypes::Zerg_Lurker_Egg;
        else if (unitType == UnitTypes::Zerg_Devourer ||
                 unitType == UnitTypes::Zerg_Guardian)
          unit->data.type = UnitTypes::Zerg_Cocoon;
        else
          unit->data.type = UnitTypes::Zerg_Egg;
      }
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      unit->data.order                 = Orders::Move;
      unit->data.targetPositionX       = position.x;
      unit->data.targetPositionY       = position.y;
      unit->data.orderTargetPositionX  = position.x;
      unit->data.orderTargetPositionY  = position.y;
      unit->data.isMoving              = true;
      unit->data.isIdle                = false;
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      unit->data.order           = Orders::Patrol;
      unit->data.isIdle          = false;
      unit->data.isMoving        = true;
      unit->data.targetPositionX = position.x;
      unit->data.targetPositionY = position.y;
      unit->data.orderTargetPositionX = position.x;
      unit->data.orderTargetPositionY = position.y;
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      if (unit->getType() != UnitTypes::Terran_SCV)
        return;
      unit->data.order  = Orders::Repair;
      unit->data.target = getUnitID(target);
      unit->data.isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      unit->data.order                 = Orders::ResearchTech;
      unit->data.tech                  = techType;
      unit->data.isIdle                = false;
      unit->data.remainingResearchTime = techType.researchTime();  // @TODO: Fix to real time
      if (frame < game.getLatencyFrames())
      {
        player->data.minerals -= techType.mineralPrice();
        player->data.gas      -= techType.gasPrice();
      }
      player->data.isResearching[techType] = true;
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (unit->data.carryResourceType == 0)
        return;
      if ((frame<=game.getLatencyFrames()   && game.getLatencyFrames()==2) ||
          (frame<=game.getLatencyFrames()+1 && game.getLatencyFrames()>2))
      {
        if (unit->isCarryingGas())
          unit->data.order = Orders::ReturnGas;
        else
          unit->data.order = Orders::ReturnMinerals;
        unit->data.isGathering = true;
        unit->data.isIdle      = false;
      }
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if ((frame<=game.getLatencyFrames()   && game.getLatencyFrames()==2) ||
          (frame<=game.getLatencyFrames()+1 && game.getLatencyFrames()>2))
      {
        unit->data.order           = Orders::Move;
        unit->data.targetPositionX = position.x;
        unit->data.targetPositionY = position.y;
        unit->data.orderTargetPositionX = position.x;
        unit->data.orderTargetPositionY = position.y;
        unit->data.isMoving        = true;
        unit->data.isIdle          = false;
      }
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      unit->data.target   = getUnitID(target);
      unit->data.isIdle   = false;
      unit->data.isMoving = true;
      if (unit->getType().isWorker() && target->getType().isMineralField() )
      {
        unit->data.isGathering = true;
        unit->data.order       = Orders::MoveToMinerals;
      }
      else if (unit->getType().isWorker() && target->getType().isRefinery() )
      {
        unit->data.isGathering = true;
        unit->data.order       = Orders::MoveToGas;
      }
      else if (unit->getType().isWorker() &&
               target->getType().getRace() == Races::Terran &&
               target->getType().whatBuilds().first == unit->getType() &&
               !target->isCompleted())
      {
        unit->data.order            = Orders::ConstructingBuilding;
        unit->data.buildUnit        = getUnitID(target);
        target->data.buildUnit      = getUnitID(unit);
        unit->data.isConstructing   = true;
        target->data.isConstructing = true;
      }
      else if ( unit->getType().canAttack() && target->getPlayer() != unit->getPlayer() && !target->getType().isNeutral() )
      {
        unit->data.order = Orders::AttackUnit;
      }
      else if ( unit->getType().canMove() )
      {
        unit->data.order = Orders::Follow;
      }
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      if (!unit->getType().canProduce())
        return;
      unit->data.order          = Orders::RallyPointTile;
      unit->data.rallyPositionX = position.x;
      unit->data.rallyPositionY = position.y;
      unit->data.rallyUnit      = -1;
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->getType().canProduce())
        return;
      if ( !target || !target->data.exists )
        return;
      unit->data.order     = Orders::RallyPointUnit;
      unit->data.rallyUnit = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      unit->data.order = Orders::Sieging;
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      if ((frame<=game.getLatencyFrames()   && game.getLatencyFrames()==2) ||
          (frame<=game.getLatencyFrames()+1 && game.getLatencyFrames()>2))
      {
        unit->data.order  = Orders::Stop;
        unit->data.isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      if (savedExtra == -1)
        savedExtra = unit->data.trainingQueueCount;
      if ((frame < game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
          (frame <= game.getLatencyFrames() && game.getLatencyFrames() > 2))
      {
        unit->data.trainingQueue[unit->data.trainingQueueCount++] = unitType;
        player->data.minerals -= unitType.mineralPrice();
        player->data.gas      -= unitType.gasPrice();
      }
      if ((frame <= game.getLatencyFrames() && game.getLatencyFrames() == 2) ||
          (frame <= game.getLatencyFrames()+1 && game.getLatencyFrames() > 2))
      {
        if (savedExtra == 0)
        {
          unit->data.remainingTrainTime = unitType.buildTime();  // @TODO: fix to real build time
          player->data.supplyUsed[unitType.getRace()] += unitType.supplyRequired();
        }
      }
      if (frame <= game.getLatencyFrames())
      {
        unit->data.isTraining = true;
        unit->data.isIdle     = false;
        if ( unitType == UnitTypes::Terran_Nuclear_Missile )
          unit->data.secondaryOrder = Orders::Train;
      }
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      unit->data.order = Orders::Unburrowing;
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      unit->data.order  = Orders::Unload;
      unit->data.target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->data.order = Orders::Unload;
      }
      else
      {
        unit->data.order                 = Orders::MoveUnload;
        unit->data.targetPositionX       = position.x;
        unit->data.targetPositionY       = position.y;
        unit->data.orderTargetPositionX  = position.x;
        unit->data.orderTargetPositionY  = position.y;
      }
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      unit->data.order                 = Orders::MoveUnload;
      unit->data.targetPositionX       = position.x;
      unit->data.targetPositionY       = position.y;
      unit->data.orderTargetPositionX  = position.x;
      unit->data.orderTargetPositionY  = position.y;
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      unit->data.order = Orders::Unsieging;
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      unit->data.order   = Orders::Upgrade;
      unit->data.upgrade = upgradeType;
      unit->data.isIdle  = false;
      int level           = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->data.remainingUpgradeTime = upgradeType.upgradeTime(level+1);  // @TODO: Fix to real time
      if (frame < game.getLatencyFrames())
      {
        player->data.minerals -= upgradeType.mineralPrice(level+1);
        player->data.gas      -= upgradeType.gasPrice(level+1);
      }
      player->data.isUpgrading[upgradeType] = true;
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if ( techType == TechTypes::Stim_Packs && unit->data.hitPoints > 10 )
      {
        unit->data.hitPoints -= 10;
        unit->data.stimTimer = 17;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if ( !techType.targetsPosition() )
        return;
      unit->data.order                 = techType.getOrder();;
      unit->data.targetPositionX       = position.x;
      unit->data.targetPositionY       = position.y;
      unit->data.orderTargetPositionX  = position.x;
      unit->data.orderTargetPositionY  = position.y;
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if ( !techType.targetsUnit() )
        return;
      unit->data.order                 = techType.getOrder();
      unit->data.orderTarget           = getUnitID(target);
      unit->data.targetPositionX       = target->getPosition().x;
      unit->data.targetPositionY       = target->getPosition().y;
      unit->data.orderTargetPositionX  = target->getPosition().x;
      unit->data.orderTargetPositionY  = target->getPosition().y;
    }
  }

};
