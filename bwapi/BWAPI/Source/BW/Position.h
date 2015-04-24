#pragma once

#include <BWAPI/Position.h>
#include <Util/Types.h>

#pragma pack(1)
namespace BW
{
  // Direct mapping of bw representation of map position.
  BWAPI_MAKE_POSITION_TEMPLATE(Position, s16, 1);
  BWAPI_MAKE_POSITION_TEMPLATE(TilePosition, s16, 32);

  static_assert(sizeof(Position) == 4, "Expected BW position to be 4 bytes");
  static_assert(sizeof(TilePosition) == 4, "Expected BW tile position to be 4 bytes");
};
#pragma pack()

