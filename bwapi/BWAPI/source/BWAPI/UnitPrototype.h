#pragma once

#include <string>
#include "../Types.h"
#include "..//BW//UnitTypes.h"
#include "..//BW//Offsets.h" /**< @todo remove (just for UNKNOWN_TYPE */
#include "..//BW//Bitmask.h" /**< @todo remove (just for UNKNOWN_TYPE */
#include "..//BW//UnitPrototypeFlags.h" /**< @todo remove (just for UNKNOWN_TYPE */
// Pre-declarations
namespace BW { class Position; };
namespace BWAPI { class AbilityPrototype; };
namespace BWAPI { class Unit; };

namespace BWAPI
{
  /** 
  * Definition of unit prototype
  */
  class UnitPrototype
  {
  public :
    UnitPrototype(const std::string&          name,
                  const BW::UnitType::Enum    unitID,
                  const u16                   maxMana);
    ~UnitPrototype();

    const std::string&    getName() const;
    BW::UnitType::Enum    getUnitID() const;      
    s8                    getSupplies() const;      
    u16                   getMaxHealthPoints() const;
    u16                   getMaxShieldPoints() const;
    u16                   getMaxMana() const;
    u16                   getMineralPrice() const;
    u16                   getGasPrice() const;
    u8                    getArmor() const;
    u16                   getBuildTime() const;
    UNKNOWN_TYPE          getUnknown() const;
    u16                   dimensionLeft() const;
    u16                   dimensionUp() const;
    u16                   dimensionRight() const;
    u16                   dimensionDown() const;
    u16                   getTileWidth() const;
    u16                   getTileHeight() const;
    /** Gets Damage factor of the ground weapon of the unit @todo find out what does it mean. */
    u8                    getDamageFactor() const;
    /** Gets base Damage of the ground weapon of the unit. */
    u16                   getGroundDamage() const;
    bool                  canProduce() const;
    bool                  isZerg() const;
    bool                  isTerran() const;
    bool                  isProtoss() const;
    bool                  isWorker() const;
    bool                  canAttack() const;
    bool                  canMove() const; /**< I use some internal heuristic that every unit from [men]  group
                                            * can move, @todo verify (or load some value specially for that) 
                                            * Note that there will have to be special canMove function for
                                            * BWAPI::Unit that will take unit instance specific things
                                            * like Not borrowed, not statised, not mealstormed, not under construction.
                                            */
    
  private :
    /** Name of the unit, it is the same as what you see in the game when you click on it. */
    std::string           name;
    u16                   maxMana;
    BW::UnitType::Enum    unitID;
    BW::BitMask<BW::UnitPrototypeFlags::Enum> getFlags() const;
  };
};
