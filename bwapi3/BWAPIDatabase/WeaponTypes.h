#pragma once
#include <string>
#include <map>
#include <set>

#include <BWAPITypes\WeaponType.h>

namespace BWAPI
{
  namespace WeaponTypes
  {
    class WeaponTypeInternal : public WeaponType
    {
      std::set<UnitTypeId> whatUses;
    public:
    };

    extern WeaponTypeInternal weaponTypeData[WeaponTypeIds::count];
    extern std::map<std::string, WeaponTypeId> weaponTypeMap;
    extern std::set< WeaponTypeId > weaponTypeSet;
    extern std::set< WeaponTypeId > specialWeaponTypeSet;
    extern std::set< WeaponTypeId > normalWeaponTypeSet;

    extern void init();
  }
}
