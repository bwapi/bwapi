#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>
#include <BWAPI/Game.h>

#include <math.h>

namespace BWAPI
{
  namespace Positions
  {
    const Position Invalid(32000, 32000);
    const Position None(32000, 32032);
    const Position Unknown(32000, 32064);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position()
      : _x(0)
      , _y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position(const TilePosition& position)
      : _x(position.x()*TILE_SIZE)
      , _y(position.y()*TILE_SIZE)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Position::Position(int x, int y)
      : _x(x)
      , _y(y)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool Position::operator == (const Position& position) const
  {
    return this->x() == position.x() &&
           this->y() == position.y();
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool Position::operator != (const Position& position) const
  {
    return this->x() != position.x() ||
           this->y() != position.y();
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool Position::operator  < (const Position& position) const
  {
    return this->x() < position.x() ||
           (this->x() == position.x() && this->y() < position.y());
  }
  //---------------------------------------------- IS VALID --------------------------------------------------
  bool Position::isValid() const
  {
    return (_x >= 0 && _y >= 0 && _x < Broodwar->mapWidth()*32 && _y < Broodwar->mapHeight()*32);
  }
  //----------------------------------------------------------------------------------------------------------
  Position Position::operator+(const Position& position) const
  {
    return Position(this->x() + position.x(), this->y() + position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  Position Position::operator-(const Position& position) const
  {
    return Position(this->x() - position.x(), this->y() - position.y());
  }
  //-------------------------------------------- MAKE VALID --------------------------------------------------
  Position& Position::makeValid()
  {
    if (_x > Broodwar->mapWidth()*32 - 1)
      _x = Broodwar->mapWidth()*32 - 1;
    if (_y > Broodwar->mapHeight()*32 - 1)
      _y = Broodwar->mapHeight()*32 - 1;
    if (_x < 0)
      _x = 0;
    if (_y < 0)
      _y = 0;
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  Position& Position::operator+=(const Position& position)
  {
    this->x() += position.x();
    this->y() += position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  Position& Position::operator-=(const Position& position)
  {
    this->x() -= position.x();
    this->y() -= position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  double Position::getDistance(const Position& position) const
  {
    return ((*this) - position).getLength();
  }
  //----------------------------------------------------------------------------------------------------------
  int Position::getApproxDistance(const Position& position) const
  {
    unsigned int min = abs(this->x() - position.x());
    unsigned int max = abs(this->y() - position.y());
    if (max < min)
    {
      unsigned int temp = min;
      min = max;
      max = temp;
    }
    if (min < (max >> 4))
      return max;
    return ((3 * min) >> 8) + ((3 * min) >> 3) + max - (max >> 4) - (max >> 6);
  }
  //----------------------------------------------------------------------------------------------------------
  double Position::getLength() const
  {
    double x = this->x();
    double y = this->y();
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
  int& Position::x()
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int& Position::y()
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
  int Position::x() const
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int Position::y() const
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
};
