#pragma once

namespace BWAPI
{
  struct RegionData
  {
    int   id;
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
