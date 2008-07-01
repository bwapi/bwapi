#include "Offsets.h"
namespace BW
{
  class TileType;
  class DoodatType;
  /** 
   *  Set of tiles to construct a map, every map can have different set, 
   *  but every map use just one set. 
   */
  class TileSet
  {
    public :
      static TileType* getTileType(TileID tileID);
      static DoodatType* getDoodatType(TileID tileID);
  };
}