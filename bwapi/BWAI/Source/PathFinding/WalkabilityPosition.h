#pragma once

#include <BWAPI.h>
#include <Util/Types.h>
namespace PathFinding
{
  /** Position of unit arranged to fit with the Walkability array. */
  class WalkabilityPosition
  {
    public :
      WalkabilityPosition();
      WalkabilityPosition(const WalkabilityPosition& position);
      WalkabilityPosition(u16 x, u16 y);
      /** Conversion constructor from #BW#Position (x,y/8, as walkability field info defines 8X8 square) */
      WalkabilityPosition(BWAPI::Position position);
      bool operator==(const WalkabilityPosition& position);
      bool operator!=(const WalkabilityPosition& position);
      u16 x, y;
      BWAPI::Position toBWAPIPosition();
  };
}