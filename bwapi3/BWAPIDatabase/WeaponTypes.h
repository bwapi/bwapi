#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  std::set<const BWAPI::UnitType*> whatUses;

  namespace WeaponTypes
  {
    extern WeaponType weaponTypeData[WeaponTypeIds::count];
    extern std::map<std::string, WeaponTypeId> weaponTypeMap;
    extern std::set< WeaponTypeId > weaponTypeSet;
    extern std::set< WeaponTypeId > specialWeaponTypeSet;
    extern std::set< WeaponTypeId > normalWeaponTypeSet;
    void init();
  }
}
