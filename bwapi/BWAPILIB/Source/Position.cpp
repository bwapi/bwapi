#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>

#include <math.h>

namespace BWAPI
{
  namespace Positions
  {
    const Position Invalid(100000,100000);
    const Position None(100000,100001);
    const Position Unknown(100000,100002);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position()
  :x(0)
  ,y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position(const TilePosition& position)
  :x(position.x*TILE_SIZE)
  ,y(position.y*TILE_SIZE)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Position::Position(int x, int y)
  :x(x)
  ,y(y)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool Position::operator == (const Position& position) const
  {
    return this->x == position.x &&
           this->y == position.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool Position::operator != (const Position& position) const
  {
    return this->x != position.x ||
           this->y != position.y;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool Position::operator  < (const Position& position) const
  {
    return this->x < position.x ||
           (this->x==position.x && this->y < position.y);
  }
  //----------------------------------------------------------------------------------------------------------
  double Position::getDistance(const Position &position) const
  {
    return sqrt((long double)((long double)this->x - position.x)*((long double)this->x - position.x) +
                     (long double)((long double)this->y - position.y)*((long double)this->y - position.y));
  }
  //----------------------------------------------------------------------------------------------------------
};
