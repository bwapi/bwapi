#include <BW\Position.h>

namespace BWAPI
{
  typedef BW::Position Position;
}

/* moved to BW 
TODO: look if it pays off

#pragma once

#include <Util\Point.h>

namespace BWAPI
{
  struct WalkPosition;
  struct BuildPosition;
  struct Position : Math::Point
  {
    static Position Invalid;

    Position();
    Position(Math::Point point);
    Position(int x, int y);
    Position(const WalkPosition &convertFrom);
    Position(const BuildPosition &convertFrom);
  };
};

*/
