#pragma once

#include <BWAPI/IDs.h>

namespace BWAPI
{
  struct RegionData
  {
    RegionID id;
    Game &game;
    int   islandID;
    int   center_x;
    int   center_y;
    int   priority;

    // region boundary
    int   leftMost;
    int   rightMost;
    int   topMost;
    int   bottomMost;

    int   neighborCount;
    int   neighbors[256];

    bool  isAccessible;
    bool  isHigherGround;
  };
};
