#pragma once

#include <BWAPI/Position.h>
#include <Util/Types.h>

#pragma pack(1)
namespace BW
{
  /** Direct mapping of bw representation of map position. */
  _MAKE_POSITION_TEMPLATE(Position,short,1)
  _MAKE_POSITION_TEMPLATE(TilePosition,short,32)
};
#pragma pack()

