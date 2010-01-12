#include "PositionUnitID.h"
#include "Unit.h"
#include "UnitArray.h"
namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitID::PositionUnitID()
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitID::PositionUnitID(const Position& position)
      : position(position)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PositionUnitID::PositionUnitID(BW::UnitID target)
      : unitId(target)
  {
    BW::Unit* unit = & BW::BWDATA_UnitNodeTable->unit[target.getIndex()];
    this->position = unit->position;
  }
  //----------------------------------------------------------------------------------------------------------
}

