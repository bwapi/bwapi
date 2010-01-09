#pragma once

#include "ForceId.h"

#include <Util\StaticString.h>

namespace BWAPI
{
  struct Player
  {
    ForceId force;
    s32 minerals;
    s32 gas;
    Util::StaticString<25> name;  // size as BW memory
  };
}
