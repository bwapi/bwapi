#pragma once
#include "Types.h"
#include <string>
class AbilityPrototype;
class Target;
class Unit;
/** 
 * Definition of unit prototype
 */
class UnitPrototype
{
  public :
    UnitPrototype(const std::string& name, 
                  const u16          maxHealthPoints,
                  const u16          maxMana,
                  const u32          abilityFlags);
    ~UnitPrototype();
    const std::string& getName() const;
          u16          getMaxHealthPoints() const;
          u16          getMaxMana() const;
          s32          getAbilityFlags() const;
          bool         canOrder(const AbilityPrototype* const ability, Unit* target) const;
          bool         canOrder(const AbilityPrototype* const ability, BW_Position target) const;
    bool biological; // TODO: implement sometimes
    bool mechanical;
    bool robotic;
  private :
    /** Name of the unit, it is the same as what you see in the game when you click on it. */
    std::string name;
    u16         maxHealthPoints;
    u16         maxMana;
    u32         abilityFlags;

};