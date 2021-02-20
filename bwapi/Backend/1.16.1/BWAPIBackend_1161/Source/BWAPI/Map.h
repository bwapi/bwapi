#pragma once
#include <string>
#include "../BW/Offsets.h"
#include "../Types.h"

namespace bwapi::data
{
  class MapData;
}

namespace BWAPI4
{
  /**
   * Interface to acces broodwar map data. Loads buildability/walkability when
   * constructed from the current map. It means it that instance of this class
   * should exist only as long as the single map is opened.
   */
  class Map
  {
    public :
      // Gets file name of the currently opened map by broodwar
      static std::string getFileName();
      static std::string getPathName();
      static std::string getName();

      // Width of the current map in terms of build tiles
      static u16 getWidth();

      // Height of the current map in terms of build tiles
      static u16 getHeight();

      static bool buildable(int x, int y);
      static bool walkable(int x, int y);
      static bool visible(int x, int y);
      static bool isExplored(int x, int y);
      static bool hasCreep(int x, int y);
      static bool isOccupied(int x, int y);
      static int  groundHeight(int x, int y);

      // Returns a value that represents the map's terrain.
      static std::string calculateMapHash();
      static std::string getMapHash();
      static void copyToSharedMemory(bwapi::data::MapData* mapData);

    private :
      static BW::TileID getTile(int x, int y);
      static inline BW::activeTile getActiveTile(int x, int y)
      {
        if (BW::BWDATA::ActiveTileArray && static_cast<unsigned>(x) < getWidth() && static_cast<unsigned>(y) < getHeight())
          return BW::BWDATA::ActiveTileArray[x + y * Map::getWidth()];
        return BW::activeTile{};
      }
      static u8 getTileVariation(BW::TileID);
      static u16 getMiniTile(int x, int y);

      static std::string savedMapHash;
  };
};
