#include <BWAPI4/UnitCommand.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <tuple>
#include "../Source/BWAPI/UnitImpl.h"

namespace BWAPI4
{
  UnitCommand::UnitCommand(Unit _unit, UnitCommandType _type, Unit _target, int _x, int _y, int _extra)
    : unit(_unit)
    , type(_type)
    , target(_target)
    , x(_x)
    , y(_y)
    , extra(_extra)
  {}
  UnitCommand::UnitCommand(Unit _unit, UnitCommandType _type)
    : unit(_unit)
    , type(_type)
  {}
  UnitCommand UnitCommand::attack(Unit unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Attack_Move };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::attack(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Attack_Unit };
    c.target = target;
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::build(Unit unit, TilePosition target, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Build };
    c.assignTarget(target);
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::buildAddon(Unit unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Build_Addon };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::train(Unit unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Train };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::morph(Unit unit, UnitType type)
  {
    UnitCommand c{ unit, UnitCommandTypes::Morph };
    c.extra = type;
    return c;
  }
  UnitCommand UnitCommand::research(Unit unit, TechType tech)
  {
    UnitCommand c{ unit, UnitCommandTypes::Research };
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::upgrade(Unit unit, UpgradeType upgrade)
  {
    UnitCommand c{ unit, UnitCommandTypes::Upgrade };
    c.extra = upgrade;
    return c;
  }
  UnitCommand UnitCommand::setRallyPoint(Unit unit, Position target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Set_Rally_Position };
    c.assignTarget(target);
    return c;
  }
  UnitCommand UnitCommand::setRallyPoint(Unit unit, Unit target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Set_Rally_Unit };
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::move(Unit unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Move };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::patrol(Unit unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Patrol };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::holdPosition(Unit unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Hold_Position };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::stop(Unit unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Stop };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::follow(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Follow };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::gather(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Gather };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::returnCargo(Unit unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Return_Cargo };
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::repair(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Repair };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::burrow(Unit unit)
  {
    return UnitCommand { unit, UnitCommandTypes::Burrow };
  }
  UnitCommand UnitCommand::unburrow(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Unburrow };
  }
  UnitCommand UnitCommand::cloak(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cloak };
  }
  UnitCommand UnitCommand::decloak(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Decloak };
  }
  UnitCommand UnitCommand::siege(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Siege };
  }
  UnitCommand UnitCommand::unsiege(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Unsiege };
  }
  UnitCommand UnitCommand::lift(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Lift };
  }
  UnitCommand UnitCommand::land(Unit unit, TilePosition target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Land };
    c.assignTarget(target);
    return c;
  }
  UnitCommand UnitCommand::load(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Load };
    c.target = target;
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::unload(Unit unit, Unit target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload };
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(Unit unit, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload_All };
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(Unit unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Unload_All_Position };
    c.assignTarget(target);
    c.extra  = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::rightClick(Unit unit, Position target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Right_Click_Position };
    c.assignTarget(target);
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::rightClick(Unit unit, Unit target, bool shiftQueueCommand)
  {
    UnitCommand c{ unit, UnitCommandTypes::Right_Click_Unit };
    c.target = target;
    c.extra = shiftQueueCommand ? 1 : 0;
    return c;
  }
  UnitCommand UnitCommand::haltConstruction(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Halt_Construction };
  }
  UnitCommand UnitCommand::cancelConstruction(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Construction };
  }
  UnitCommand UnitCommand::cancelAddon(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Addon };
  }
  UnitCommand UnitCommand::cancelTrain(Unit unit, int slot)
  {
    UnitCommand c{ unit, slot >= 0 ? UnitCommandTypes::Cancel_Train_Slot : UnitCommandTypes::Cancel_Train };
    c.extra = slot;
    return c;
  }
  UnitCommand UnitCommand::cancelMorph(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Morph };
  }
  UnitCommand UnitCommand::cancelResearch(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Research };
  }
  UnitCommand UnitCommand::cancelUpgrade(Unit unit)
  {
    return UnitCommand{ unit, UnitCommandTypes::Cancel_Upgrade };
  }
  UnitCommand UnitCommand::useTech(Unit unit, TechType tech)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech };
    c.extra = tech;
    if ( tech == TechTypes::Burrowing )
      c.type = unit->isBurrowed() ? UnitCommandTypes::Unburrow : UnitCommandTypes::Burrow;
    else if ( tech == TechTypes::Cloaking_Field || tech == TechTypes::Personnel_Cloaking )
      c.type = unit->isCloaked() ? UnitCommandTypes::Decloak : UnitCommandTypes::Cloak;
    else if ( tech == TechTypes::Tank_Siege_Mode )
      c.type = unit->isSieged() ? UnitCommandTypes::Unsiege : UnitCommandTypes::Siege;
    return c;
  }
  UnitCommand UnitCommand::useTech(Unit unit, TechType tech, Position target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech_Position };
    c.assignTarget(target);
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::useTech(Unit unit, TechType tech, Unit target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Use_Tech_Unit };
    c.target = target;
    c.extra = tech;
    return c;
  }
  UnitCommand UnitCommand::placeCOP(Unit unit, TilePosition target)
  {
    UnitCommand c{ unit, UnitCommandTypes::Place_COP };
    c.assignTarget(target);
    return c;
  }
  UnitCommandType UnitCommand::getType() const
  {
    return type;
  }
  Unit UnitCommand::getUnit() const
  {
    return unit;
  }
  Unit UnitCommand::getTarget() const
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
}
