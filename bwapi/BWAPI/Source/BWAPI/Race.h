#pragma once

namespace BWAPI
{
  /** 
   * Our internal type to represent race.
   * BW#ChangeRace#Race probably maps the bw implemantation, but it must be investigated more
   */
  namespace Race
  {
    enum Enum : u8
    {
      Terran = 0,
      Zerg = 1,
      Protoss = 2,
      Other = 3
    };
  }
}