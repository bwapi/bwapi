#include <BW\WalkPosition.h>

namespace BWAPI
{
  typedef BW::WalkPosition WalkPosition;
}

/* moved to BW 
TODO: look if it pays off

#pragma once

#include <Util\Point.h>

namespace BWAPI
{
  struct Position;
  struct BuildPosition;
  struct WalkPosition : Math::Point
  {
    Math::Point point;

    static WalkPosition Invalid;
    WalkPosition();
    WalkPosition(Math::Point point);
    WalkPosition(int x, int y);
    WalkPosition(const Position &convertFrom);
    WalkPosition(const BuildPosition &convertFrom);
  };
};
*/
