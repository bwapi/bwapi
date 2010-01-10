#pragma once

#include "RaceId.h"
#include "ForceId.h"
#include "PlayerTypeId.h"

#include <Util\StaticString.h>
#include <Util\StaticSet.h>

namespace BWAPI
{
  struct Player
  {
    ForceId force;
    PlayerTypeId type;
    RaceId race;
    s32 minerals;
    s32 gas;
    s32 cumulativeMinerals;
    s32 cumulativeGas;
    s32 suppliesAvailable[3];
    s32 suppliesUsed[3];
    Util::StaticString<25> name;  // size as BW memory

    struct Attitude
    {
      bool ally;
    };
    Attitude attitudes[12];       // index correlated with players

    // helper functions
    int inline getOwnRaceAvailableSupplies() const
    {
      return suppliesAvailable[race];
    }

    int inline getOwnRaceUsedSupplies() const
    {
      return suppliesUsed[race];
    }
  };
}
