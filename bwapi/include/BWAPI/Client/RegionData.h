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

    // region boundry
    int   leftMost;
    int   rightMost;
    int   topMost;
    int   bottomMost;

    int   neighborCount;
    int   neighbors[256];

    bool  isWalkable;
    bool  isHigherGround;
  };
};
