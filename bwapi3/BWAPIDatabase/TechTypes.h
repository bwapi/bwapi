#pragma once

#include <BWAPITypes\TechTypeId.h>

#include <string>
#include <map>
#include <set>


namespace BWAPI
{
  namespace TechTypes
  {
    TechTypeId getIdByName(const std::string& name);
    std::set<TechTypeId>& allTechTypes();
    void init();
  }
}
