#pragma once
#include "../StaticAssert.h"
#pragma pack(1)
namespace BW
{
  const u32 TILE_TYPE_SIZE_IN_BYTES = 52;
  /** Represents information shared for all tiles in one tile group. */
  class TileType
  {
    public :
      u16 index;
      u8 buildability; /**< 8th bit should sign not buildable. */
      u8 groundHeight; /**< Ground Height(4 lower bits) - Deprecated? Some values are incorrect. */
      u16 leftEdge;
      u16 topEdge;
      u16 rightEdge;
      u16 buttomEdge;
      u16 _1;
      u16 _2; /**<  Unknown - Edge piece has rows above it. (Recently noticed; not fully understood.)
                    o 1 = Basic edge piece.
                    o 2 = Right edge piece.
                    o 3 = Left edge piece.*/
      u16 _3;
      u16 _4;
      u16 miniTile[16]; /** MegaTile References (VF4/VX4) */
  };
  BOOST_STATIC_ASSERT(sizeof(TileType) == TILE_TYPE_SIZE_IN_BYTES);
}
#pragma pack()
