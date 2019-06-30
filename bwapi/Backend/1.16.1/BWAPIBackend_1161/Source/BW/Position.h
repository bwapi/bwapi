#pragma once

#include <BWAPI/Position.h>
#include "../Types.h"

#pragma pack(1)
namespace BW
{
  // Direct mapping of bw representation of map position.
  typedef BWAPI::Point<s16, 1> Position;
  namespace Positions
  {
    const Position Invalid(32000 / 1, 32000 / 1);
    const Position None(32000 / 1, 32032 / 1);
    const Position Unknown(32000 / 1, 32064 / 1);
    const Position Origin(0, 0);
  }

  typedef BWAPI::Point<s16, 32> TilePosition;
  namespace TilePositions
  {
    const TilePosition Invalid(32000 / 32, 32000 / 32);
    const TilePosition None(32000 / 32, 32032 / 32);
    const TilePosition Unknown(32000 / 32, 32064 / 32);
    const TilePosition Origin(0, 0);
  }

  static_assert(sizeof(Position) == 4, "Expected BW position to be 4 bytes");
  static_assert(sizeof(TilePosition) == 4, "Expected BW tile position to be 4 bytes");
};
#pragma pack()

