#include "Command.h"

#include "GameImpl.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"

namespace BWAPI4 {

  void Command::insertIntoCommandBuffer(std::vector<std::vector<Command>>& buf)&&
  {
    const auto addToBuffer = [&buf](auto&& command, int frames)
    {
      command.execute(frames == 0);
      if (frames == 0) return;

      if (static_cast<decltype(frames)>(buf.size()) < frames)
      {
        buf.resize(frames); // Will probably never trigger, since we resize
                            // the buffer in applyLatencyCompensation()
                            // but it's better to be safe.
      }

      if (frames > 0)
        buf[frames - 1].push_back(std::forward<Command>(command)); // Forward rvalue ref
    };

    auto orderEvent = makeEvent(EventType::Order);
    auto finishEvent = makeEvent(EventType::Finish);

    switch (command.type) {
      // RLF: Resource event
      // RLF + 1: Order event
      // RLF + 2: Finish event
    case UnitCommandTypes::Cancel_Construction:
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());
      addToBuffer(std::move(orderEvent), BroodwarImpl.getRemainingLatencyFrames() + 1);
      addToBuffer(std::move(finishEvent), BroodwarImpl.getRemainingLatencyFrames() + 2);
      break;

      // RLF: Resource event
      // RLF + 1: Order event
    case UnitCommandTypes::Build_Addon:
    case UnitCommandTypes::Cancel_Addon:
    case UnitCommandTypes::Cancel_Research:
    case UnitCommandTypes::Cancel_Upgrade:
    case UnitCommandTypes::Morph:
    case UnitCommandTypes::Gather:
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());
      addToBuffer(std::move(orderEvent), BroodwarImpl.getRemainingLatencyFrames() + 1);
      break;

