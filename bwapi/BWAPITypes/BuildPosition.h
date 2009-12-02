#pragma once

#include "Math\Point.h"

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

