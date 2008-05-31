#include "UnitModel.h"

/**
 * All stuff related directly to path finding.
 */
namespace PathFinding
{
   /**
    * Utilities related to pathfinding.
    */
   class Util
   {
     public :
       /**
        * Gets if the specified unit conflicts with map.
        * @param unit Model of the unit to test.
        * @return @c true if the unit has conflict with the map walkability, @c false otherwise
        */                                    
       static bool conflictsWithMap(const UnitModel& unit);
   };
}