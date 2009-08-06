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
    int mask = 2047;
    u16 index=target.getTarget() & mask;
    Unit* unit=(Unit*)((int)BWDATA_UnitNodeTable+(int)index*336);
    this->position=unit->position;
  }
  //----------------------------------------------------------------------------------------------------------
}

