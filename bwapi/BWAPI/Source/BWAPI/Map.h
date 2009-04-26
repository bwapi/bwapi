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
      static std::string getFileName();
      /**
       * Saves the buildability map for the currently opened map to the
       * specified file.
       */
      char *saveBuildabilityMap(const std::string& fileName) const;
      char *saveWalkabilityMap(const std::string& fileName) const;
      char *saveFogOfWarMap(const std::string& fileName) const;
      bool buildable(int x, int y) const;
      bool walkable(int x, int y) const;
      bool visible(int x, int y) const;
      /** Loads buildability/walkability arrays from the currently opened map. */
      void load();
    private :
      Util::RectangleArray<u32>* fogOfWar;
      Util::RectangleArray<bool> buildability;
      Util::RectangleArray<bool> walkability;
      void setBuildability();
      void setWalkability();
  };
}