#pragma once

#include <BWAPI.h>
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
      BWAPI::UnitType getType() const;
      int getHealthPoints() const; 
      int getShieldPoints() const; 
      int getEnergyPoints() const; 
      int getKillCount() const; 
      int groundWeaponCooldown() const;
      int airWeaponCooldown() const;
      int spellCooldown() const;
      const BWAPI::Position getPosition() const;
      const BWAPI::TilePosition getTilePosition() const;
      Unit* getTarget() const;
      Unit* getOrderTarget() const;
      Unit* getBuildUnit() const;
      Unit* getChild() const;
      BWAPI::Position getTargetPosition() const;
      Player* getOwner() const;
      std::string getName() const;

      bool isMineral() const;
      bool isCompleted() const;
      bool isLifted() const;
      bool isSelected() const;
      bool isIdle() const;
      bool isMoving() const;
      bool isAccelerating() const;
      bool isBraking() const;
      bool isStartingAttack() const;
      bool isBlind() const;
      bool isBeingHealed() const;
      bool isUnderStorm() const;

      bool isTraining() const;
      std::list<BWAPI::UnitType > getTrainingQueue() const;
      int getRemainingBuildTime() const;
      int getOrderTimer() const;

      /** Gets distance of unit edges. */
      int getDistance(Unit *unit) const;
      /** Gets distance of unit center and position. */
      int getDistance(BWAPI::Position position) const;
      /** Gets distance of unit centers. */
      int getCenterDistance(Unit *unit) const;
      /**< Gets bwUnit->BW#Unit#orderID. */
      BWAPI::Order getOrder() const;
      /**< Gets bwUnit->BW#Unit#secondaryOrderID. */
      BWAPI::Order getSecondaryOrder() const;

      void orderAttackMove(BWAPI::Position position);
      void orderRightClick(BWAPI::Position position);
      void orderRightClick(Unit *target);
      void orderPatrol(BWAPI::Position position);
      void repair(BWAPI::Unit *target);
      void morph(BWAPI::UnitType type);
      void stop();
      void holdPosition();
      void useTech(BWAPI::TechType tech);
      
      void trainUnit(BWAPI::UnitType type);
      void build(BWAPI::TilePosition position, BWAPI::UnitType type);
      void research(BWAPI::TechType tech);
      void upgrade(BWAPI::UpgradeType upgrade);

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