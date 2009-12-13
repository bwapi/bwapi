#pragma once

#include <BWAPITypes\Position.h>

#include <Util\Types.h>

#include <string>
#include <vector>

namespace BWAPI
{
  namespace Tracer
  {
//public:
    extern void deactivate();
    extern bool command(const std::vector<std::string> &);
    extern void onMatchFrame();
    extern void onDraw();
  }
}
