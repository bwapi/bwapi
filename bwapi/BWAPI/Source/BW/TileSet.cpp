#include "TileSet.h"
#include "TileType.h"

#include "../../../Debug.h"

namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    if ( *BW::BWDATA::TileSet )
      return  (*BW::BWDATA::TileSet + ((tileID >> 4 ) & 0x7FF));
    return NULL;
  }
};
