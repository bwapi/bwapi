#pragma once

#include "Position.h"
#include "UnitID.h"

namespace BW
{
  /**
   * Combination of bw position representation and bw target representation,
   * it is connectd as bw uses this combination on several places.
   */
  struct PositionUnitID
  {
    BW::Position position;
    BW::UnitID unitId;

    PositionUnitID();
    PositionUnitID(const BW::Position& target);
    PositionUnitID(BW::UnitID target);
  };
}

