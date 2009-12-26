#pragma once

#include <BWAPITypes\DamageType.h>
#include <BWAPITypes\DamageTypeId.h>

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  namespace DamageTypes
  {
    extern DamageType damageTypeData[DamageTypeIds::count];
    extern std::map<std::string, DamageTypeId> damageTypeMap;
    extern std::set< DamageTypeId > damageTypeSet;

    DamageTypeId getIdByName(const std::string& name);
    void init();
  }
}
