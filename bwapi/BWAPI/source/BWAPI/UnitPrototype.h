#pragma once

#include <string>
#include "../Types.h"
#include "..//BW//UnitTypes.h"
#include "..//BWAPI//RaceTypes.h"

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
                  const BWAPI::RaceType::Enum race,
                  const u16                   maxShieldPoints,
                  const u16                   maxMana,
                  const u32                   abilityFlags);
    ~UnitPrototype();

    const std::string&    getName() const;
    BW::UnitType::Enum    getUnitID() const;      
    BWAPI::RaceType::Enum getRace() const;      
    s8                    getSupplies() const;      
    u16                   getMaxHealthPoints() const;
    u16                   getMaxShieldPoints() const;
    u16                   getMaxMana() const;
    s32                   getAbilityFlags() const;
    u16                   getMineralPrice() const;
    u16                   getGasPrice() const;
    
    bool canOrder(const AbilityPrototype* const ability, Unit* target) const;
    bool canOrder(const AbilityPrototype* const ability, const BW::Position& target) const;

    bool biological; // @todo implement sometimes
    bool mechanical;
    bool robotic;
  private :
    /** Name of the unit, it is the same as what you see in the game when you click on it. */
    std::string           name;
    u16                   maxHealthPoints;
    u16                   maxShieldPoints;
    u16                   maxMana;
    u32                   abilityFlags;
    BW::UnitType::Enum    unitID;
    BWAPI::RaceType::Enum race;
  };
};
