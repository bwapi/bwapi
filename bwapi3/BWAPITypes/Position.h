#pragma once

#include <Util\Point.h>
#include <Util\Types.h>

namespace BWAPI
{
  typedef Util::Point<s32> Position;

  /*
  struct WalkPosition;
  struct BuildPosition;
  struct Position : Util::Point<int>
  {
    static Position Invalid;

    Position();
    Position(int x, int y);
    explicit Position(Util::Point<int> a);
    Position operator + (const Position& b) const;
    // TODO: wrap all other Point functionalities
    

    WalkPosition toWalkTiles();
    BuildPosition toBuildTiles();
  };
  */
};
