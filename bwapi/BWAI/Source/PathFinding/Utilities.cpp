#include "Utilities.h"

#include <RectangleArray.h>

#include "../BWAI/Unit.h"
#include "../BWAI/Globals.h"
#include "../BWAI/ai.h"
#include "../../../BWAPI/Source/BWAPI/Map.h"


namespace PathFinding
{
  Util::RectangleArray<int> Utilities::temp(1,1);
  std::vector<BW::Position> Utilities::returnValue;
  std::multimap<int, WalkabilityPosition> Utilities::vawe;
  //--------------------------------- CONFLICTS WITH MAP -------------------------------------------
  bool Utilities::conflictsWithMap(const UnitModel& unit)
  {
    const Util::RectangleArray<bool>& walkability = BWAI::ai->map->getWalkabilityArray();
    int x1 = (unit.position.x - unit.original->getType().dimensionLeft())/4;
    int x2 = ((unit.position.x + unit.original->getType().dimensionRight()) + 3)/4;
    int y1 = (unit.position.y - unit.original->getType().dimensionUp())/4;
    int y2 = ((unit.position.y + unit.original->getType().dimensionDown()) + 3)/4;
    for (int x = x1; x < x2; x++)
      for (int y = y1; y < y2; y++)
        if (!walkability[x][y])
           return false;
    return true;
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::generatePath(const UnitModel& unit, BW::Position target)
  {
    Utilities::vawe.clear();
    temp.resize(BWAPI::Map::getWidth()*4, BWAPI::Map::getHeight()*4);
    temp.setTo(-1);
    /** First sample pathfinding to go here*/
  }
  //------------------------------------------------------------------------------------------------
}