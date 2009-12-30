#pragma once

#include "PlayerId.h"

namespace BWAPI
{
  struct Force
  {
    PlayerId players[8];
    char name[32];
  };
}
