#include "Map.h"
namespace BWAPI
{
  //------------------------------- GET TILE --------------------------------
  BW::TileID Map::getTile(int x, int y)
  {
    return *(*BW::BWXFN_MapTileArray + (x + y*Map::getWidth())) & 0x2FF;
  }
  //------------------------------ GET WIDTH --------------------------------
  u16 Map::getWidth()
  {
    return *BW::BWXFN_MapSizeX;
  }
  //------------------------------ GET HEIGHT -------------------------------
  u16 Map::getHeight()
  {
    return *BW::BWXFN_MapSizeY;
  }
  //-------------------------------------------------------------------------
}