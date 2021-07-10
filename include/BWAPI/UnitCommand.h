#pragma once

#include <BWAPI/IDs.h>
#include <BWAPI/Position.h>
#include <BWAPI/UnitCommandType.h>

namespace BWAPI
{
  // Forwards
  class UnitType;
  class TechType;
  class UpgradeType;

  class UnitCommand
  {
    public:
      UnitCommand() = default;
      UnitCommand(UnitID _unit, UnitCommandType _type, UnitID _target, int _x, int _y, int _extra);
      UnitCommand(UnitID _unit, UnitCommandType _type);

      static UnitCommand attack(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand attack(UnitID unit, Position target, bool shiftQueueCommand = false);
      static UnitCommand build(UnitID unit, TilePosition target, UnitType type);
      static UnitCommand buildAddon(UnitID unit, UnitType type);
      static UnitCommand train(UnitID unit, UnitType type);
      static UnitCommand morph(UnitID unit, UnitType type);
      static UnitCommand research(UnitID unit, TechType tech);
      static UnitCommand upgrade(UnitID unit, UpgradeType upgrade);
      static UnitCommand setRallyPoint(UnitID unit, Position target);
      static UnitCommand setRallyPoint(UnitID unit, UnitID target);
      static UnitCommand move(UnitID unit, Position target, bool shiftQueueCommand = false);
      static UnitCommand patrol(UnitID unit, Position target, bool shiftQueueCommand = false);
      static UnitCommand holdPosition(UnitID unit, bool shiftQueueCommand = false);
      static UnitCommand stop(UnitID unit, bool shiftQueueCommand = false);
      static UnitCommand follow(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand gather(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand returnCargo(UnitID unit, bool shiftQueueCommand = false);
      static UnitCommand repair(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand burrow(UnitID unit);
      static UnitCommand unburrow(UnitID unit);
      static UnitCommand cloak(UnitID unit);
      static UnitCommand decloak(UnitID unit);
      static UnitCommand siege(UnitID unit);
      static UnitCommand unsiege(UnitID unit);
      static UnitCommand lift(UnitID unit);
      static UnitCommand land(UnitID unit, TilePosition target);
      static UnitCommand load(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand unload(UnitID unit, UnitID target);
      static UnitCommand unloadAll(UnitID unit, bool shiftQueueCommand = false);
      static UnitCommand unloadAll(UnitID unit, Position target, bool shiftQueueCommand = false);
      static UnitCommand rightClick(UnitID unit, Position target, bool shiftQueueCommand = false);
      static UnitCommand rightClick(UnitID unit, UnitID target, bool shiftQueueCommand = false);
      static UnitCommand haltConstruction(UnitID unit);
      static UnitCommand cancelConstruction(UnitID unit);
      static UnitCommand cancelAddon(UnitID unit);
      static UnitCommand cancelTrain(UnitID unit, int slot = -2);
      static UnitCommand cancelMorph(UnitID unit);
      static UnitCommand cancelResearch(UnitID unit);
      static UnitCommand cancelUpgrade(UnitID unit);
      static UnitCommand useTech(UnitID unit,TechType tech);
      static UnitCommand useTech(UnitID unit, TechType tech, Position target);
      static UnitCommand useTech(UnitID unit, TechType tech, UnitID target);
      static UnitCommand placeCOP(UnitID unit, TilePosition target);

      UnitCommandType getType() const;
      UnitID          getUnit() const;
      UnitID          getTarget() const;
      Position        getTargetPosition() const;
      TilePosition    getTargetTilePosition() const;
      UnitType        getUnitType() const;
      TechType        getTechType() const;
      UpgradeType     getUpgradeType() const;
      int             getSlot() const;
      bool            isQueued() const;

      bool operator==(const UnitCommand& other) const;
      bool operator!=(const UnitCommand& other) const;

      void assignTarget(Position pos);
      void assignTarget(TilePosition tilePos);

      UnitID unit = UnitID::None;
      UnitCommandType type = UnitCommandTypes::None;
      UnitID target = UnitID::None;
      int x = Positions::None.x;
      int y = Positions::None.y;
      int extra = 0;
  };
}