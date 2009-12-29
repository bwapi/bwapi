#pragma once

#include "UnitCommandTypeId.h"

#include <BWAPITypes\TechTypeId.h>
#include <BWAPITypes\UpgradeTypeId.h>
#include <BWAPITypes\UnitTypeId.h>

/**
 * UnitOrder contains a single whole order
 */

namespace BWAPI
{
  struct UnitCommand
  {
    UnitCommandTypeId commandId;
    int unitIndex;
    int x;              //x position or tile position, depending on order type
    int y;              //y position or tile position, depending on order type
    int targetIndex;    //target unit linear index, if applicable
    union   // depending on command type
    {
      int extra;
      TechTypeId tech;
      UpgradeTypeId upgrade;
      UnitTypeId unitType;
    };
  };
}