#include <BWAPI/PositionUnit.h>
#include <BWAPI/Unit.h>

#define ISUNIT 0x89ABCDEF

namespace BWAPI
{
  // ctors
  PositionOrUnit::PositionOrUnit(Position pos)
  {
    this->position.x = pos.x;
    this->position.y = pos.y;
  }
  PositionOrUnit::PositionOrUnit(Unit pUnit)
  {
    this->unit.pUnit = pUnit;
    this->unit.y = ISUNIT;
  }

  // operators
  PositionOrUnit &PositionOrUnit::operator =(Unit pUnit)
  {
    this->unit.pUnit = pUnit;
    this->unit.y = ISUNIT;
    return *this;
  }
  PositionOrUnit &PositionOrUnit::operator =(Position pos)
  {
    this->position.x = pos.x;
    this->position.y = pos.y;
    return *this;
  }

  // members (unit)
  bool PositionOrUnit::isUnit() const
  {
    return this->unit.y == ISUNIT;
  }
  Unit PositionOrUnit::getUnit() const
  {
    if ( this->isPosition() )
      return nullptr;
    return this->unit.pUnit;
  }

  // members (position)
  bool PositionOrUnit::isPosition() const
  {
    return !this->isUnit();
  }
  Position PositionOrUnit::getPosition() const
  {
    if ( this->isPosition() )
      return Position(this->position.x, this->position.y);
    if ( this->getUnit() )
      return this->getUnit()->getPosition();
    return Positions::None;
  }
}

