#include "Utilities.h"

#include <Util/RectangleArray.h>
#include <Util/Strings.h>
#include <BWAI/Unit.h>
#include <BWAI/Globals.h>
#include <BWAI/ai.h>
#include <BWAPI/Map.h>


namespace PathFinding
{
  Util::RectangleArray<std::pair<int, AdvancedDirection> > Utilities::temp(1,1);
  std::vector<BW::Position> Utilities::returnValue;
  std::multimap<int, WalkabilityPosition> Utilities::vawe;
  std::vector<std::pair<int, int> > Utilities::directionBuffer[BASIC_DIRECTION_COUNT];
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
    Utilities::vawe.clear();
    temp.resize(BWAPI::Map::getWidth()*4 + 2, BWAPI::Map::getHeight()*4 + 2);
    temp.setTo(std::pair<int, AdvancedDirection>(-1, AdvancedDirection(AdvancedDirectionID::Up)));
    temp.setBorderTo(std::pair<int, AdvancedDirection>(-2, AdvancedDirection(AdvancedDirectionID::Up)));
    Utilities::setDirectionBuffer(unit);
    /** First sample pathfinding to go here*/
    if (Utilities::conflictsWithMap(unit))
    {
      FILE* f = fopen("Path.txt","at");
      fprintf(f, "Conflicts");
      fclose(f); 
      return false;
    }
    for (int x = unit.walkabilityPosition.x; x <= (unit.position.x + 7)/8; x++)
      for (int y = unit.walkabilityPosition.y; y <= (unit.position.y + 7)/8; y++)
      {
        WalkabilityPosition position(x, y);
        int distance = position.toBWPosition().getDistance(unit.position);
        Utilities::vawe.insert(std::pair<int, WalkabilityPosition>(distance, position));
        temp[position.x][position.y] = std::pair<int, AdvancedDirection>(distance, AdvancedDirectionID::Near);
      }
    bool couldGo[BASIC_DIRECTION_COUNT];
    std::pair<int, WalkabilityPosition> result;
    while (!vawe.empty())
    {
      std::multimap<int, WalkabilityPosition>::iterator spotIterator = vawe.begin();
      std::pair<int, WalkabilityPosition> spot = *spotIterator;
      if (spot.second == target)
      {
        result = spot;
        break;
      }
      for (int i = 0; i < BASIC_DIRECTION_COUNT; i++)
        if (Utilities::canMove(spot.second, (BasicDirection::Enum)i))
        {
          couldGo[i] = true;
          WalkabilityPosition next = WalkabilityPosition(spot.second.x + forwardDirection[i][0],
                                                         spot.second.y + forwardDirection[i][1]);
          if (temp[next.x][next.y].first == -1 ||
              temp[next.x][next.y].first > spot.first + 8)
          {
            vawe.insert(std::pair<int, WalkabilityPosition>(spot.first + 8, next));
            temp[next.x][next.y].first = spot.first + 8;
            temp[next.x][next.y].second = AdvancedDirection((AdvancedDirectionID::Enum)i);
          }
        }
        else 
          couldGo[i] = false;
      vawe.erase(spotIterator);
    }
    if (!vawe.empty())
    {
      FILE* f = fopen("Path.txt","at");
 
      Util::RectangleArray<char> resultArray = 
          Util::RectangleArray<char>(BWAI::ai->map->getWalkabilityArray().getWidth(), 
                                     BWAI::ai->map->getWalkabilityArray().getHeight());
      for (unsigned int x = 0; x < BWAI::ai->map->getWalkabilityArray().getWidth(); x++)
        for (unsigned int y = 0; y < BWAI::ai->map->getWalkabilityArray().getHeight(); y++)
          resultArray[x][y] = BWAI::ai->map->getWalkabilityArray()[x][y] ? '.' : 'X';
      
      for (WalkabilityPosition i(target);
           temp[i.x][i.y].second != AdvancedDirectionID::Near;
           i.x += reverseDirection[temp[i.x][i.y].second.direction][0],
           i.y += reverseDirection[temp[i.x][i.y].second.direction][1])
        resultArray[i.x][i.y] = char(178);
      Util::Strings::makeBorder(resultArray).printToFile(f); 
      fclose(f);          
      return true;
    }
    else
    {
      FILE* f = fopen("Path.txt","at");
      fprintf(f, "Path not found");
      fclose(f);          
      return false;
    }
  }
  //------------------------------------------------------------------------------------------------
  bool Utilities::canMove(const WalkabilityPosition& position, BasicDirection::Enum direction)
  {
    bool result = true;
    for (size_t i = 0; i < directionBuffer[direction].size(); i++)
    {
      if (position.x + directionBuffer[direction][i].first < 0 ||
          position.x + directionBuffer[direction][i].first >= BWAI::ai->map->getWidth() ||
          position.y + directionBuffer[direction][i].second < 0 ||
          position.y + directionBuffer[direction][i].second >= BWAI::ai->map->getHeight())
        return false;
      result &= BWAI::ai->map->getWalkabilityArray()[position.x + directionBuffer[direction][i].first]
                                                    [position.y + directionBuffer[direction][i].second];
    }                                                    
    return result;                                                    
  }
  //------------------------------------------------------------------------------------------------
  void Utilities::setDirectionBuffer(const UnitModel& unit)
  {
    for (int i = 0; i < BASIC_DIRECTION_COUNT; i++)
      directionBuffer[i].clear();
    
    int x1 = (-unit.original->getType().dimensionLeft())/8;
    int x2 = ((unit.original->getType().dimensionRight()) + 7)/8;
    int y1 = (-unit.original->getType().dimensionUp())/8;
    int y2 = ((unit.original->getType().dimensionDown()) + 7)/8;
    for (int i = y1; i <= y2; i++)
    {
      directionBuffer[BasicDirection::Left].push_back(std::pair<int, int>(x1 - 1,i));
      directionBuffer[BasicDirection::Right].push_back(std::pair<int, int>(x2 + 1,i));
    }

    for (int i = x1; i <= x2; i++)
    {
      directionBuffer[BasicDirection::Up].push_back(std::pair<int, int>(i, y1 - 1));
      directionBuffer[BasicDirection::Down].push_back(std::pair<int, int>(i, y2 + 1));
    }
  }
  //------------------------------------------------------------------------------------------------
}