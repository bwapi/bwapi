#pragma once

#include <string>
#include <Util/Types.h>

namespace BW
{
  /** Maps bw definition of race. */
  namespace Race
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
    Enum stringToRace(const std::string& name);
    std::string raceName(Enum race);
  }
}
