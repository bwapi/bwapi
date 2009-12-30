#pragma once

#include "ForceId.h"

namespace BWAPI
{
  struct Player
  {
    ForceId force;
    s32 minerals;
    s32 gas;
    char name[25];  // size as BW memory
  };
}
