#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BWAPI
{
  typedef Util::Point<s32> BuildPosition;

  /*
  struct Position;
  struct WalkPosition;
  struct BuildPosition : Util::Point<int>
  {
    static BuildPosition Invalid;

    BuildPosition();
    BuildPosition(int x, int y);
    // TODO: wrap all Point functionalities

    Position toPixels();
    WalkPosition toWalkTiles();

    template<typename T>
      BuildPosition(Util::Point<T> point)
        : Point(point)
      {
      }
  };
  */
};
