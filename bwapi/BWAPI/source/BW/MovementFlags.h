#pragma once

namespace BW
{
  /** Definition of Flags specifying movement type - used in BW#UnitData#movementFlags */
  namespace MovementFlags
  {
    enum Enum : u8
    {
      MakingBuilding = 1 << 1,
      Moving         = 1 << 2
    };
  };
};

