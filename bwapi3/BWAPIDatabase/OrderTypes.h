#pragma once
/**
 *  the race data table
 */

#include <BWAPITypes\OrderTypeId.h>

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  struct OrderType
  {
    const char* name;
  };
  namespace OrderTypes
  {
    extern OrderType actionTypeData[OrderTypeIds::count];

    extern void init();
    extern OrderTypeId &getIdByName(std::string& name);
  }
}
