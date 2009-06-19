#include "WalkabilityPosition.h"

namespace PathFinding
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WalkabilityPosition::WalkabilityPosition(u16 x, u16 y)
  :x(x)
  ,y(y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  WalkabilityPosition::WalkabilityPosition(BWAPI::Position position)
  :x(position.x()/8)
  ,y(position.y()/8)
  {
  }
  //-------------------------------------------- TO BW::Position ---------------------------------------------
  BWAPI::Position WalkabilityPosition::toBWAPIPosition()
  {
    return BWAPI::Position(this->x*8, this->y*8);
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WalkabilityPosition::operator==(const WalkabilityPosition& position)
  {
    return this->x == position.x &&
           this->y == position.y;
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool WalkabilityPosition::operator!=(const WalkabilityPosition& position)
  {
    return this->x != position.x ||
           this->y != position.y;
  }
  //----------------------------------------------------------------------------------------------------------
  WalkabilityPosition::WalkabilityPosition(const WalkabilityPosition& position)
  :x(position.x)
  ,y(position.y)
  {
  }
  //----------------------------------------------------------------------------------------------------------
  WalkabilityPosition::WalkabilityPosition()
  :x(0)
  ,y(0)
  {
  }  
  //----------------------------------------------------------------------------------------------------------
}