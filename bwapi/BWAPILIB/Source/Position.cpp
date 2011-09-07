#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>
#include <BWAPI/Game.h>

#include <math.h>
#include <algorithm>

#include "../../Debug.h"

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
    if ( _x < 0 || _y < 0 )
      return false;
    if ( !Broodwar )
      return true;
    return _x < Broodwar->mapWidth()*32 && _y < Broodwar->mapHeight()*32;
  }
  Position::operator bool() const
  {
    return this->isValid();
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
    if (_x < 0)
      _x = 0;
    if (_y < 0)
      _y = 0;

    if ( !Broodwar )
      return *this;

    if (_x > Broodwar->mapWidth()*32 - 1)
      _x = Broodwar->mapWidth()*32 - 1;
    if (_y > Broodwar->mapHeight()*32 - 1)
      _y = Broodwar->mapHeight()*32 - 1;
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
    unsigned int min = abs(_x - position.x());
    unsigned int max = abs(_y - position.y());
    if ( max < min )
      std::swap<unsigned int>(min,max);

    if ( min < (max >> 2) )
      return max;

    unsigned int minCalc = (3*min) >> 3;
    return (minCalc >> 5) + minCalc + max - (max >> 4) - (max >> 6);
    // Simplifies to (99*min + 236*max)/256;
  }
  //----------------------------------------------------------------------------------------------------------
  double Position::getLength() const
  {
    double x = this->x();
    double y = this->y();
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
  bool Position::hasPath(const Position& destination) const
  {
    if ( !Broodwar )
      return false;
    return Broodwar->hasPath(*this, destination);
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
