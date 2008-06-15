#pragma once

#include <BW/TilePosition.h>

namespace BWAI
{
  class BuildingPosition
  {
    public:
      BuildingPosition(const BW::TilePosition& position);
      BW::TilePosition position;
      bool reserved;
  };
}