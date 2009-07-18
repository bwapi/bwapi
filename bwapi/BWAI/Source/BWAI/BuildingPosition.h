#pragma once

#include <BWAPI.h>

namespace BWAI
{
  /** Represent sinlge pre-defined building position. */
  class BuildingPosition
  {
    public:
      BuildingPosition(const BWAPI::TilePosition& position);
      BWAPI::TilePosition position;
      /**
       * Specifies if this position is already reserved by some TaskBuild, this prevents other TaskBuild to
       * use the same location.
       */
      bool reserved;
  };
}