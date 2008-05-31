#pragma once
#include "../StaticAssert.h"
#pragma pack(1)
namespace BW
{
  const u32 DOODAT_TYPE_SIZE_IN_BYTES = 52;
  /**
   * Represents information about doodat part (It is not used by bwapi yet, as
   * for walkability reasons it is the same as TileType)
   */
  class DoodatType
  {
    public :
      u16 index;
      u8 buildability; /**< 8th bit should sign not buildable. */
      u8 groundHeightAndOverlayFlags; /**< Ground Height(4 lower bits) - Deprecated? Some values are incorrect. 
                                       * Overlay Flags:
                                       * o 0x0 - None
                                       * o 0x1 - Sprites.dat Reference
                                       * o 0x2 - Units.dat Reference (unit sprite)
                                       * o 0x4 - Overlay is Flipped 
                                       */
      u16 overlayID;
      u16 _1;
      u16 doodatGrupString;
      u16 _2;
      u16 dddataBinIndex;
      u16 doodatHeight; 
      u16 doodatWidth; 
      u16 _3; 
      u16 miniTile[16]; /** MegaTile References (VF4/VX4) */
  };
  BOOST_STATIC_ASSERT(sizeof(TileType) == DOODAT_TYPE_SIZE_IN_BYTES);
}
#pragma pack()
