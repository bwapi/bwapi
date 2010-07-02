#include "CommandLand.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLand::CommandLand(UnitImpl* executor, const BW::TilePosition& targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLand::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::BuildingLand;
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandLand::getType()
  {
    return BWAPI::CommandTypes::Land;
  }
  //----------------------------------------------------------------------------------------------------------
};
