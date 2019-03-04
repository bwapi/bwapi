#include <BWAPI/UnitCommand.h>
#include <BWAPI/Unit.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <tuple>

namespace BWAPI
{
  UnitCommand::UnitCommand(UnitID _unit, UnitCommandType _type, UnitID _target, int _x, int _y, int _extra)
    : unit(_unit)
    , type(_type)
    , target(_target)
    , x(_x)
    , y(_y)
    , extra(_extra)
  {}
  UnitCommand::UnitCommand(UnitID _unit, UnitCommandType _type)
    : unit(_unit)
    , type(_type)
  {}
  UnitCommand UnitCommand::attack(UnitID unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Attack_Move };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::attack(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Attack_Unit };
    c.target = target;
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::build(UnitID unit, TilePosition target, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Build };
    c.assignTarget(target);
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::buildAddon(UnitID unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Build_Addon };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::train(UnitID unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Train };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::morph(UnitID unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Morph };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::research(UnitID unit, TechType tech)
  {
    UnitCommand c{ unit, UnitCommandTypes::Research };
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::upgrade(UnitID unit, UpgradeType upgrade)
  {
    UnitCommand c{ unit, UnitCommandTypes::Upgrade };
    c.extra = upgrade;
    return c;
  }
  UnitCommand UnitCommand::setRallyPoint(UnitID unit, Position target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Set_Rally_Position };
    c.assignTarget(target);
    return c;
  }
  UnitCommand UnitCommand::setRallyPoint(UnitID unit, UnitID target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Set_Rally_Unit };
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::move(UnitID unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Move };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::patrol(UnitID unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Patrol };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::holdPosition(UnitID unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Hold_Position };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::stop(UnitID unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Stop };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::follow(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Follow };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::gather(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Gather };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::returnCargo(UnitID unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Return_Cargo };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::repair(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Repair };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::burrow(UnitID unit)
  {
    return UnitCommand { unit, UnitCommandTypes::Burrow };
  }
  UnitCommand UnitCommand::unburrow(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Unburrow };
  }
  UnitCommand UnitCommand::cloak(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cloak };
  }
  UnitCommand UnitCommand::decloak(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Decloak };
  }
  UnitCommand UnitCommand::siege(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Siege };
  }
  UnitCommand UnitCommand::unsiege(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Unsiege };
  }
  UnitCommand UnitCommand::lift(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Lift };
  }
  UnitCommand UnitCommand::land(UnitID unit, TilePosition target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Land };
    c.assignTarget(target);
    return c;
  }
  UnitCommand UnitCommand::load(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Load };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::unload(UnitID unit, UnitID target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload };
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(UnitID unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload_All };
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(UnitID unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload_All_Position };
    c.assignTarget(target);
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::rightClick(UnitID unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Right_Click_Position };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::rightClick(UnitID unit, UnitID target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Right_Click_Unit };
    c.target = target;
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::haltConstruction(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Halt_Construction };
  }
  UnitCommand UnitCommand::cancelConstruction(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Construction };
  }
  UnitCommand UnitCommand::cancelAddon(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Addon };
  }
  UnitCommand UnitCommand::cancelTrain(UnitID unit, int slot)
  {
    UnitCommand c{ unit, slot >= 0 ? UnitCommandTypes::Cancel_Train_Slot : UnitCommandTypes::Cancel_Train };
    c.extra = slot;
    return c;
  }
  UnitCommand UnitCommand::cancelMorph(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Morph };
  }
  UnitCommand UnitCommand::cancelResearch(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Research };
  }
  UnitCommand UnitCommand::cancelUpgrade(UnitID unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Upgrade };
  }
  UnitCommand UnitCommand::useTech(UnitID unit, TechType tech)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech };
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::useTech(UnitID unit, TechType tech, Position target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech_Position };
    c.assignTarget(target);
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::useTech(UnitID unit, TechType tech, UnitID target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech_Unit };
    c.target = target;
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::placeCOP(UnitID unit, TilePosition target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Place_COP };
    c.assignTarget(target);
    return c;
  }
  UnitCommandType UnitCommand::getType() const
  {
    return type;
  }
  UnitID UnitCommand::getUnit() const
  {
    return unit;
  }
  UnitID UnitCommand::getTarget() const
  {
    return target;
  }
  Position UnitCommand::getTargetPosition() const
  {
    if (type == UnitCommandTypes::Build ||
        type == UnitCommandTypes::Land ||
        type == UnitCommandTypes::Place_COP)
      return Position(TilePosition(x,y));
    return Position(x,y);
  }
  TilePosition UnitCommand::getTargetTilePosition() const
  {
    if (type == UnitCommandTypes::Build ||
        type == UnitCommandTypes::Land ||
        type == UnitCommandTypes::Place_COP)
      return TilePosition(x,y);
    return TilePosition(Position(x,y));
  }
  UnitType UnitCommand::getUnitType() const
  {
    if (type == UnitCommandTypes::Build ||
        type == UnitCommandTypes::Build_Addon ||
        type == UnitCommandTypes::Train ||
        type == UnitCommandTypes::Morph)
      return UnitType(extra);
    return UnitTypes::None;
  }
  TechType UnitCommand::getTechType() const
  {
    if (type == UnitCommandTypes::Research ||
        type == UnitCommandTypes::Use_Tech ||
        type == UnitCommandTypes::Use_Tech_Position ||
        type == UnitCommandTypes::Use_Tech_Unit)
      return TechType(extra);
    return TechTypes::None;
  }
  UpgradeType UnitCommand::getUpgradeType() const
  {
    if (type == UnitCommandTypes::Upgrade)
      return UpgradeType(extra);
    return UpgradeTypes::None;
  }
  int UnitCommand::getSlot() const
  {
    if (type == UnitCommandTypes::Cancel_Train_Slot)
      return extra;
    return -1;
  }
  bool UnitCommand::isQueued() const
  {
    if ( type == UnitCommandTypes::Attack_Move    ||
         type == UnitCommandTypes::Attack_Unit    ||
         type == UnitCommandTypes::Move           ||
         type == UnitCommandTypes::Patrol         ||
         type == UnitCommandTypes::Hold_Position  ||
         type == UnitCommandTypes::Stop           ||
         type == UnitCommandTypes::Follow         ||
         type == UnitCommandTypes::Gather         ||
         type == UnitCommandTypes::Return_Cargo   ||
         type == UnitCommandTypes::Repair         ||
         type == UnitCommandTypes::Load           ||
         type == UnitCommandTypes::Unload_All     ||
         type == UnitCommandTypes::Unload_All_Position  ||
         type == UnitCommandTypes::Right_Click_Position ||
         type == UnitCommandTypes::Right_Click_Unit )
      return !!extra;
    return false;
  }
  bool UnitCommand::operator==(const UnitCommand& other) const
  {
    return std::tie(type, target, x, y, extra) == std::tie(other.type, other.target, other.x, other.y, other.extra);
  }
  bool UnitCommand::operator!=(const UnitCommand& other) const
  {
    return !(*this == other);
  }
  void UnitCommand::assignTarget(Position pos) {
    x = pos.x;
    y = pos.y;
  }
  void UnitCommand::assignTarget(TilePosition tilePos) {
    x = tilePos.x;
    y = tilePos.y;
  }
}
