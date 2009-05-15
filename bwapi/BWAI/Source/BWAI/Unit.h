#pragma once

#include <BW/OrderID.h>
#include <BW/UnitType.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/TilePosition.h>
#include <list>

namespace BWAPI  { class Unit; }
namespace BWAI  { class Task; }
namespace BWAI  { class Expansion; }
namespace BWAI  { class Player; }

namespace BWAI
{
  /** 
   * Proxy of BWAPI unit type with included information that uses BWAI
   */
  class Unit
  {
    public :
      Unit(BWAPI::Unit *unit);
      Expansion* expansion;
      static Unit* BWAPIUnitToBWAIUnit(BWAPI::Unit* unit);
      /** 
       * Gets #bwUnit->BW#Unit#targetUnit. 
       * This is similar to BWAPI#Unit#getTarget, it just get's the unit from ai scope
       */
      BW::UnitType getType() const;
      int getHealthPoints() const; 
      int getShieldPoints() const; 
      const BW::Position& getPosition() const;
      BW::TilePosition getTilePosition() const;
      Unit* getTarget() const;
      Unit* getOrderTarget() const;
      Unit* getBuildUnit() const;
      Unit* getChild() const;
      BW::Position getTargetPosition() const;
      Player* getOwner() const;
      std::string getName() const;
      Unit* getNext() const;
      bool isMineral() const;
      bool isCompleted() const;
      bool isLifted() const;
      bool isSelected() const;
      bool isIdle() const;

      int getBuildQueueSlot() const;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType* getBuildQueue() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue() const;
      /** Returns if the unit has full building queue */
      bool hasFullBuildQueue() const;
      int getRemainingBuildTime() const;
      int getOrderTimer() const;

      /** Gets distance of unit edges. */
      int getDistance(Unit *unit) const;
      /** Gets distance of unit center and position. */
      int getDistance(BW::Position position) const;
      /** Gets distance of unit centers. */
      int getCenterDistance(Unit *unit) const;
      /**< Gets bwUnit->BW#Unit#orderID. */
      BW::OrderID::Enum getOrderID() const;
      /**< Gets bwUnit->BW#Unit#secondaryOrderID. */
      BW::OrderID::Enum getSecondaryOrderID() const;
      int getIndex() const;

      void orderAttackLocation(BW::Position position, int OrderID);
      void orderRightClick(BW::Position position);
      void orderRightClick(Unit *target);
      void trainUnit(BW::UnitType type);
      void build(BW::TilePosition position, BW::UnitType type);
      void invent(BW::TechType tech);
      void upgrade(BW::UpgradeType upgrade);

      int lastFrameSpam;
      
      Task* getTask() const;
      void freeFromTask();
      void clearTask();
      void setTask(Task* task);
      std::list<Unit*>::iterator taskListIterator;
      BWAPI::Unit* getUnit();
    private :
      Task* task;
      BWAPI::Unit* unit;
  };
}