#include "PositionUnitTarget.h"
#include "CUnit.h"

#include "Offsets.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const Position& position)
      : position(position)
  {
  }
  PositionUnitTarget::PositionUnitTarget(int x, int y)
    : position(static_cast<s16>(x), static_cast<s16>(y))
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const UnitTarget& target)
      : target(target)
  {
    int index = (target.getTarget() & 0x7FF) - 1;
    CUnit* unit = &BWDATA::UnitNodeTable[index];
    this->position = unit->position;
  }
  //----------------------------------------------------------------------------------------------------------
}

