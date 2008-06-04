#include "Utilities.h"

#include <Util/RectangleArray.h>
#include <Util/Strings.h>
#include <Util/Logger.h>
#include <BWAI/Unit.h>
#include <BWAI/Globals.h>
#include <BWAI/ai.h>
#include <BWAPI/Map.h>


namespace PathFinding
{
  std::vector<BW::Position> Utilities::returnValue;
//  std::vector<std::pair<int, int> > Utilities::directionBuffer[BASIC_DIRECTION_COUNT];
  //------------------------------------- CONSTRUCTOR ----------------------------------------------
  Utilities::Utilities()
  :world      (BWAPI::Map::getWidth()*4 + 2, BWAPI::Map::getWidth()*4 + 2)
  ,walkability(BWAPI::Map::getWidth()*4 + 2, BWAPI::Map::getWidth()*4 + 2)
  ,vaweID(0)
  {
    Spot pattern;
    pattern.distance = 0; // This really doesn't matter
    pattern.vaweID = 0;
    pattern.from = Direction::Unset;
    world.setTo(pattern);
    for (int x = 0; x < BWAPI::Map::getWidth()*4; x++)
      for (int y = 0; y < BWAPI::Map::getHeight()*4; y++)
        walkability[x + 1][y + 1] = BWAI::ai->map->getWalkabilityArray()[x][y];
    for (unsigned int x = 0; x < walkability.getWidth(); x++)
    {
      walkability[x][0] = false;
      walkability[x][walkability.getHeight() - 1] = false;
    }
    
    for (unsigned int y = 0; y < walkability.getHeight(); y++)
    {
      walkability[0][y] = false;
      walkability[walkability.getWidth() - 1][y] = false;
    }
    for (int i = 0; i < SPOT_DISTANCE_WINDOW_SIZE; i++)
      count[i] = 0;
  }
  //--------------------------------- CONFLICTS WITH MAP -------------------------------------------
  bool Utilities::conflictsWithMap(const UnitModel& unit)
  {
    const Util::RectangleArray<bool>* walkability = &BWAI::ai->map->getWalkabilityArray();
    int x1 = (unit.position.x - unit.original->getType().dimensionLeft())/8;
    int x2 = ((unit.position.x + unit.original->getType().dimensionRight()) + 7)/8;
    int y1 = (unit.position.y - unit.original->getType().dimensionUp())/8;
    int y2 = ((unit.position.y + unit.original->getType().dimensionDown()) + 4)/8;
    for (int x = x1; x <= x2; x++)
      for (int y = y1; y <= y2; y++)
      {
        if (!(*walkability)[x][y])
           return true;
      }           
    return false;
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::generatePath(const UnitModel& unit, WalkabilityPosition target)
  {
    this->vaweID++;
    Utilities::setDirectionBuffer(unit);
    #pragma region DisabledDebugOutput
   /* if (Utilities::conflictsWithMap(unit))
    {
      FILE* f = fopen("Path.txt","at");
      fprintf(f, "Conflicts\n");
      fclose(f); 
      return false;
    }*/
    #pragma endregion DisabledDebugOutput    
    // sets starting spots of the search
    for (int i = 0; i < SPOT_DISTANCE_WINDOW_SIZE; i++)
      count[i] = 0;
    for (int x = unit.walkabilityPosition.x; x <= (unit.position.x + 7)/8; x++)
      for (int y = unit.walkabilityPosition.y; y <= (unit.position.y + 7)/8; y++)
      {
        WalkabilityPosition here(x, y);
        int distance = here.toBWPosition().getDistance(unit.position);
        here.x ++;
        here.y ++;
        world[here.x][here.y].distance = distance;
        world[here.x][here.y].vaweID = vaweID;
        world[here.x][here.y].from = Direction::Near;
        vawe[distance][count[distance]] = here;
        count[distance]++;
      }
    bool couldGo[BASIC_DIRECTION_COUNT];
    u16 countOfStepsWithoutChanges;
    Spot spot;
    WalkabilityPosition here;
    WalkabilityPosition next;
    u16 newDistancePared, newDistanceAngledPared;
    u16 newDistance, newDistanceAngled;
    
    for (u8 position = 0, distance = 0; true; position = (position + 1) & SPOT_DISTANCE_WINDOW_SIZE_BITS, distance++)
    {
      if (vawe[position])
        countOfStepsWithoutChanges = 0;
      else
        countOfStepsWithoutChanges++;

      newDistance = distance + STRAIGHT_SPOT_DIRECTION;
      newDistancePared = newDistance & SPOT_DISTANCE_WINDOW_SIZE_BITS;
      newDistanceAngled = distance + ANGLED_SPOT_DIRECTION;
      newDistanceAngledPared = newDistanceAngled & SPOT_DISTANCE_WINDOW_SIZE_BITS;

      for (int i = 0; i < count[position]; i++)
      {
        if (vawe[position][i] == target)
          goto foundTarget;
        here = vawe[position][i];
        spot = world[here.x][here.y];
        
        for (u8 j = 0; j < BASIC_DIRECTION_COUNT; j++)
        {
          if (this->canMove(here, (Direction::Enum)j))
          {
            couldGo[j] = true;
            next.x = here.x + forwardDirection[j][0];
            next.y = here.y + forwardDirection[j][1];
            if (world[next.x][next.y].vaweID != this->vaweID ||
                world[next.x][next.y].distance > newDistance)
            {
              vawe[newDistancePared][count[newDistancePared]] = next;
              count[newDistancePared]++;
              world[next.x][next.y].vaweID = this->vaweID;
              world[next.x][next.y].from = (Direction::Enum) j;
              world[next.x][next.y].distance = newDistance;
            }
          }
          else 
            couldGo[j] = false;
        }
        for (u8 j = BASIC_DIRECTION_COUNT; j < ADVANCED_DIRECTION_COUNT; j++)       
          if (couldGo[directionConditions[j][0]] &&
              couldGo[directionConditions[j][1]])
          {
            next.x = here.x + forwardDirection[j][0];
            next.y = here.y + forwardDirection[j][1];
            if (world[next.x][next.y].vaweID != this->vaweID ||
                world[next.x][next.y].distance > newDistanceAngled)
            {
              vawe[newDistanceAngledPared][count[newDistanceAngledPared]] = next;
              count[newDistanceAngledPared]++;
              world[next.x][next.y].vaweID = this->vaweID;
              world[next.x][next.y].from = (Direction::Enum) j;
              world[next.x][next.y].distance = newDistanceAngled;
            }
          }
      }  
     count[position] = 0;
     if (countOfStepsWithoutChanges >= SPOT_DISTANCE_WINDOW_SIZE)
       break;
    }
    #pragma region DisabledDebugOutput
    /* FILE* f = fopen("Path.txt","at");
    fprintf(f, "Path not found\n");
    
    Util::RectangleArray<char> resultArray = 
        Util::RectangleArray<char>(BWAI::ai->map->getWalkabilityArray().getWidth(), 
                                   BWAI::ai->map->getWalkabilityArray().getHeight());
    for (unsigned int x = 0; x < BWAI::ai->map->getWalkabilityArray().getWidth(); x++)
      for (unsigned int y = 0; y < BWAI::ai->map->getWalkabilityArray().getHeight(); y++)
        if (temp[x][y].first == -1)
          resultArray[x][y] = BWAI::ai->map->getWalkabilityArray()[x][y] ? '.' : 'X';
        else
          resultArray[x][y] = 'O';
    Util::Strings::makeBorder(resultArray).printToFile(f); 
    fclose(f); */
    #pragma endregion DisabledDebugOutput
    return false;
    foundTarget:
    #pragma region DisabledDebugOutput
    /*
    FILE* f = fopen("Path.txt","at");
 
    Util::RectangleArray<char> resultArray = 
        Util::RectangleArray<char>(BWAI::ai->map->getWalkabilityArray().getWidth(), 
                                   BWAI::ai->map->getWalkabilityArray().getHeight());
    for (unsigned int x = 0; x < BWAI::ai->map->getWalkabilityArray().getWidth(); x++)
      for (unsigned int y = 0; y < BWAI::ai->map->getWalkabilityArray().getHeight(); y++)
        resultArray[x][y] = BWAI::ai->map->getWalkabilityArray()[x][y] ? '.' : 'X';
    
    for (WalkabilityPosition i(target);
         world[i.x][i.y].from != Direction::Near;
         i.x += reverseDirection[world[i.x][i.y].from][0],
         i.y += reverseDirection[world[i.x][i.y].from][1])
      resultArray[i.x - 1][i.y - 1] = '0';
    Util::Strings::makeBorder(resultArray).printToFile(f); 
    fclose(f);*/
    #pragma endregion DisabledDebugOutput
    return true;
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::canMove(const WalkabilityPosition& position, Direction::Enum direction)
  {
    bool result = true;
    for (size_t i = 0; i < directionBuffer[direction].size(); i++)
      result &= this->walkability[position.x + directionBuffer[direction][i].first]
                                 [position.y + directionBuffer[direction][i].second];
    return result;                                                     
  }
  //------------------------------------------------------------------------------------------------
  void Utilities::setDirectionBuffer(const UnitModel& unit)
  {
    for (int i = 0; i < BASIC_DIRECTION_COUNT; i++)
      directionBuffer[i].clear();
    
    int x1 = (-unit.original->getType().dimensionLeft() - 7)/8;
    int x2 = ((unit.original->getType().dimensionRight()) + 7)/8;
    int y1 = (-unit.original->getType().dimensionUp() - 7)/8;
    int y2 = ((unit.original->getType().dimensionDown()) + 7)/8;
    
    for (int i = y1; i <= y2; i++)
    {
      directionBuffer[Direction::Left].push_back(std::pair<int, int>(x1 - 1,i));
      directionBuffer[Direction::Right].push_back(std::pair<int, int>(x2 + 1,i));
    }

    for (int i = x1; i <= x2; i++)
    {
      directionBuffer[Direction::Up].push_back(std::pair<int, int>(i, y1 - 1));
      directionBuffer[Direction::Down].push_back(std::pair<int, int>(i, y2 + 1));
    }
  }
  //------------------------------------------------------------------------------------------------
}