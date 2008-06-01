#include "UnitModel.h"

#include <vector>
#include <map>

#include <RectangleArray.h>

#include "../../../BWAPI/Source/BW/Position.h"

class WalkabilityPosition
{
  public :
    u16 x,y;
};

namespace BasicDirection
{
  enum Enum
  {
    Up    = 0,
    Right = 1,
    Down  = 2,
    Left  = 3
  };
}


/**
 * All stuff related directly to path finding.
 */
namespace PathFinding
{
   /**
    * Utilities related to pathfinding.
    */
   class Utilities
   {
     public :
       /**
        * Gets if the specified unit conflicts with map.
        * @param unit Model of the unit to test.
        * @return @c true if the unit has conflict with the map walkability, @c false otherwise
        */                                    
       static bool conflictsWithMap(const UnitModel& unit);
       
       static bool generatePath(const UnitModel& unit, BW::Position target);
       /** static help variables. */
       static Util::RectangleArray<int> temp;
       static std::vector<BW::Position> returnValue;
       static std::multimap<int, WalkabilityPosition> vawe;
   };
}