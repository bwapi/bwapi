#include "..//BW//Offsets.h"

#include <string>

#include <RectangleArray.h>

namespace BWAPI
{
  /**
   * Interface to acces broodwar map data. Loads buildability/walkability when
   * constructed from the current map. It means it that instance of this class
   * should exist only as long as the single map is opened.
   */
  class Map
  {
    public :
      /** Loads buildability/walkability arrays from the currently opened map. */
      Map();
      static BW::TileID getTile(int x, int y);
      static u8 getTileVariation(BW::TileID);
      static u16 getWidth();
      static u16 getHeight();
      /** Gets file name of the currently opened map by broodwar */
      static char* getFileName();
      /**
       * Saves the buildability map for the currently opened map to the
       * specified file.
       */
      void saveBuildabilityMap(const std::string& fileName);
      void saveWalkabilityMap(const std::string& fileName);
      const Util::RectangleArray<bool>& getBuildabilityArray();
      const Util::RectangleArray<bool>& getWalkabilityArray();
    private :
      Util::RectangleArray<bool> buildability;
      Util::RectangleArray<bool> walkability;
      void setBuildability();
      void setWalkability();
  };
}