#pragma once

#include <string>

#include <Util/RectangleArray.h>
#include <BW/Offsets.h>

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
      Map();
      ~Map();
      static BW::TileID getTile(int x, int y);
      static u8 getTileVariation(BW::TileID);
      static u16 getWidth();
      static u16 getHeight();
      /** Gets file name of the currently opened map by broodwar */
      static char* getFileName();
      static std::string getName();
      /**
       * Saves the buildability map for the currently opened map to the
       * specified file.
       */
      char *saveBuildabilityMap(const std::string& fileName);
      char *saveWalkabilityMap(const std::string& fileName);
      char *saveFogOfWarMap(const std::string& fileName, u8 playerID);
      const Util::RectangleArray<bool>& getBuildabilityArray();
      const Util::RectangleArray<bool>& getWalkabilityArray();
      /** Loads buildability/walkability arrays from the currently opened map. */
      void load();
      Util::RectangleArray<u32>* fogOfWar;
    private :
      Util::RectangleArray<bool> buildability;
      Util::RectangleArray<bool> walkability;
      void setBuildability();
      void setWalkability();
  };
}