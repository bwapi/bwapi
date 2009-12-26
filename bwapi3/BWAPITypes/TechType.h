#pragma once

#include <BWAPITypes\RaceId.h>
#include <BWAPITypes\UnitTypeId.h>
#include <BWAPITypes\WeaponTypeId.h>

namespace BWAPI
{
  struct TechType
  {
    int id;
    RaceId race;
    int mineralPrice;
    int gasPrice;
    int energyUsed;
    UnitTypeId whatResearches;
    WeaponTypeId weapon;
    const char *name;
    // const std::set<const UnitType*>& whatUses() const;
  };
}
