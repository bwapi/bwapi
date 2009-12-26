#pragma once

#include <BWAPITypes\DamageType.h>
#include <BWAPITypes\DamageTypeId.h>

#include <string>
#include <set>
namespace BWAPI
{
  namespace DamageTypes
  {
    DamageTypeId getIdByName(const char* name);
    std::set<DamageType>& allDamageTypes();
    void init();
  }
}
