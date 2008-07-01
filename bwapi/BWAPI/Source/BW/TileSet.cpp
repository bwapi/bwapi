#include "TileSet.h"
#include "TileType.h"
#include "DoodatType.h"
namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    return  (*BW::BWXFN_TileSet + ((tileID >> 4 )& 0X7FF));
  }
  DoodatType* TileSet::getDoodatType(TileID tileID)
  {
    return  (*BW::BWXFN_DoodatSet + ((tileID >> 4 )& 0X7FF));
  }
}