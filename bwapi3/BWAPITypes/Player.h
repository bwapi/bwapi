#pragma once

#include "ForceId.h"
#include "PlayerTypeId.h"

#include <Util\StaticString.h>

namespace BWAPI
{
  struct Player
  {
    ForceId force;
    PlayerTypeId type;
    s32 minerals;
    s32 gas;
    s32 cumulativeMinerals;
    s32 cumulativeGas;
    Util::StaticString<25> name;  // size as BW memory
  };
}
