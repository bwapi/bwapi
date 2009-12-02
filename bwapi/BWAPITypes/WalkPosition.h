#pragma once

#include "Math\Point.h"

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

