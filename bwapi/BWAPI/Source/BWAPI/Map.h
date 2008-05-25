#include "..//BW//Offsets.h"

#include <string>

#include <RectangleArray.h>

namespace BWAPI
{
  /**
   * Interface to acces broodwar map data.
   * @TODO make constructor/destructor for the class
   * and load the map into some reasonable format or at least
   * make pointes to the beginning of lines, so the getTile
   * function doesn't have to use the slow multiplication.
   */
  class Map
  {
    public :
     static BW::TileID getTile(int x, int y);
     static u16 getWidth();
     static u16 getHeight();
     /** Gets file name of the currently opened map by broodwar */
     static char* getFileName();
     /** Saves the buildability map for the currently opened map to the specified file. */
     static void saveBuildabilityMap(const std::string& fileName);
     static RectangleArray<char> getBuildabilityArray();
  };
}