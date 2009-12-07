#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BW
{
  struct Position;
  struct BuildPosition;
  struct WalkPosition : Util::Point<u16>
  {
    static WalkPosition Invalid;
    WalkPosition();
    WalkPosition(Util::Point<u16> point);
    WalkPosition(u16 x, u16 y);
    WalkPosition(const Position &convertFrom);
    WalkPosition(const BuildPosition &convertFrom);
  };
};

