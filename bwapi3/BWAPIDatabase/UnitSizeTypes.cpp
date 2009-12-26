#include "UnitSizeTypes.h"

#include <BWAPITypes/UnitSizeTypeId.h>

namespace BWAPI
{
  namespace UnitSizeTypes
  {
    UnitSizeType unitSizeTypeData[UnitSizeTypeIds::count];
    std::map<std::string, UnitSizeTypeId> unitSizeTypeMap;
    std::set< UnitSizeTypeId > unitSizeTypeSet;

    void fillUnitSizeType(UnitSizeTypeId id, const char* name)
    {
      UnitSizeType &target = unitSizeTypeData[id];
      target.name = name;
    }

    void init()
    {
      fillUnitSizeType(UnitSizeTypeIds::Independent, "Independent");
      fillUnitSizeType(UnitSizeTypeIds::Small, "Small");
      fillUnitSizeType(UnitSizeTypeIds::Medium, "Medium");
      fillUnitSizeType(UnitSizeTypeIds::Large, "Large");
      fillUnitSizeType(UnitSizeTypeIds::None, "None");

      unitSizeTypeSet.insert(UnitSizeTypeIds::Independent);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Small);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Medium);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Large);
      unitSizeTypeSet.insert(UnitSizeTypeIds::None);

      for(std::set<UnitSizeTypeId>::iterator i = unitSizeTypeSet.begin(); i != unitSizeTypeSet.end(); i++)
      {
        unitSizeTypeMap.insert(std::make_pair(std::string(unitSizeTypeData[*i].name), *i));
      }
    }
    UnitSizeTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, UnitSizeTypeId>::iterator i = unitSizeTypeMap.find(name);
      if (i == unitSizeTypeMap.end()) return UnitSizeTypeIds::None;
      return (*i).second;
    }
  }
}

