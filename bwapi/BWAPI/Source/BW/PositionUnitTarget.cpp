#include "PositionUnitTarget.h"
#include "Unit.h"
namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const Position& position)
      : position(position)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitTarget::PositionUnitTarget(const UnitTarget& target)
      : target(target)
  {
    u16 index = 336 * (target.getTarget() & 0x7FF - 1);
    Unit* unit = (Unit*)((int)BW::BWDATA_UnitNodeTable + index);
    this->position = unit->position;
  }
  //----------------------------------------------------------------------------------------------------------
}

