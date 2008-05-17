#pragma once
#pragma pack(1)
namespace BW
{
  /** Isn't mapped yet, but I will leave it here*/
  class TileType
  {
   u16 index;
   u8 buildability;
   u8 groundHeight;
   _UNKNOWN _1[16];
   u16 miniTile[16];
  }
}
#pragma pack()