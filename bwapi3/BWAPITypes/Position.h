#pragma once

#include <Util\Point.h>

namespace BW { struct Position; }

namespace BWAPI
{
  struct WalkPosition;
  struct BuildPosition;
  struct Position : Util::Point<int>
  {
    static Position Invalid;

    Position();
    Position(int x, int y);
    Position(const WalkPosition &convertFrom);
    Position(const BuildPosition &convertFrom);

    template<typename T>
      Position(Util::Point<T> point)
        : Point(point)
      {
      }
  };
};
