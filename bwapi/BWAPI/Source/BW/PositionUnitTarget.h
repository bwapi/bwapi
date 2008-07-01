#pragma once

#include "Position.h"
#include "UnitTarget.h"

namespace BW
{
  /** 
   * Combination of bw position representation and bw target representation, 
   * it is connectd as bw uses this combination on several places. 
   */
  class PositionUnitTarget
  {
    public :
      PositionUnitTarget(const Position& position);
      PositionUnitTarget(const UnitTarget& target);
      BW::Position position;
      BW::UnitTarget target; 
  };
}

