#pragma once

#include <string>
#include <Util/RectangleArray.h>
#include <BW/Offsets.h>

namespace BWAPI
{
  /**
   * Interface to acces broodwar map data. Loads buildability/walkability
   * from the current map.
   */
  namespace Map
  {
//public :
    // Gets file name of the currently opened map by broodwar
    extern std::string getFileName();
    extern std::string getName();

    // Width of the current map in terms of build tiles
    extern u16 getWidth();

    // Height of the current map in terms of build tiles
    extern u16 getHeight();

    extern bool buildable(int x, int y);
    extern bool walkable(int x, int y);
    extern bool visible(int x, int y);
    extern bool isExplored(int x, int y);
    extern bool hasCreep(int x, int y);
    extern int groundHeight(int x, int y);

    // Loads buildability/walkability arrays from the currently opened map.
    extern void load();

    // Returns a value that represents the map's terrain.
    extern int getMapHash();
  };
};
