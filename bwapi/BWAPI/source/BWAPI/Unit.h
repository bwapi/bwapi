#pragma once

#include "../Types.h"
#include "../BW/OrderID.h"
#include "../BW/UnitData.h"

namespace BW { struct Position; };
namespace BW { struct UnitData; };
namespace BWAPI { class UnitPrototype;  };
namespace BWAPI { class AbilityPrototype;  };
namespace BWAPI { class Player;  };

namespace BWAPI
{
  /** 
  * Interface of broodwar unit, can be used to obtain any information and issue commands.
  */
  class Unit
  {
  public:
    /** Constructor
    * @param bwUnitData pointer to broodwar unit data structure, the class will 
    *                use that object to load information over time.
    */
    Unit(BW::UnitData* bwUnitData,
         BW::UnitData* bwOriginalUnitData );
    ~Unit();
    /**
    * Gets health points of the current unit, note that this value can be lower 
    * (by one) than the displayed value in in broodwar as broodwar doesn't want 
    * to confuse users with 0 hitpoint unit (with non-zero hp fraction).
    * @returns Integral part of hitpoints.
    */
    u16 getHealthPoints() const; 
    /**
    * Gets the fraction of hitpoints 
    * @returns values in interval 0-255 where 256 is equal to one hitpoint.
    */
    u8 getHealthPointsFraction() const;
    /**
    * Gets shield points of the current unit
    * @returns Integral part of shield points.
    */
    u16 getShieldPoints() const; 
    /**
    * Gets the fraction of shield points
    * @returns values in interval 0-255 where 256 is equal to one shield point,
    */
    u8 getShieldPointsFraction() const;
    /**
    * GetsPosition of the unit
    * @returns BW_Position structure describing the position (defined in Types.h)
    */
    const BW::Position& getPosition() const;
    u16 getDistance(Unit *unit) const; /**< Gets distance between this and the specified unit */
    Player* getOwner() const;
    const UnitPrototype* const getPrototype() const;
    bool canOrder(const AbilityPrototype* const ability, Unit* target) const;
    bool canOrder(const AbilityPrototype* const ability, const BW::Position& target) const;
    void order(const AbilityPrototype* const ability, Unit* target);
    void order(const AbilityPrototype* const ability, const BW::Position& target);
    void order(int commandCode, const BW::Position& target);
    bool isValid() const; /**< Gets if the unit is alive, it uses hp > 0 heuristic for now. */
    BW::UnitData *getRawData();
    BW::UnitData *getOriginalRawData();
    BW::OrderID::Enum getOrderID() const;
    bool hasEmptyQueue(void);
    void orderMove(int x,int y, Unit *target);
    void orderSelect();
  private:
    BW::UnitData* bwUnitData; /**< Pointer to broodwar unit data */
    BW::UnitData* bwOriginalUnitData; /**< Pointer to broodwar unit data */

  };
};
