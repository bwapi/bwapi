#include "TileSet.h"
#include "TileType.h"
#include "DoodatType.h"
namespace BW
{
  TileType* TileSet::getTileType(TileID tileID)
  {
    return  (*BW::BWDATA_TileSet + ((tileID >> 4 )& 0X7FF));
  }
  DoodatType* TileSet::getDoodatType(TileID tileID)
  {
    return  (*BW::BWDATA_DoodatSet + ((tileID >> 4 )& 0X7FF));
  }
}