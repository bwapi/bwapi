#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>

#include <math.h>

namespace BWAPI
{
  namespace Positions
  {
    const Position Invalid(100000, 100000);
    const Position None(100000, 100001);
    const Position Unknown(100000, 100002);
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
  bool __cdecl Position::operator == (const Position& position) const
  {
    return this->x() == position.x() &&
           this->y() == position.y();
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool __cdecl Position::operator != (const Position& position) const
  {
    return this->x() != position.x() ||
           this->y() != position.y();
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool __cdecl Position::operator  < (const Position& position) const
  {
    return this->x() < position.x() ||
           (this->x() == position.x() && this->y() < position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  Position __cdecl Position::operator+(const Position& position) const
  {
    return Position(this->x() + position.x(), this->y() + position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  Position __cdecl Position::operator-(const Position& position) const
  {
    return Position(this->x() - position.x(), this->y() - position.y());
  }
  //----------------------------------------------------------------------------------------------------------
  Position& __cdecl Position::operator+=(const Position& position)
  {
    this->x() += position.x();
    this->y() += position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  Position& __cdecl Position::operator-=(const Position& position)
  {
    this->x() -= position.x();
    this->y() -= position.y();
    return *this;
  }
  //----------------------------------------------------------------------------------------------------------
  double __cdecl Position::getDistance(const Position& position) const
  {
    return ((*this) - position).getLength();
  }
  //----------------------------------------------------------------------------------------------------------
  double __cdecl Position::getLength() const
  {
    double x = this->x();
    double y = this->y();
    return sqrt(x * x + y * y);
  }
  //----------------------------------------------------------------------------------------------------------
  int& __cdecl Position::x()
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int& __cdecl Position::y()
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
  int __cdecl Position::x() const
  {
    return this->_x;
  }
  //----------------------------------------------------------------------------------------------------------
  int __cdecl Position::y() const
  {
    return this->_y;
  }
  //----------------------------------------------------------------------------------------------------------
};
