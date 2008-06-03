#pragma once
#include <vector>
#include <map>

#include <BW/Position.h>
#include <Util/RectangleArray.h>

#include "UnitModel.h"
#include "WalkabilityPosition.h"


/* All the stuff here will be sorted somehow soon . */
static const int BASIC_DIRECTION_COUNT = 4;
namespace BasicDirection
{
  enum Enum : u8
  {
    Up    = 0,
    Right = 1,
    Down  = 2,
    Left  = 3
  };
}

namespace AdvancedDirectionID
{
  enum Enum : u8
  {
    Up    = 0,
    Right = 1,
    Down  = 2,
    Left  = 3,
    Near  = 4,
    Unset = 5
  };
}


class AdvancedDirection
{
  public:
    AdvancedDirection() : direction(AdvancedDirectionID::Unset) {}
    AdvancedDirection(const BasicDirection::Enum& direction) : direction((AdvancedDirectionID::Enum)direction) {}
    AdvancedDirection(const AdvancedDirectionID::Enum& direction) : direction(direction) {}
    AdvancedDirectionID::Enum direction;
    bool operator==(const AdvancedDirectionID::Enum& direction) { return this->direction == direction;}
    bool operator!=(const AdvancedDirectionID::Enum& direction) { return this->direction != direction;}
};

static const int forwardDirection[BASIC_DIRECTION_COUNT][2] = 
{
 { 0, -1},
 { 1,  0},
 { 0,  1},
 {-1,  0},
};


static const int reverseDirection[BASIC_DIRECTION_COUNT][2] = 
{
 { 0,  1},
 {-1,  0},
 { 0, -1},
 { 1,  0},
};

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
     public:
       /**
        * Gets if the specified unit conflicts with map.
        * @param unit Model of the unit to test.
        * @return @c true if the unit has conflict with the map walkability, @c false otherwise
        */                                    
       static bool conflictsWithMap(const UnitModel& unit);
       
       static bool generatePath(const UnitModel& unit, WalkabilityPosition target);
       /** static help variables. */
       static Util::RectangleArray<std::pair<int, AdvancedDirection> > temp;
       static std::vector<BW::Position> returnValue;
       static std::multimap<int, WalkabilityPosition> vawe;
       static std::vector<std::pair<int, int> > directionBuffer[BASIC_DIRECTION_COUNT];
       static void setDirectionBuffer(const UnitModel& unit);
       static bool canMove(const WalkabilityPosition& position, BasicDirection::Enum direction);
   };
}