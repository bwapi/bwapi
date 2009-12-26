#pragma once

#include <BWAPITypes\TechType.h>
#include <BWAPITypes\TechTypeId.h>

#include <string>
#include <map>
#include <set>


namespace BWAPI
{
  namespace TechTypes
  {
    class TechTypeInternal : public TechType
    {
    public:
      std::set<UnitTypeId> whatUses;
      bool valid;
    };

    extern TechTypeInternal techTypeData[TechTypeIds::count];
    extern std::map<std::string, TechTypeId> techTypeMap;
    extern std::set< TechTypeId > techTypeSet;

    TechTypeId getIdByName(const std::string& name);
    void init();
  }
}
