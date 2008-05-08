#pragma once

#include "../Types.h"

namespace BWAPI
{
  namespace TargetType
  {
    enum Enum : u16
    {
      TARGET_MY_UNITS        = 1 << 0,
      TARGET_OPPONENTS_UNITS = 1 << 1,
      TARGET_BIOLOGICAL      = 1 << 2,
      TARGET_MECHANICAL      = 1 << 3,
      TARGET_ROBOTIC         = 1 << 4,
      TARGET_LOCATION        = 1 << 5,
      TARGET_SELF            = 1 << 6 | TARGET_MY_UNITS,
      TARGET_TEMPLAR         = 1 << 7,
      TARGET_DARK_TEMPLAR    = 1 << 8,
      TARGET_RESOURCE        = 1 << 9
    };
  }
}