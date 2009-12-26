#pragma once

#include "UnitCommandTypeID.h"

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
    UnitCommandTypeID commandId;
    int unitIndex;
    int x;              //x position or tile position, depending on order type
    int y;              //y position or tile position, depending on order type
    int targetIndex;    //target unit ID, if applicable
    union   // depending on command type
    {
      TechTypeId tech;
      UpgradeTypeId upgrade;
      UnitTypeId unit;
    };
  };
}