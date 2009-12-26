#pragma once

#include "BWAPITypes\UnitTypeId.h"

namespace BWAPI
{
  struct Race
  {
    int id;
    const char *name;
    UnitTypeId worker;
    UnitTypeId center;
    UnitTypeId refinery;
    UnitTypeId transport;
    UnitTypeId supplyProvider;
  };
}
