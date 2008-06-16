#pragma once

#include <BW/TilePosition.h>

namespace BWAI
{
  /** Represent sinlge pre-defined building position. */
  class BuildingPosition
  {
    public:
      BuildingPosition(const BW::TilePosition& position);
      BW::TilePosition position;
      /**
       * Specifies if this position is already reserved by some TaskBuild, this prevents other TaskBuild to
       * use the same location.
       */
      bool reserved; 
  };
}