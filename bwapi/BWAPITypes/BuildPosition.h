#include <BW\BuildPosition.h>

namespace BWAPI
{
  typedef BW::BuildPosition BuildPosition;
}

/* moved to BW 
TODO: look if it pays off

#pragma once

#include <Util\Point.h>

namespace BWAPI
{
  struct Position;
  struct WalkPosition;
  struct BuildPosition : Math::Point
  {
    Math::Point point;

    static BuildPosition Invalid;
    BuildPosition();
    BuildPosition(Math::Point point);
    BuildPosition(int x, int y);
    BuildPosition(const Position &convertFrom);
    BuildPosition(const WalkPosition &convertFrom);
  };
};

*/