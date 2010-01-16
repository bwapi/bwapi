#pragma once

#include <BWAPITypes\Position.h>

#include <string>

namespace BWAPI
{
  namespace Tracer
  {
    extern void pushLineScreen(Position from, Position to, int color);
    extern void pushLineMap   (Position from, Position to, int color);
    extern void pushTextScreen(Position pos, const char* str, ...);
    extern void pushTextMap   (Position pos, const char* str, ...);
    extern void removeShapes();
    extern void drawShapes();
  }
}
