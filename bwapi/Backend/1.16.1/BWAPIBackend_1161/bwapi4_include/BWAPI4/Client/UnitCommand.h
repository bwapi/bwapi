#pragma once

#include <BWAPI4/UnitCommandType.h>

/**
 * UnitOrder contains a single whole order
 */

namespace BWAPIC4
{
  struct UnitCommand
  {
    BWAPI4::UnitCommandType type;
    int unitIndex;
    int targetIndex;
    int x;
    int y;
    int extra;
  };
}