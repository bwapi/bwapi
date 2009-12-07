#include "Offsets.h"
namespace BW
{
  class TileType;
  class DoodatType;
  /**
   *  Set of tiles to construct a map, every map can have different set,
   *  but every map use just one set.
   */
  namespace TileSet
  {
//public :
    extern TileType* getTileType(TileID tileID);
    extern DoodatType* getDoodatType(TileID tileID);
  };
};
