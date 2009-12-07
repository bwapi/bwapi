#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BW
{
  struct WalkPosition;
  struct BuildPosition;
  struct Position : Util::Point<u16>
  {
    static Position Invalid;

    Position();
    Position(Util::Point<u16> point);
    Position(int x, int y);
    Position(const WalkPosition &convertFrom);
    Position(const BuildPosition &convertFrom);
  };
};


