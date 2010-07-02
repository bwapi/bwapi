#include "CommandReturnCargo.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandReturnCargo::CommandReturnCargo(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandReturnCargo::execute()
  {
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    if (executors[0]->isCarryingGas())
      executors[0]->self->order = BW::OrderID::ReturnGas;
    else
      executors[0]->self->order = BW::OrderID::ReturnMinerals;
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandReturnCargo::getType()
  {
    return BWAPI::CommandTypes::Stop;
  }
  //----------------------------------------------------------------------------------------------------------
};
