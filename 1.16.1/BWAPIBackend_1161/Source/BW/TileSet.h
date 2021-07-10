#pragma once

#include "../Types.h"
namespace BW
{
  typedef u16 TileID;
  class TileType;
  /**
   *  Set of tiles to construct a map, every map can have different set,
   *  but every map use just one set.
   */
  class TileSet
  {
    public :
      static TileType* getTileType(TileID tileID);
  };
};
