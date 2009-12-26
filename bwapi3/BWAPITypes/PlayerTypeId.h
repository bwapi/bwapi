#pragma once

namespace BWAPI
{
  namespace PlayerTypeIds
  {
    enum Enum
    {
      NotUsed = 0,
      Computer = 1,
      Human = 2,
      Rescuable = 3,
      ComputerSlot = 5,
      OpenSlot = 6,
      Neutral = 7,
      ClosedSlot = 8,
      HumanDefeated = 10,
      ComputerDefeated = 11,
      None = 12,
      count
    };
  }
  typedef PlayerTypeIds::Enum PlayerTypeId;
}
