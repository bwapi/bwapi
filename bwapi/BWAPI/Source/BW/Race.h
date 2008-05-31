#pragma once

namespace BW
{
  /** 
   * Maps bw definition of race.
   */
  namespace Race
  {
    enum Enum : u8
    {
      Zerg    = 0,
      Terran  = 1,
      Protoss = 2,
      Other   = 3,
      Random  = 6
    };
  }
}