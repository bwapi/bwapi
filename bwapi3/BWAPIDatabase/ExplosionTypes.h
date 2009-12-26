#pragma once

#include <BWAPITypes\ExplosionType.h>
#include <BWAPITypes\ExplosionTypeId.h>

#include <string>
#include <set>
#include <map>

namespace BWAPI
{
  namespace ExplosionTypes
  {
    class ExplosionTypeInternal : public ExplosionType
    {
    public:
    };

    extern ExplosionTypeInternal explosionTypeData[ExplosionTypeIds::count];
    extern std::map<std::string, ExplosionTypeId> explosionTypeMap;
    extern std::set< ExplosionTypeId > explosionTypeSet;

    ExplosionTypeId getIdByName(const std::string& name);
    void init();
  }
}
