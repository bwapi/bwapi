#include <string>
#include <map>
#include <set>
#include <BWAPITypes/UnitSizeTypeId.h>

namespace BWAPI
{
  std::string unitSizeTypeName[UnitSizeTypeIds::count];
  std::map<std::string, UnitSizeTypeId> unitSizeTypeMap;
  std::set< UnitSizeTypeId > unitSizeTypeSet;
  namespace UnitSizeTypes
  {
    void init()
    {
      unitSizeTypeName[UnitSizeTypeIds::Independent] = "Independent";
      unitSizeTypeName[UnitSizeTypeIds::Small] = "Small";
      unitSizeTypeName[UnitSizeTypeIds::Medium] = "Medium";
      unitSizeTypeName[UnitSizeTypeIds::Large] = "Large";
      unitSizeTypeName[UnitSizeTypeIds::None] = "None";

      unitSizeTypeSet.insert(UnitSizeTypeIds::Independent);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Small);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Medium);
      unitSizeTypeSet.insert(UnitSizeTypeIds::Large);
      unitSizeTypeSet.insert(UnitSizeTypeIds::None);

      for(std::set<UnitSizeTypeId>::iterator i = unitSizeTypeSet.begin(); i != unitSizeTypeSet.end(); i++)
      {
        unitSizeTypeMap.insert(std::make_pair(unitSizeTypeName[*i], *i));
      }
    }
  }
  namespace UnitSizeTypes
  {
    UnitSizeTypeId getIdByName(std::string& name)
    {
      std::map<std::string, UnitSizeTypeId>::iterator i = unitSizeTypeMap.find(name);
      if (i == unitSizeTypeMap.end()) return UnitSizeTypeIds::None;
      return (*i).second;
    }
    std::set<UnitSizeTypeId>& allUnitSizeTypes()
    {
      return unitSizeTypeSet;
    }
  }
}

