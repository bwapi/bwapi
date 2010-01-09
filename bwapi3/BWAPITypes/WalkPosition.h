#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BWAPI
{
  typedef Util::Point<s32> WalkPosition;

  /*
  struct Position;
  struct BuildPosition;
  struct WalkPosition : Util::Point<int>
  {
    static WalkPosition Invalid;

    WalkPosition();
    WalkPosition(int x, int y);
    // TODO: wrap all Point functionalities

    Position toPixels();
    BuildPosition toBuildTiles();
  
    template<typename T>
      WalkPosition(Util::Point<T> point)
        : Point(point)
      {
      }
  };
  */
};
