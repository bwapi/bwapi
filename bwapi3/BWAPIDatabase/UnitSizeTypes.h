#pragma once
#include <string>
#include <map>
#include <set>

#include <BWAPITypes\UnitSizeType.h>
#include <BWAPITypes\UnitSizeTypeId.h>

namespace BWAPI
{
  namespace UnitSizeTypes
  {
    extern UnitSizeType unitSizeTypeData[UnitSizeTypeIds::count];
    extern std::map<std::string, UnitSizeTypeId> unitSizeTypeMap;
    extern std::set< UnitSizeTypeId > unitSizeTypeSet;

    UnitSizeTypeId getIdByName(std::string& name);
    void init();
  }
}
