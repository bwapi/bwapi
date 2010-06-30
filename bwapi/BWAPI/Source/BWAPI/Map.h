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
      /** Gets file name of the currently opened map by broodwar */
      static std::string getFileName();
      static std::string getName();

      /** Width of the current map in terms of build tiles */
      static u16 getWidth();

      /** Height of the current map in terms of build tiles */
      static u16 getHeight();

      bool buildable(int x, int y) const;
      bool walkable(int x, int y) const;
      bool visible(int x, int y) const;
      bool isExplored(int x, int y) const;
      bool hasCreep(int x, int y) const;
      int groundHeight(int x, int y) const;

      /** Loads buildability/walkability arrays from the currently opened map. */
      void load();

      /** Returns a value that represents the map's terrain. */
      static int getMapHash();
      void copyToSharedMemory();

    private :
      static BW::TileID getTile(int x, int y);
      static u8 getTileVariation(BW::TileID);
      Util::BitMask<u16> getMiniTile(int x, int y) const;
      Util::RectangleArray<u32>* fogOfWar;
      Util::RectangleArray<u16>* zergCreep;
      Util::RectangleArray<bool> buildability;
      Util::RectangleArray<bool> walkability;
      void setBuildability();
      void setWalkability();
  };
};
