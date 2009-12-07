#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BW
{
  struct Position;
  struct WalkPosition;
  struct BuildPosition : Util::Point<u16>
  {
    static BuildPosition Invalid;
    BuildPosition();
    BuildPosition(Util::Point<u16> point);
    BuildPosition(u16 x, u16 y);
    BuildPosition(const Position &convertFrom);
    BuildPosition(const WalkPosition &convertFrom);
  };
};

