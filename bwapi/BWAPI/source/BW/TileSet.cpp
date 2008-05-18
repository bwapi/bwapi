#include "TileSet.h"
#include "TileType.h"
namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    //return  (TileType *)(((int)*BW::BWXFN_TileSet) + (((tileID>>4) & 0x7FF)*0x1A + (tileID & 0xF))*2+0x14);
    return  (*BW::BWXFN_TileSet + ((tileID >> 4 )& 0X7FF));
  }
}