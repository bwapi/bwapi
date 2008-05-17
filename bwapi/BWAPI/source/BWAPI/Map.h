#include "..//BW//Offsets.h"
namespace BWAPI
{
  /**
   * Interface to acces broodwar map data.
   * @Todo make constructor/destructor for the class
   * and load the map into some reasonable format.
   */
  class Map
  {
    public :
     static BW::TileID getTile(int x, int y);
     static u16 getWidth();
     static u16 getHeight();
  };
}