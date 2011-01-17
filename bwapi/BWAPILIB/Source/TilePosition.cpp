#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>
#include <BWAPI/Game.h>

#include <math.h>

#include "../../Debug.h"

namespace BWAPI
{
  namespace TilePositions
  {
    const TilePosition Invalid(1000, 1000);
    const TilePosition None(1000, 1001);
    const TilePosition Unknown(1000, 1002);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition()
      : _x(0)
      , _y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(int x, int y)
      : _x(x)
      , _y(y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(const Position& position)
      : _x(position.x() / TILE_SIZE)
      , _y(position.y() / TILE_SIZE)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TilePosition::operator == (const TilePosition& TilePosition) const
  {
    return this->x() == TilePosition.x() &&
           this->y() == TilePosition.y();
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool TilePosition::operator != (const TilePosition& TilePosition) const
  {
    return this->x() != TilePosition.x() ||
           this->y() != TilePosition.y();
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool TilePosition::operator < (const TilePosition& TilePosition) const
  {
    return this->x() < TilePosition.x() ||
           (this->x() == TilePosition.x() && this->y() < TilePosition.y());
  }
  //---------------------------------------------- IS VALID --------------------------------------------------
  bool TilePosition::isValid() const
  {
    return (_x >= 0 && _y >= 0 && _x < Broodwar->mapWidth() && _y < Broodwar->mapHeight());
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition TilePosition::operator+(const TilePosition& position) const
  {
    return TilePosition(this->x() + position.x(), this->y() + position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition TilePosition::operator-(const TilePosition& position) const
  {
    return TilePosition(this->x() - position.x(), this->y() - position.y());
  }
  //-------------------------------------------- MAKE VALID --------------------------------------------------
  TilePosition& TilePosition::makeValid()
  {
    if (_x > Broodwar->mapWidth() - 1)
      _x = Broodwar->mapWidth() - 1;
    if (_y > Broodwar->mapHeight() - 1)
      _y = Broodwar->mapHeight() - 1;
    if (_x < 0)
      _x = 0;
    if (_y < 0)
      _y = 0;
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition& TilePosition::operator+=(const TilePosition& position)
  {
    this->x() += position.x();
    this->y() += position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition& TilePosition::operator-=(const TilePosition& position)
  {
    this->x() -= position.x();
    this->y() -= position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  double TilePosition::getDistance(const TilePosition& position) const
  {
    return ((*this) - position).getLength();
  }
  //----------------------------------------------------------------------------------------------------------
  double TilePosition::getLength() const
  {
    double x = this->x();
    double y = this->y();
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
  int& TilePosition::x()
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int& TilePosition::y()
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
  int TilePosition::x() const
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int TilePosition::y() const
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
};
