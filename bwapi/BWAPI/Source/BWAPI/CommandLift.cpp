#include "CommandLift.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLift::CommandLift(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLift::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::BuildingLiftOff;
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandLift::getType()
  {
    return BWAPI::CommandTypes::Lift;
  }
  //----------------------------------------------------------------------------------------------------------
};
