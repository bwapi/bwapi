#pragma once

#include <string>
#include <Util/Types.h>

namespace BW
{
  //  Notes: I think this is ID & 0x07
  // Maps bw definition of race.
  namespace RaceIDs
  {
    enum Enum : u8
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
  typedef RaceIDs::Enum RaceID;

  // Database functions
  namespace RaceIDs
  {
    RaceID fromString(const std::string& name);
    std::string getName(RaceID race);
  };
};
