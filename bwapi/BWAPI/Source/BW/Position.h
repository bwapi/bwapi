#pragma once

#include <BWAPI/Position.h>
#include <Util/Types.h>

#pragma pack(1)
namespace BW
{
  // Direct mapping of bw representation of map position.
  BWAPI_MAKE_POSITION_TEMPLATE(Position,short,1)
  BWAPI_MAKE_POSITION_TEMPLATE(TilePosition,short,32)
};
#pragma pack()

