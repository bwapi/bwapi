#pragma once

#include <BWAPI/UnitCommandType.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class Unit;
  class UnitCommand
  {
    public:
      UnitCommand() : target(NULL), x(0), y(0), extra(0) { type = UnitCommandTypes::None; }
      UnitCommand(Unit* source, UnitCommandType _type, Unit* _target, int _x, int _y, int _extra) : unit(source), type(_type), target(_target), x(_x), y(_y), extra(_extra) {}

      static UnitCommand attackMove(Unit* unit, Position target);
      static UnitCommand attackUnit(Unit* unit, Unit* target);
      static UnitCommand build(Unit* unit, TilePosition target, UnitType type);
      static UnitCommand buildAddon(Unit* unit, UnitType type);
      static UnitCommand train(Unit* unit, UnitType type);
      static UnitCommand morph(Unit* unit, UnitType type);
      static UnitCommand research(Unit* unit, TechType tech);
      static UnitCommand upgrade(Unit* unit, UpgradeType upgrade);
      static UnitCommand setRallyPosition(Unit* unit, Position target);
      static UnitCommand setRallyUnit(Unit* unit, Unit* target);
      static UnitCommand move(Unit* unit, Position target);
      static UnitCommand patrol(Unit* unit, Position target);
      static UnitCommand holdPosition(Unit* unit);
      static UnitCommand stop(Unit* unit);
      static UnitCommand follow(Unit* unit, Unit* target);
      static UnitCommand gather(Unit* unit, Unit* target);
      static UnitCommand returnCargo(Unit* unit);
      static UnitCommand repair(Unit* unit, Unit* target);
      static UnitCommand burrow(Unit* unit);
      static UnitCommand unburrow(Unit* unit);
      static UnitCommand cloak(Unit* unit);
      static UnitCommand decloak(Unit* unit);
      static UnitCommand siege(Unit* unit);
      static UnitCommand unsiege(Unit* unit);
      static UnitCommand lift(Unit* unit);
      static UnitCommand land(Unit* unit, TilePosition target);
      static UnitCommand load(Unit* unit, Unit* target);
      static UnitCommand unload(Unit* unit, Unit* target);
      static UnitCommand unloadAll(Unit* unit);
      static UnitCommand unloadAll(Unit* unit, Position target);
      static UnitCommand rightClick(Unit* unit, Position target);
      static UnitCommand rightClick(Unit* unit, Unit* target);
      static UnitCommand haltConstruction(Unit* unit);
      static UnitCommand cancelConstruction(Unit* unit);
      static UnitCommand cancelAddon(Unit* unit);
      static UnitCommand cancelTrain(Unit* unit, int slot = -2);
      static UnitCommand cancelMorph(Unit* unit);
      static UnitCommand cancelResearch(Unit* unit);
      static UnitCommand cancelUpgrade(Unit* unit);
      static UnitCommand useTech(Unit* unit,TechType tech);
      static UnitCommand useTech(Unit* unit,TechType tech, Position target);
      static UnitCommand useTech(Unit* unit,TechType tech, Unit* target);
      static UnitCommand placeCOP(Unit* unit, TilePosition target);
      Unit* unit;
      UnitCommandType type;
      Unit* target;
      int x;
      int y;
      int extra;
  };
}