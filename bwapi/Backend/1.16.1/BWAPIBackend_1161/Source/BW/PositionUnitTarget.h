#pragma once

#include "Position.h"
#include "UnitTarget.h"

namespace BW
{
  /**
   * Combination of bw position representation and bw target representation,
   * it is connected as bw uses this combination in several places.
   */
  class PositionUnitTarget
  {
    public :
      PositionUnitTarget(const Position& position);
      PositionUnitTarget(int x, int y);
      PositionUnitTarget(const UnitTarget& target);
      BW::Position   position;
      BW::UnitTarget target;
  };
}

