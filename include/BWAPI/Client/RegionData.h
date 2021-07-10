#pragma once

#include <BWAPI/IDs.h>

namespace BWAPI
{
  class Game;
  struct RegionData
  {
    RegionData(Game &g, RegionID rid) : game{ g }, id{ rid } { }
    
    RegionID id;
    Game &game;
    int   islandID = 0;
    int   center_x = 0;
    int   center_y = 0;
    int   priority = 0;

    // region boundary
    int   leftMost = 0;
    int   rightMost = 0;
    int   topMost = 0;
    int   bottomMost = 0;

    int   neighborCount = 0;
    int   neighbors[256] = {};

    bool  isAccessible = false;
    bool  isHigherGround = false;
  };
}
