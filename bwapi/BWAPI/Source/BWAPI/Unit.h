#pragma once

#include <Util/Types.h>

#include <BW/OrderID.h>
#include <BW/UnitType.h>
#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/TilePosition.h>

namespace Util  { class Logger; }
namespace BW    { class Position; };
namespace BW    { struct Unit; };
namespace BWAPI { class UnitPrototype;  };
namespace BWAPI { class AbilityPrototype;  };
namespace BWAPI { class Player;  };

namespace BWAPI
{
  /**
   * Interface for broodwar unit, can be used to obtain any information and
   * issue commands.
   */
  class Unit
  {
    public:
      /** 
       * @param bwUnit         #bwUnit                          
       * @param bwOriginalUnit #bwOriginalUnit
       * @param bwUnitLocal    #bwUnitLocal
       *
       */
      Unit(BW::Unit* bwUnit,
           BW::Unit* bwOriginalUnit,
           BW::Unit* bwUnitLocal,
           u16 index);
      /** Nothing is deleted as no data are owned.*/
      ~Unit();
      /** Gets #bwUnit->BW#Unit#unitID */
      BW::UnitType getType() const;
      /** Gets #bwUnit->BW#Unit#healthPoints. */
      u16 getHealthPoints() const; 
      /** Gets #bwUnit->BW#Unit#shieldPoints. */
      u32 getShieldPoints() const; 
      /** Gets #bwUnit->BW#Unit#position. */
      const BW::Position& getPosition() const;
      /** Gets #bwUnit->BW#Unit#orderTargetUnit. */
      Unit* getTarget();
      /** Gets #bwUnit->BW#Unit#targetUnit. */
      const Unit* getTarget() const;
      /** Gets #bwUnitLocal->BW#Unit#orderTargetUnit. */
      Unit* getTargetLocal();
      /** Gets #bwUnitLocal->BW#Unit#targetUnit. */
      const Unit* getTargetLocal() const;
      /** (const version) Gets #bwUnitLocal->BW#Unit#targetUnit. */
      Unit* getOrderTarget();
      /** Gets #bwUnit->BW#Unit#orderTargetUnit. */
      const Unit* getOrderTarget() const;
      /** (const version) Gets #bwUnitLocal->BW#Unit#orderTargetUnit. */
      Unit* getOrderTargetLocal();
      /**
       * Gets owner of the unit defined by
       * #bwUnitLocal->BW#Unit#orderTargetUnit.
       */
      const Unit* getOrderTargetLocal() const;
       /** Gets #bwUnitLocal->BW#Unit#currentBuildUnit. */
      Unit* getBuildUnit();
      /** Gets #bwUnit->BW#Unit#moveToPos */
      BW::Position getTargetPosition() const;
      /** (const version) Gets #bwUnitLocal->BW#Unit#moveToPos. */
      BW::Position getTargetPositionLocal() const;
      /**
       * (const version) Gets owner of the unit defined by
       * #bwUnit->BW#Unit#playerID.
       */
      Player* getOwner() const;
      /** Gets #bwUnit->BW#Unit#queueSlot. */
      u8 getBuildQueueSlot() const;
      /** Gets #bwUnitLocal->BW#Unit#buildQueueSlot - @ref localData */
      u8 getBuildQueueSlotLocal() const;
      /** Gets distance of unit edges. */
      u16 getDistance(Unit *unit) const; 
      /** Gets distance of unit center and position. */
      u16 getDistance(BW::Position position) const; 
      /** Gets distance of unit centers. */
      u16 getCenterDistance(Unit *unit) const; 
      /** Gets distance between two positions. */
      u16 getDistance(int x1, int y1, int x2, int y2) const; 
      /**< Gets bwUnit->BW#Unit#orderID. */
      BW::OrderID::Enum getOrderID() const;
      /**< Gets bwUnit->BW#Unit#secondaryOrderID. */
      BW::OrderID::Enum getSecondaryOrderID() const;      
      /**< Gets bwUnitLocal->BW#Unit#secondaryOrderID. */
      BW::OrderID::Enum getSecondaryOrderIDLocal() const;      
      /**< Gets bwUnitLocal->BW#Unit#orderID - @ref localData */
      BW::OrderID::Enum getOrderIDLocal() const;
      /* Timer specifiing how long it will take to finish the current order
       * (verified for mining).
       */
      u8 getOrderTimer() const;
      /** Gets #bwUnit->BW#Unit#buildQueue */
      BW::UnitType* getBuildQueue();
      /** Gets #bwUnitLocal->BW#Unit#buildQueue - @ref localData*/
      BW::UnitType* getBuildQueueLocal();
      void order(int commandCode, const BW::Position& target);
      /**
       * Gets if the unit is alive (it exists), it uses hp > 0 heuristic for
       * now.
       */
      bool isValid() const;
      /** Gets if the unit is alive - exists and it's construction is done. */
      bool isReady() const;
      /** Gets if the unit construction is done */
      bool isCompleted() const;
      /** Gets #bwUnit */
      BW::Unit *getRawData();
      /** Gets #bwUnit (const version that returns const pointer) */
      const BW::Unit *getRawData() const;
      /** Gets #bwOriginalUnit */
      BW::Unit *getOriginalRawData();
      /** Gets #bwOriginalUnit (const version that returns const pointer) */
      const BW::Unit *getOriginalRawData() const;
      /** Gets #bwUnitLocal */
      BW::Unit *getRawDataLocal();
      /** Gets #bwUnitLocal (const version that returns const pointer)*/
      const BW::Unit *getRawDataLocal() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue();
      /** Returns if the unit has empty building queue in the local version  - @ref localData*/
      bool hasEmptyBuildQueueLocal();
      /**
       * Order this unit to right click on the specified location. Note that
       * right click on location will always result in move.
       */
      void orderRightClick(BW::Position position);
      /**
       * Orders this unit to right click on the specified unit. Note that right
       * click on unit can result in lot of commands (attack, gather, follow,
       * set relly point)
       */
      void orderRightClick(Unit *target);
      /** Orders this unit to train (construct) the specified unit. */
      void trainUnit(BW::UnitType type);
      /** Orders to build the specified building. */
      void build(BW::TilePosition position, BW::UnitType type);
      /** Orders to build the invent the specified tech. */
      void invent(BW::TechType tech);
      /** Orders to build the invent the specified upgrade. */
      void upgrade(BW::UpgradeType upgrade);      
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();
      static Unit* BWUnitToBWAPIUnit(BW::Unit* unit);
      std::string getName() const;
      Unit* getNext();
      /** Updates pointer to next unit (and recursively updates that unit. */
      void updateNext();
      /** Gets if the current unit mineral (there are 3 Types of minerals) */
      bool isMineral() const;
      /**
       * Gets index of the un it in the unit array. Note that the index is same
       * for the original unit array, BWAPI::Unit array and BWAI::Unit array, so
       * it is good way to compare units of different types and for conversion.
       * @return 0-based index of the unit in the unit array.
       */
      u16 getIndex() const;
    private:
      BW::Unit* bwUnit; /**< Pointer to our copy of of unit data table. */
      BW::Unit* bwUnitLocal; /**< Pointer to our local (precomputed) version of unit data table  @ref localData. */ 
      BW::Unit* bwOriginalUnit; /**< Pointer to broodwar unit data table. */
      Unit *next; /**< Corresponds to bwOriginalUnit->nextUnit, precomputed pointer - to optimize unit listing. */
      u16 index;
  };
};

