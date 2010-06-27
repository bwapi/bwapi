#include "CommandStop.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandStop::CommandStop(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandStop::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Stop;
    executors[0]->self->isIdle = true;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandStop::getType()
  {
    return BWAPI::CommandTypes::Stop;
  }
  //----------------------------------------------------------------------------------------------------------
};
