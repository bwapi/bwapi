#pragma once

#include "UnitCommandTypeID.h"

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
      int special; //tech type, upgrade type, or unit type, 
    };
  };
}