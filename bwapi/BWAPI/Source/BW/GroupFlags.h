#pragma once
namespace BW
{
  /** Additional unit prototype information. */
  namespace GroupFlags
  {
  enum Enum
    {
      Zerg        = 1 << 0,
      Terran      = 1 << 1,
      Protoss     = 1 << 2,
      Men         = 1 << 3,
      Building    = 1 << 4,
      Factory     = 1 << 5,
      Independant = 1 << 6,
      Neutral     = 1 << 7
    };
  }
};