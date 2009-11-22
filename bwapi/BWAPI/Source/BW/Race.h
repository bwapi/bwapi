#pragma once

#include <string>
#include <Util/Types.h>

namespace BW
{
  /*  Notes: I think this is ID & 0x07
  */
  /** Maps bw definition of race. */
  namespace Race
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
    u8 stringToRace(const std::string& name);
    std::string raceName(u8 race);
  }
}
