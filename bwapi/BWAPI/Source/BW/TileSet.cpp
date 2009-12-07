#include "TileSet.h"
#include "TileType.h"
#include "DoodatType.h"
namespace BW
{
  namespace TileSet
  {
    TileType* getTileType(TileID tileID)
    {
      return (*BW::BWDATA_TileSet + ((tileID >> 4 ) & 0X7FF));
    }
    DoodatType* getDoodatType(TileID tileID)
    {
      return (*BW::BWDATA_DoodatSet + ((tileID >> 4 ) & 0X7FF));
    }
  }
};
