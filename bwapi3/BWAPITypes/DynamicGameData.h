#pragma once
/**
 *  DynamicGameData exposes all dynamic data like events to the AI
 *  The arrays have to be filled with pointers. We don't want to give
 *  the AI our data, just point it to it.
 *
 *  We only fill the containers once each frame when handling the
 *  shared memory stacks. So since only push_back() and clear() are
 *  called on this containers, vectors should be used. A vector also
 *  gives reallocation advantage since it does not shrink, and it's
 *  linear storage has excelent caching qualities.
 */

#include <vector>
#include <BWAPITypes\UnitEvent.h>

namespace BWAPI
{
  class DynamicGameData
  {
  public:
    std::vector<const UnitAddEvent*> unitAddEvents;
    std::vector<const UnitRemoveEvent*> unitRemoveEvents;
  };
}
