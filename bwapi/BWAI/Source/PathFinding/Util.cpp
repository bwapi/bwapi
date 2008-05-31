#include "Util.h"

#include "../BWAI/Unit.h"

namespace PathFinding
{
  //--------------------------------- CONFLICTS WITH MAP -------------------------------------------
  bool Util::conflictsWithMap(const UnitModel& unit)
  {
    int x1 = (unit.position.x - unit.original->getType().dimensionLeft())/4;
    int x2 = (unit.position.x + unit.original->getType().dimensionRight())/4;
  }
  //------------------------------------------------------------------------------------------------
}