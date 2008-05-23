#pragma once

#include <string>
#include "../Types.h"
#include "UnitPrototypeFlags.h"
#include "GroupFlags.h"
#include "Bitmask.h"
#include "UnitID.h"

namespace BW
{
  /** 
  * Definition of unit prototype, it's possible types are listed in #BW#UnitID.
  * The only data this class holds is the enumeration of type u16, so it can be used directly
  * in the mapping of #BW#Unit
  * 
  * Use This class instance instead of the #BW#UnitID enumeration value as most as possible 
  * alghough they are the same thing practicly, but the enumeration doesn't have all these 
  * help functions, note that as there is == nad != operator and conversion constructor, so
  * the enumeration and this class can be compared/assigned freely.
  * 
  */
  class UnitType
  {
  public :
    UnitType(); /**< Default constructor orderId will be BW::OrderID::None. */
    UnitType(const BW::UnitID::Enum& id); /**< Conversion constructor */
    bool operator == (const BW::UnitID::Enum& id);
    bool operator != (const BW::UnitID::Enum& id);
    const char*           getName() const;
    BW::UnitID::Enum      getID() const;      
    s8                    getSupplies() const;      
    u16                   getMaxHealthPoints() const;
    u16                   getMaxShieldPoints() const;
    u16                   getMaxMana() const;
    u16                   getMineralPrice() const;
    u16                   getGasPrice() const;
    u8                    getArmor() const;
    u16                   getBuildTime() const;
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
    bool isValid();
  private :
    BW::UnitID::Enum id;
    BW::BitMask<BW::UnitPrototypeFlags::Enum> getFlags() const;
    BW::BitMask<BW::GroupFlags::Enum> getGroupFlags() const;
  };
};
