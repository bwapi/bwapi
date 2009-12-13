#pragma once

#include <Util\Point.h>

namespace BW { struct WalkPosition; }

namespace BWAPI
{
  struct Position;
  struct BuildPosition;
  struct WalkPosition : Util::Point<int>
  {
    static WalkPosition Invalid;

    WalkPosition();
    WalkPosition(int x, int y);
    WalkPosition(const Position &convertFrom);
    WalkPosition(const BuildPosition &convertFrom);
  
    template<typename T>
      WalkPosition(Util::Point<T> point)
        : Point(point)
      {
      }
  };
};
