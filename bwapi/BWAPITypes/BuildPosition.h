#pragma once

#include <Util\Point.h>

namespace BW { struct BuildPosition; }

namespace BWAPI
{
  struct Position;
  struct WalkPosition;
  struct BuildPosition : Util::Point<int>
  {
    static BuildPosition Invalid;

    BuildPosition();
    BuildPosition(int x, int y);
    BuildPosition(const Position &convertFrom);
    BuildPosition(const WalkPosition &convertFrom);

    template<typename T>
      BuildPosition(Util::Point<T> point)
        : Point(point)
      {
      }
  };
};