      // RLF: Resource event
      // RLF + 1: Order event   (only for building -> building morphs)
      // RLF + 13: Finish event (only for unit     -> unit morphs)
      // RLF + 15: Finish event (only for building -> building morphs)
    case UnitCommandTypes::Cancel_Morph:
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());

      if (auto unit = reinterpret_cast<UnitImpl*>(command.unit);
        unit && unit->getType().isBuilding())
      {
        addToBuffer(std::move(orderEvent), BroodwarImpl.getRemainingLatencyFrames() + 1);
        addToBuffer(std::move(finishEvent), BroodwarImpl.getRemainingLatencyFrames() + 15);
      }
      else
      {
        addToBuffer(std::move(finishEvent), BroodwarImpl.getRemainingLatencyFrames() + 13);
      }
      break;

      // RLF: Resource event
      // RLF + 1: Order event
      // RLF + 3: Finish event
    case UnitCommandTypes::Cancel_Train:
    case UnitCommandTypes::Cancel_Train_Slot:
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());
      addToBuffer(std::move(orderEvent), BroodwarImpl.getRemainingLatencyFrames() + 1);
      addToBuffer(std::move(finishEvent), BroodwarImpl.getRemainingLatencyFrames() + 3);
      break;

      // RLF: Order event
      // RLF + 1: Finish event
    case UnitCommandTypes::Halt_Construction:
      eventType = EventType::Order;
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());
      addToBuffer(finishEvent, BroodwarImpl.getRemainingLatencyFrames() + 1);
      break;

    default:
      addToBuffer(std::move(*this), BroodwarImpl.getRemainingLatencyFrames());
      break;
    }
  }

  Command Command::makeEvent(EventType type)
  {
    Command other{ command };
    other.eventType = type;
    return other;
  }

  int Command::getUnitID(Unit unit)
  {
    if (!unit)
      return -1;
    return unit->getID();
  }

  void Command::execute()
  {
    switch (command.type)
    {
    case UnitCommandTypes::Halt_Construction:
      eventType = EventType::Order;
      [[fallthrough]];
    default:
      execute(BroodwarImpl.getRemainingLatencyFrames() == 0);
      break;
    }
  }

  void Command::execute(const bool isCurrentFrame)
  {
    // Immediately return if latency compensation is disabled or if the command was queued
    if (!BroodwarImpl.isLatComEnabled() || command.isQueued()) return;
    UnitImpl* unit = reinterpret_cast<UnitImpl*>(command.unit);
    UnitImpl* target = reinterpret_cast<UnitImpl*>(command.target);

    if (isCurrentFrame) {
      switch (command.type) // Commands which do things during the current frame
      {
      case UnitCommandTypes::Morph:       // Morph, Build_Addon and Train orders may reserve resources or supply that
      case UnitCommandTypes::Build_Addon: // SC does not take until the next frame to protect bots from overspending.
      case UnitCommandTypes::Train:
        if (eventType == EventType::Resource)
          break;
        return;
      default:
        return;
      }
    }

    // Get the player (usually the unit's owner)
    if (!player)
      player = static_cast<PlayerImpl*>(unit ? unit->getPlayer() : BroodwarImpl.self());

    // Existence test
    if (!unit->self->exists)
      return;

    // Move test
    switch (command.type)
    {
    case UnitCommandTypes::Follow:
    case UnitCommandTypes::Hold_Position:
    case UnitCommandTypes::Move:
    case UnitCommandTypes::Patrol:
    case UnitCommandTypes::Right_Click_Position:
    case UnitCommandTypes::Attack_Move:
      if (!unit->getType().canMove())
        return;
      break;
    default:
      break;
    }

    // Apply command changes
    switch (command.type)
    {
      // RLF
    case UnitCommandTypes::Attack_Move:
      unit->self->order = Orders::AttackMove;
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
      break;

      // RLF
    case UnitCommandTypes::Attack_Unit:
      if (!target || !target->self->exists || !unit->getType().canAttack())
        return;
      unit->self->order = Orders::AttackUnit;
      unit->self->target = getUnitID(target);
      break;

      // RLF
    case UnitCommandTypes::Build:
      unit->self->order = Orders::PlaceBuilding;
      unit->self->isConstructing = true;
      unit->self->isIdle = false;
      unit->self->buildType = command.extra;
      break;

      // For building addons, SC takes minerals on RLF + 1.
      // Latcom will do as with building->building morph and reserve these resources.
      // RLF: Resource event
      // RLF + 1: Order event
    case UnitCommandTypes::Build_Addon: {
      UnitType addonType{ command.extra };
      switch (eventType)
      {
      case EventType::Resource:
        player->self->minerals -= static_cast<int>(addonType.mineralPrice());
        player->self->gas -= static_cast<int>(addonType.gasPrice());

        if (!isCurrentFrame) // We will pretend the building is busy building, this doesn't
        {
          unit->self->isIdle = false;
          unit->self->order = Orders::PlaceAddon;
        }
        break;

      case EventType::Order:
        unit->self->isConstructing = true;
        unit->self->order = Orders::Nothing;
        unit->self->secondaryOrder = Orders::BuildAddon;
        unit->self->buildType = command.extra;
        break;
      }
    }
                                      break;

                                      // RLF
    case UnitCommandTypes::Burrow:
      unit->self->order = Orders::Burrowing;
      break;

      // RLF: Resource event
      // RLF + 1: Order event
    case UnitCommandTypes::Cancel_Addon:
      switch (eventType)
      {
      case EventType::Resource: {
        UnitType addonType{ unit->self->buildType };
        player->self->minerals += static_cast<int>(addonType.mineralPrice() * 0.75);
        player->self->gas += static_cast<int>(addonType.gasPrice() * 0.75);
        unit->self->buildType = UnitTypes::None;
      }
                              break;
      case EventType::Order:
        unit->self->remainingBuildTime = 0;
        unit->self->isConstructing = false;
        unit->self->order = Orders::Nothing;
        unit->self->isIdle = true;
        unit->self->buildUnit = -1;
        break;
      }

      break;

      // RLF: Resource event
      // RLF + 1: Order event
      // RLF + 2: Finish event
    case UnitCommandTypes::Cancel_Construction:
    {
      if (unit->getType().getRace() == Races::Terran)
      {
        if (auto builder = static_cast<UnitImpl*>(BroodwarImpl.getUnit(unit->self->buildUnit));
          builder && builder->exists())
        {
          switch (eventType)
          {
          case EventType::Resource:
            builder->self->buildType = UnitTypes::None;
            break;
          case EventType::Order:
            builder->self->isConstructing = false;
            builder->self->order = Orders::ResetCollision;
            break;
          case EventType::Finish:
            builder->self->order = Orders::PlayerGuard;
            break;
          }
        }
      }

      if (eventType == EventType::Resource)
      {
        unit->self->buildUnit = -1;
        player->self->minerals += static_cast<int>(unit->getType().mineralPrice() * 0.75);
        player->self->gas += static_cast<int>(unit->getType().gasPrice() * 0.75);
        unit->self->remainingBuildTime = 0;
      }

      if (unit->getType().getRace() == Races::Zerg)
      {
        switch (eventType)
        {
        case EventType::Resource:
          unit->self->type = unit->getType().whatBuilds().first;
          unit->self->buildType = UnitTypes::None;
          unit->self->isMorphing = false;
          unit->self->order = Orders::ResetCollision;
          unit->self->isConstructing = false;

          player->self->supplyUsed[unit->getType().getRace()] += unit->getType().supplyRequired();
          break;

        case EventType::Order:
          unit->self->order = Orders::PlayerGuard;
          unit->self->isIdle = true;
          break;
        }
      }

      break;
    }


    // RLF: Resource event
    // RLF + 1: Order event (only for builing -> building morphs)
    // RLF + 13: Finish event (only for unit -> unit morphs)
    // RLF + 15: Finish event (only for building -> building morphs)
    case UnitCommandTypes::Cancel_Morph:
      switch (eventType)
      {
      case EventType::Resource:
      {
        UnitType builtType{ unit->self->buildType };
        UnitType newType{ builtType.whatBuilds().first };

        if (newType.isBuilding())
        {
          player->self->minerals += static_cast<int>(builtType.mineralPrice() * 0.75);
          player->self->gas += static_cast<int>(builtType.gasPrice() * 0.75);
        }
        else
        {
          player->self->minerals += builtType.mineralPrice();
          player->self->gas += builtType.gasPrice();
        }

        if (newType.isBuilding() && newType.producesCreep())
        {
          unit->self->order = Orders::InitCreepGrowth;
        }

        if (unit->self->type != UnitTypes::Zerg_Egg) { // Issue #781
                        // https://github.com/bwapi/bwapi/issues/781
          unit->self->type = newType;
        }

        unit->self->buildType = UnitTypes::None;
        unit->self->isConstructing = false;
        unit->self->isMorphing = false;
        unit->self->isCompleted = true;
        unit->self->remainingBuildTime = 0;
      }

      break;

      case EventType::Order:
        if (unit->getType().isBuilding()) // This event would hopefully not have been created
        {                                 // if this wasn't true (see event note above)
          unit->self->isIdle = true;
          unit->self->order = Orders::Nothing;
          if (unit->self->type == UnitTypes::Zerg_Hatchery || unit->self->type == UnitTypes::Zerg_Lair)
          { // Type should have updated during last event to the cancelled type
            unit->self->secondaryOrder = Orders::SpreadCreep;
          }
        }
        else
        {
          player->self->supplyUsed[unit->getType().getRace()] -=
            unit->getType().supplyRequired() * (1 + static_cast<int>(unit->getType().isTwoUnitsInOneEgg()));

          player->self->supplyUsed[unit->getType().getRace()] += // Could these races be different? Probably not.
                                                                 // Should we handle it?            Definetely.
            unit->getType().whatBuilds().first.supplyRequired() * unit->getType().whatBuilds().second;
          // Note: unit->getType().whatBuilds().second is always 1 but we
          // might as well handle the general case, in case Blizzard
          // all of a sudden allows you to cancel archon morphs
        }

        break;

      case EventType::Finish:
        if (unit->self->type == UnitTypes::Zerg_Hatchery || unit->self->type == UnitTypes::Zerg_Lair)
        {
          unit->self->secondaryOrder = Orders::SpawningLarva;
        }
        else if (!unit->getType().isBuilding())
        {
          unit->self->order = Orders::PlayerGuard;
          unit->self->isCompleted = true;
          unit->self->isConstructing = false;
          unit->self->isIdle = true;
          unit->self->isMorphing = false;
        }
        break;
      }

      break;

      // RLF: Resource event
      // RLF + 1: Order update
    case UnitCommandTypes::Cancel_Research:
    {
      switch (eventType)
      {
      case EventType::Resource:
      {
        TechType techType{ unit->self->tech };
        player->self->minerals += techType.mineralPrice();
        player->self->gas += techType.gasPrice();
        unit->self->remainingResearchTime = 0;
        unit->self->tech = TechTypes::None;
      }
      break;

      case EventType::Order:
        unit->self->order = Orders::Nothing;
        unit->self->isIdle = true;
        break;
      }
    }

    break;

    // RLF: Resource event
    // RLF + 1: Order event
    // RLF + 3: Finish event
    case UnitCommandTypes::Cancel_Train_Slot:
      if (command.extra != 0)
      {
        if (eventType == EventType::Resource)
        {
          UnitType unitType{ unit->self->trainingQueue[command.extra] };
          player->self->minerals += unitType.mineralPrice();
          player->self->gas += unitType.gasPrice();

          // Shift training queue back one slot after the cancelled unit
          for (int i = command.extra; i < 4; ++i)
          {
            unit->self->trainingQueue[i] = unit->self->trainingQueue[i + 1];
          }

          --unit->self->trainingQueueCount;
        }
        break;
      }

      // If we're cancelling slot 0, we fall through to Cancel_Train.
      [[fallthrough]];

      // RLF: Resource event
      // RLF + 1: Order event
      // RLF + 3: Finish event
    case UnitCommandTypes::Cancel_Train: {
      switch (eventType)
      {
      case EventType::Resource:
      {
        UnitType unitType{ unit->self->trainingQueue[unit->self->trainingQueueCount - 1] };
        player->self->minerals += unitType.mineralPrice();
        player->self->gas += unitType.gasPrice();

        unit->self->buildUnit = -1;

        if (unit->self->trainingQueueCount == 1)
        {
          unit->self->isIdle = false;
          unit->self->isTraining = false;
        }
        break;
      }

      case EventType::Order:
      {
        UnitType unitType{ unit->self->trainingQueue[--unit->self->trainingQueueCount] };
        player->self->supplyUsed[unitType.getRace()] -= unitType.supplyRequired();

        if (unit->self->trainingQueueCount == 0)
        {
          unit->self->buildType = UnitTypes::None;
        }
        else
        {
          // Actual time decreases, but we'll let it be the buildTime until latency catches up.
          unit->self->remainingTrainTime =
            static_cast<UnitType>(unit->self->trainingQueue[unit->self->trainingQueueCount - 1]).buildTime();
          unit->self->buildType = unit->self->trainingQueue[unit->self->trainingQueueCount - 1];
        }
      }

      break;

      case EventType::Finish:
        if (unit->self->buildType == UnitTypes::None)
        {
          unit->self->order = Orders::Nothing;
        }
        break;
      }
      break;
    }

                                       // RLF: Resource event
                                       // RLF + 1: Order event
    case UnitCommandTypes::Cancel_Upgrade:
      switch (eventType)
      {
      case EventType::Resource:
      {
        UpgradeType upgradeType = UpgradeType(unit->self->upgrade);
        const int nextLevel = unit->getPlayer()->getUpgradeLevel(upgradeType) + 1;

        player->self->minerals += upgradeType.mineralPrice(nextLevel);
        player->self->gas += upgradeType.gasPrice(nextLevel);

        unit->self->upgrade = UpgradeTypes::None;
        unit->self->remainingUpgradeTime = 0;
      }
      break;

      case EventType::Order:
        unit->self->order = Orders::Nothing;
        unit->self->isIdle = true;
        break;
      }

      break;

      // RLF
    case UnitCommandTypes::Cloak:
      unit->self->order = Orders::Cloak;
      unit->self->energy -= unit->getType().cloakingTech().energyCost();
      break;

      // RLF
    case UnitCommandTypes::Decloak:
      unit->self->order = Orders::Decloak;
      break;

      // RLF
    case UnitCommandTypes::Follow:
      unit->self->order = Orders::Follow;
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      break;

      // RLF
      // RLF + 1: Order event
    case UnitCommandTypes::Gather:
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      unit->self->isGathering = true;

      // @TODO: Fully time and test this order
      if (target->getType().isMineralField())
        unit->self->order = Orders::MoveToMinerals;
      else if (target->getType().isRefinery())
        unit->self->order = Orders::MoveToGas;

      break;

      // RLF: Order event
      // RLF + 1: Finish event
    case UnitCommandTypes::Halt_Construction:
      switch (eventType)
      {
      case EventType::Order:
        if (auto building = reinterpret_cast<UnitImpl*>(BroodwarImpl.getUnit(unit->self->buildUnit));
          building)
        {
          building->self->buildUnit = -1;
        }
        unit->self->buildUnit = -1;
        unit->self->order = Orders::ResetCollision;
        unit->self->isConstructing = false;
        unit->self->buildType = UnitTypes::None;
        break;

      case EventType::Finish:
        unit->self->order = Orders::PlayerGuard;
        unit->self->isIdle = true;
        break;
      }

      break;

      // RLF
    case UnitCommandTypes::Hold_Position:
      unit->self->isMoving = false;
      unit->self->isIdle = false;
      unit->self->order = Orders::HoldPosition;
      break;

      // RLF
    case UnitCommandTypes::Land:
      unit->self->order = Orders::BuildingLand;
      unit->self->isIdle = false;
      break;

      // RLF
    case UnitCommandTypes::Lift:
      unit->self->order = Orders::BuildingLiftOff;
      unit->self->isIdle = false;
      break;

      // RLF
    case UnitCommandTypes::Load:
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::PickupBunker;
        unit->self->target = getUnitID(target);
      }
      else if (unit->getType().spaceProvided())
      {
        unit->self->order = Orders::PickupTransport;
        unit->self->target = getUnitID(target);
      }
      else if (target->getType().spaceProvided())
      {
        unit->self->order = Orders::EnterTransport;
        unit->self->target = getUnitID(target);
      }
      unit->self->isIdle = false;

      break;

      // For morph, SC takes minerals on RLF + 1 if morphing building->building.
      // Latcom will do as with addons and reserve these resources.
      // RLF: Resource event
      // RLF + 1: Order event
    case UnitCommandTypes::Morph:
    {
      UnitType morphType{ command.extra };

      switch (eventType)
      {
      case EventType::Resource:
        if (!isCurrentFrame)
        {
          unit->self->isCompleted = false;
          unit->self->isIdle = false;
          unit->self->isConstructing = true;
          unit->self->isMorphing = true;
          unit->self->buildType = morphType;
        }

        if (unit->getType().isBuilding())
        {
          if (!isCurrentFrame)
          { // Actions that don't happen when we're reserving resources
            unit->self->order = Orders::ZergBuildingMorph;
            unit->self->type = morphType;
          }
          player->self->minerals -= morphType.mineralPrice();
          player->self->gas -= morphType.gasPrice();
        }
        else
        {
          player->self->supplyUsed[morphType.getRace()] += morphType.supplyRequired() *
            (1 + static_cast<int>(morphType.isTwoUnitsInOneEgg())) - unit->getType().supplyRequired();

          if (!isCurrentFrame)
          {
            unit->self->order = Orders::ZergUnitMorph;

            player->self->minerals -= morphType.mineralPrice();
            player->self->gas -= morphType.gasPrice();

            switch (morphType)
            {
            case UnitTypes::Zerg_Lurker_Egg:
              unit->self->type = UnitTypes::Zerg_Lurker_Egg;
              break;

            case UnitTypes::Zerg_Devourer:
            case UnitTypes::Zerg_Guardian:
              unit->self->type = UnitTypes::Zerg_Cocoon;
              break;

            default:
              unit->self->type = UnitTypes::Zerg_Egg;
              break;
            }

            unit->self->trainingQueue[unit->self->trainingQueueCount++] = morphType;
          }
        }
        break;
      case EventType::Order:
        if (unit->getType().isBuilding())
        {
          unit->self->order = Orders::IncompleteBuilding;
        }
        break;
      }
    }

    break;

    // RLF
    case UnitCommandTypes::Move:
      unit->self->order = Orders::Move;
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
      unit->self->isMoving = true;
      unit->self->isIdle = false;
      break;

      // RLF
    case UnitCommandTypes::Patrol:
      unit->self->order = Orders::Patrol;
      unit->self->isIdle = false;
      unit->self->isMoving = true;
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
      break;

      // RLF
    case UnitCommandTypes::Repair:
      if (unit->getType() != UnitTypes::Terran_SCV)
      {
        return;
      }
      unit->self->order = Orders::Repair;
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      break;

      // RLF
    case UnitCommandTypes::Research:
    {
      TechType techType{ command.extra };
      unit->self->order = Orders::ResearchTech;
      unit->self->tech = techType;
      unit->self->isIdle = false;
      unit->self->remainingResearchTime = techType.researchTime();

      player->self->minerals -= techType.mineralPrice();
      player->self->gas -= techType.gasPrice();
      player->self->isResearching[techType] = true;
    }
    break;

    // RLF
    case UnitCommandTypes::Return_Cargo:
      if (!unit->self->carryResourceType)
        return;

      unit->self->order = (unit->isCarryingGas() ? Orders::ReturnGas : Orders::ReturnMinerals);
      unit->self->isGathering = true;
      unit->self->isIdle = false;

      break;

      // RLF
    case UnitCommandTypes::Right_Click_Position:
      unit->self->order = Orders::Move;
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
      unit->self->isMoving = true;
      unit->self->isIdle = false;
      break;

      // RLF
    case UnitCommandTypes::Right_Click_Unit:
      unit->self->target = getUnitID(target);
      unit->self->isIdle = false;
      unit->self->isMoving = true;

      if (unit->getType().isWorker() && target->getType().isMineralField())
      {
        unit->self->isGathering = true;
        unit->self->order = Orders::MoveToMinerals;
      }
      else if (unit->getType().isWorker() && target->getType().isRefinery())
      {
        unit->self->isGathering = true;
        unit->self->order = Orders::MoveToGas;
      }
      else if (unit->getType().isWorker() &&
        target->getType().getRace() == Races::Terran &&
        target->getType().whatBuilds().first == unit->getType() &&
        !target->isCompleted())
      {
        unit->self->order = Orders::ConstructingBuilding;
        unit->self->buildUnit = getUnitID(target);
        target->self->buildUnit = getUnitID(unit);
        unit->self->isConstructing = true;
        target->self->isConstructing = true;
      }
      else if (unit->getType().canAttack() && target->getPlayer() != unit->getPlayer() && !target->getType().isNeutral())
      {
        unit->self->order = Orders::AttackUnit;
      }
      else if (unit->getType().canMove())
      {
        unit->self->order = Orders::Follow;
      }

      break;

      // RLF
    case UnitCommandTypes::Set_Rally_Position:
      if (!unit->getType().canProduce())
        return;

      unit->self->order = Orders::RallyPointTile;
      unit->self->rallyPositionX = command.x;
      unit->self->rallyPositionY = command.y;
      unit->self->rallyUnit = -1;

      break;

      // RLF
    case UnitCommandTypes::Set_Rally_Unit:
      if (!unit->getType().canProduce())
        return;
      if (!target || !target->self->exists)
        return;

      unit->self->order = Orders::RallyPointUnit;
      unit->self->rallyUnit = getUnitID(target);

      break;

      // RLF
    case UnitCommandTypes::Siege:
      unit->self->order = Orders::Sieging;
      break;

      // RLF
    case UnitCommandTypes::Stop:
      unit->self->order = Orders::Stop;
      unit->self->isIdle = true;
      break;

      // With train, the game does not take the supply until RLF + 1.
      // We just pretend that it happens on RLF.
    case UnitCommandTypes::Train:
    {
      UnitType unitType = UnitType(command.extra);

      if (!isCurrentFrame)
      {
        // Happens on RLF, we don't want to duplicate this.
        player->self->minerals -= unitType.mineralPrice();
        player->self->gas -= unitType.gasPrice();
      }

      // Happens on RLF + 1, we want to pretend this happens on RLF.
      unit->self->trainingQueue[unit->self->trainingQueueCount++] = unitType;
      player->self->supplyUsed[unitType.getRace()] += unitType.supplyRequired();

      // Happens on RLF or RLF + 1, doesn't matter if we do twice
      unit->self->isTraining = true;
      unit->self->isIdle = false;
      unit->self->remainingTrainTime = unitType.buildTime();

      if (unitType == UnitTypes::Terran_Nuclear_Missile)
      {
        unit->self->secondaryOrder = Orders::Train;
      }
    }

    break;

    // RLF
    case UnitCommandTypes::Unburrow:
      unit->self->order = Orders::Unburrowing;
      break;

      // RLF
    case UnitCommandTypes::Unload:
      unit->self->order = Orders::Unload;
      unit->self->target = getUnitID(target);
      break;

      // RLF
    case UnitCommandTypes::Unload_All:
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->self->order = Orders::Unload;
      }
      else
      {
        unit->self->order = Orders::MoveUnload;
        unit->self->targetPositionX = command.x;
        unit->self->targetPositionY = command.y;
        unit->self->orderTargetPositionX = command.x;
        unit->self->orderTargetPositionY = command.y;
      }

      break;

      // RLF
    case UnitCommandTypes::Unload_All_Position:
      unit->self->order = Orders::MoveUnload;
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
      break;

      // RLF
    case UnitCommandTypes::Unsiege:
      unit->self->order = Orders::Unsieging;
      break;

      // RLF
    case UnitCommandTypes::Upgrade:
    {
      UpgradeType upgradeType{ command.extra };

      unit->self->order = Orders::Upgrade;
      unit->self->upgrade = upgradeType;
      unit->self->isIdle = false;

      const int level = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->self->remainingUpgradeTime = upgradeType.upgradeTime(level + 1);

      player->self->minerals -= upgradeType.mineralPrice(level + 1);
      player->self->gas -= upgradeType.gasPrice(level + 1);

      player->self->isUpgrading[upgradeType] = true;
    }
    break;

    // RLF
    case UnitCommandTypes::Use_Tech:
      if (static_cast<TechType>(command.extra) == TechTypes::Stim_Packs
        && unit->self->hitPoints > 10)
      {
        unit->self->hitPoints -= 10;
        unit->self->stimTimer = 17;
      }
      break;

      // RLF
    case UnitCommandTypes::Use_Tech_Position:
    {
      TechType techType{ command.extra };

      if (!techType.targetsPosition())
        return;

      unit->self->order = techType.getOrder();
      unit->self->targetPositionX = command.x;
      unit->self->targetPositionY = command.y;
      unit->self->orderTargetPositionX = command.x;
      unit->self->orderTargetPositionY = command.y;
    }

    break;

    // RLF
    case UnitCommandTypes::Use_Tech_Unit:
    {
      TechType techType{ command.extra };

      if (!techType.targetsUnit())
        return;

      unit->self->order = techType.getOrder();
      unit->self->orderTarget = getUnitID(target);

      Position const targetPosition = target->getPosition();

      unit->self->targetPositionX = targetPosition.x;
      unit->self->targetPositionY = targetPosition.y;
      unit->self->orderTargetPositionX = targetPosition.x;
      unit->self->orderTargetPositionY = targetPosition.y;

      break;
    }
    }
  }

}