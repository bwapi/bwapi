#include <BWAPI/UnitCommand.h>
namespace BWAPI
{
  UnitCommand UnitCommand::attackMove(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Attack_Move;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::attackUnit(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Attack_Unit;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::build(Unit* unit, TilePosition target, UnitType type)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Build;
    c.x     = target.x();
    c.y     = target.y();
    c.extra = type.getID();
    return c;
  }
  UnitCommand UnitCommand::buildAddon(Unit* unit, UnitType type)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Build_Addon;
    c.extra = type.getID();
    return c;
  }
  UnitCommand UnitCommand::train(Unit* unit, UnitType type)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Train;
    c.extra = type.getID();
    return c;
  }
  UnitCommand UnitCommand::morph(Unit* unit, UnitType type)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Morph;
    c.extra = type.getID();
    return c;
  }
  UnitCommand UnitCommand::research(Unit* unit, TechType tech)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Research;
    c.extra = tech.getID();
    return c;
  }
  UnitCommand UnitCommand::upgrade(Unit* unit, UpgradeType upgrade)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Upgrade;
    c.extra = upgrade.getID();
    return c;
  }
  UnitCommand UnitCommand::setRallyPosition(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Set_Rally_Position;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::setRallyUnit(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Set_Rally_Unit;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::move(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Move;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::patrol(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Patrol;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::holdPosition(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Hold_Position;
    return c;
  }
  UnitCommand UnitCommand::stop(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Stop;
    return c;
  }
  UnitCommand UnitCommand::follow(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Follow;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::gather(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Gather;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::returnCargo(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Return_Cargo;
    return c;
  }
  UnitCommand UnitCommand::repair(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Repair;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::burrow(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Burrow;
    return c;
  }
  UnitCommand UnitCommand::unburrow(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Unburrow;
    return c;
  }
  UnitCommand UnitCommand::cloak(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cloak;
    return c;
  }
  UnitCommand UnitCommand::decloak(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Decloak;
    return c;
  }
  UnitCommand UnitCommand::siege(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Siege;
    return c;
  }
  UnitCommand UnitCommand::unsiege(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Unsiege;
    return c;
  }
  UnitCommand UnitCommand::lift(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Lift;
    return c;
  }
  UnitCommand UnitCommand::land(Unit* unit, TilePosition target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Land;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::load(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Load;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::unload(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Unload;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Unload_All;
    return c;
  }
  UnitCommand UnitCommand::unloadAll(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Unload_All_Position;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::rightClick(Unit* unit, Position target)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Right_Click_Position;
    target.makeValid();
    c.x    = target.x();
    c.y    = target.y();
    return c;
  }
  UnitCommand UnitCommand::rightClick(Unit* unit, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Right_Click_Unit;
    c.target = target;
    return c;
  }
  UnitCommand UnitCommand::haltConstruction(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Halt_Construction;
    return c;
  }
  UnitCommand UnitCommand::cancelConstruction(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cancel_Construction;
    return c;
  }
  UnitCommand UnitCommand::cancelAddon(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cancel_Addon;
    return c;
  }
  UnitCommand UnitCommand::cancelTrain(Unit* unit, int slot)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = slot >= 0 ? UnitCommandTypes::Cancel_Train_Slot : UnitCommandTypes::Cancel_Train;
    c.extra = slot;
    return c;
  }
  UnitCommand UnitCommand::cancelMorph(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cancel_Morph;
    return c;
  }
  UnitCommand UnitCommand::cancelResearch(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cancel_Research;
    return c;
  }
  UnitCommand UnitCommand::cancelUpgrade(Unit* unit)
  {
    UnitCommand c;
    c.unit = unit;
    c.type = UnitCommandTypes::Cancel_Upgrade;
    return c;
  }
  UnitCommand UnitCommand::useTech(Unit* unit, TechType tech)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Use_Tech;
    c.extra = tech.getID();
    return c;
  }
  UnitCommand UnitCommand::useTech(Unit* unit, TechType tech, Position target)
  {
    UnitCommand c;
    c.unit  = unit;
    c.type  = UnitCommandTypes::Use_Tech_Position;
    c.extra = tech.getID();
    target.makeValid();
    c.x     = target.x();
    c.y     = target.y();
    return c;
  }
  UnitCommand UnitCommand::useTech(Unit* unit,TechType tech, Unit* target)
  {
    UnitCommand c;
    c.unit   = unit;
    c.type   = UnitCommandTypes::Use_Tech_Unit;
    c.extra  = tech.getID();
    c.target = target;
    return c;
  }
}
