#pragma once

#include <BWAPITypes\Position.h>

#include <string>

namespace BWAPI
{
  namespace Tracer
  {
    extern void pushLine(Position from, Position to, int color);
    extern void pushText(Position pos, std::string str);
    extern void pushMapLine(Position from, Position to, int color);
    extern void pushMapText(Position pos, std::string str);
    extern void removeShapes();
    extern void drawShapes();
  }
}
