#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  namespace UnitSizeTypes
  {
    UnitSizeTypeId getIdByName(std::string& name);
    std::set<UnitSizeType>& allUnitSizeTypes();
    void init();
  }
}
