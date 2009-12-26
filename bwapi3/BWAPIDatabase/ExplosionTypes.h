#pragma once

#include <BWAPITypes\ExplosionType.h>
#include <BWAPITypes\ExplosionTypeId.h>

#include <string>
#include <set>
#include <map>

namespace BWAPI
{
  class ExplosionTypeInternal : public ExplosionType
  {
  public:
  };
  extern ExplosionTypeInternal explosionTypeName[ExplosionTypeIds::count];
  extern std::map<std::string, ExplosionTypeId> explosionTypeMap;
  extern std::set< ExplosionTypeId > explosionTypeSet;
  namespace ExplosionTypes
  {
    ExplosionType getExplosionType(std::string& name);
    void init();
  }
}
