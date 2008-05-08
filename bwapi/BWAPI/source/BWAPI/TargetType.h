#pragma once

#include "../Types.h"

namespace BWAPI
{
  /** Flag value used to sepcify what target types can ability target in BWAPI#AbilityPrototype#targetFlags */
  namespace TargetType
  {
    enum Enum : u16
    {
      TARGET_MY_UNITS        = 1 << 0, /** Ability can target my own unit */
      TARGET_OPPONENTS_UNITS = 1 << 1, /** Ability can target opponents unit */
      TARGET_BIOLOGICAL      = 1 << 2, /** Ability can target Biological units */
      TARGET_MECHANICAL      = 1 << 3,
      TARGET_ROBOTIC         = 1 << 4,
      TARGET_LOCATION        = 1 << 5, /** Ability can target location */
      TARGET_SELF            = 1 << 6, /** Ability can target it's executor */
      TARGET_TEMPLAR         = 1 << 7,
      TARGET_DARK_TEMPLAR    = 1 << 8,
      TARGET_RESOURCE        = 1 << 9  /** Ability can target minerals/gas*/
    };
  }
}