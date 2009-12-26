#pragma once
#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\UnitTypeId.h>

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  class UnitTypeInternal : public UnitType
  {
  public:
    std::map< UnitTypeId, int > requiredUnits;
    std::set< TechTypeId > abilities;
    std::set< UpgradeTypeId > upgrades;
  };

  extern UnitTypeInternal unitTypeData[UnitTypeIds::count];
  extern std::map<std::string, UnitTypeId> unitTypeMap;
  extern std::set< UnitTypeId > unitTypeSet;
  namespace UnitTypes
  {
    UnitTypeId getIdByName(std::string& name);
    std::set<UnitTypeId>& allUnitTypes();
    void init();
  }
}
