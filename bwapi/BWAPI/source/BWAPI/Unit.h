#pragma once

#include "../Types.h"
#include "../BW/OrderID.h"
#include "../BW/UnitTypes.h"

namespace BW { class Position; };
namespace BW { struct UnitData; };
namespace BWAPI { class UnitPrototype;  };
namespace BWAPI { class AbilityPrototype;  };
namespace BWAPI { class Player;  };

namespace BWAPI
{
  /** 
  * Interface for broodwar unit, can be used to obtain any information and issue commands.
  */
  class Unit
  {
    public:
      /** 

       * @param bwUnitData         #bwUnitData                          
       * @param bwOriginalUnitData #bwOriginalUnitData
       * @param bwUnitDataLocal    #bwUnitDataLocal
       *
       */
      Unit(BW::UnitData* bwUnitData,
           BW::UnitData* bwOriginalUnitData,
           BW::UnitData* bwUnitDataLocal);
      /** Nothing is deleted as no data are owned.*/
      ~Unit();
      /** Gets #bwUnitData->BW#UnitData#unitID */
      BW::UnitType::Enum getType();
      /** Gets #bwUnitData->BW#UnitData#healthPoints. */
      u16 getHealthPoints() const; 
      /** Gets #bwUnitData->BW#UnitData#healthPointsFraction. */
      u8 getHealthPointsFraction() const;
      /** Gets #bwUnitData->BW#UnitData#shieldPoints. */
      u16 getShieldPoints() const; 
      /** Gets #bwUnitData->BW#UnitData#shieldPointsFraction. */
      u8 getShieldPointsFraction() const;
      /** Gets #bwUnitData->BW#UnitData#position. */
      const BW::Position& getPosition() const;
      /** Gets owner of the unit defined by #bwUnitData->BW#UnitData#playerID. */
      Player* getOwner() const;
      /** Gets prototype of the unit defined by #bwUnitData->BW#UnitData#unitID. */
      const UnitPrototype* const getPrototype() const;
      /** Gets #bwUnitData->BW#UnitData#queueSlot. */
      u8 getBuildQueueSlot();
      /** Gets #bwUnitDataLocal->BW#UnitData#buildQueueSlot - @ref localData */
      u8 getBuildQueueSlotLocal();
      /** Gets distance between this and the specified unit. */
      u16 getDistance(Unit *unit) const; 
      /**< Gets bwUnitData->BW#UnitData#orderID. */
      BW::OrderID::Enum getOrderID() const;
      /**< Gets bwUnitDataLocal->BW#UnitData#orderID - @ref localData */
      BW::OrderID::Enum getOrderIDLocal() const;
      /** Gets #bwUnitData->BW#UnitData#buildQueue */
      BW::UnitType::Enum *getBuildQueue();
      /** Gets #bwUnitDataLocal->BW#UnitData#buildQueue - @ref localData*/
      BW::UnitType::Enum *getBuildQueueLocal();

      bool canOrder(const AbilityPrototype* const ability, Unit* target) const;
      bool canOrder(const AbilityPrototype* const ability, const BW::Position& target) const;
      void order(const AbilityPrototype* const ability, Unit* target);
      void order(const AbilityPrototype* const ability, const BW::Position& target);
      void order(int commandCode, const BW::Position& target);
      /** Gets if the unit is alive, it uses hp > 0 heuristic for now. */
      bool isValid() const;
      /** Gets #bwUnitData */
      BW::UnitData *getRawData();
      /** Gets #bwUnitData (const version that returns const pointer) */
      const BW::UnitData *getRawData() const;
      /** Gets #bwOriginalUnitData */
      BW::UnitData *getOriginalRawData();
      /** Gets #bwOriginalUnit (const version that returns const pointer) */
      const BW::UnitData *getOriginalRawData() const;
      /** Gets #bwUnitDataLocal */
      BW::UnitData *getRawDataLocal();
      /** Gets #bwUnitDataLocal (const version that returns const pointer)*/
      const BW::UnitData *getRawDataLocal() const;
      /** Returns if the unit has empty building queue */
      bool hasEmptyBuildQueue();
      /** Returns if the unit has empty building queue in the local version  - @ref localData*/
      bool hasEmptyBuildQueueLocal();
      /**
       * Order this unit to right click on the specified location. 
       * Note that right click on location will always result in move.
       */
      void orderRightClick(u16 x,u16 y);
      /**
       * Orders this unit to right click on the specified unit.
       * Note that right click on unit can result in lot of commands (attack, gather, follow, set relly point)
       */
      void orderRightClick(Unit *target);
      /** Orders this unit to train (construct) the specified unit. */
      void trainUnit(UnitPrototype *type);
      /** Orders to select this unit (previous selection will be lost. */
      void orderSelect();
    private:
      BW::UnitData* bwUnitData; /**< Pointer to our copy of of unit data table. */
      BW::UnitData* bwUnitDataLocal; /**< Pointer to our local (precomputed) version of unit data table  @ref localData. */ 
      BW::UnitData* bwOriginalUnitData; /**< Pointer to broodwar unit data table. */
      
  };
};

