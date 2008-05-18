#include "TileSet.h"
#include "TileType.h"
namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    return *BW::BWXFN_TileSet + tileID;
  }
}