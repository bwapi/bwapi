#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>

#include <math.h>

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
  bool __cdecl TilePosition::operator == (const TilePosition& TilePosition) const
  {
    return this->x() == TilePosition.x() &&
           this->y() == TilePosition.y();
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool __cdecl TilePosition::operator != (const TilePosition& TilePosition) const
  {
    return this->x() != TilePosition.x() ||
           this->y() != TilePosition.y();
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool __cdecl TilePosition::operator < (const TilePosition& TilePosition) const
  {
    return this->x() < TilePosition.x() ||
           (this->x() == TilePosition.x() && this->y() < TilePosition.y());
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition __cdecl TilePosition::operator+(const TilePosition& position) const
  {
    return TilePosition(this->x() + position.x(), this->y() + position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition __cdecl TilePosition::operator-(const TilePosition& position) const
  {
    return TilePosition(this->x() - position.x(), this->y() - position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition& __cdecl TilePosition::operator+=(const TilePosition& position)
  {
    this->x() += position.x();
    this->y() += position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  TilePosition& __cdecl TilePosition::operator-=(const TilePosition& position)
  {
    this->x() -= position.x();
    this->y() -= position.y();
    return *this;
  }
  //----------------------------------------------- IS VALID -------------------------------------------------
  bool __cdecl TilePosition::isValid() const
  {
    return (*this != TilePositions::Invalid && *this != TilePositions::None && *this != TilePositions::Unknown);
  }
  //----------------------------------------------------------------------------------------------------------
  double __cdecl TilePosition::getDistance(const TilePosition& position) const
  {
    return ((*this) - position).getLength();
  }
  //----------------------------------------------------------------------------------------------------------
  double __cdecl TilePosition::getLength() const
  {
    double x = this->x();
    double y = this->y();
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
  int& __cdecl TilePosition::x()
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int& __cdecl TilePosition::y()
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
  int __cdecl TilePosition::x() const
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int __cdecl TilePosition::y() const
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
};
