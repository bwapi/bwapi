#include "TileSet.h"
#include "TileType.h"
namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    if ( BW::BWDATA_TileSet )
      return  (BW::BWDATA_TileSet + ((tileID >> 4 ) & 0x7FF));
    return NULL;
  }
};
