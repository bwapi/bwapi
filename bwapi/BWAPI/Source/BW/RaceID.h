#pragma once

#include <string>
#include <Util/Types.h>

namespace BW
{
  //  Notes: I think this is ID & 0x07
  // Maps bw definition of race.
  namespace RaceIDs
  {
    enum Enum
    {
      Zerg    = 0,
      Terran  = 1,
      Protoss = 2,
      Other   = 3,
      Select  = 5,
      Random  = 6,
      None    = 7,
    };
  }

  // Database functions
  namespace RaceIDs
  {
    u8 fromString(const std::string& name);
    std::string getName(u8 race);
  };
};
