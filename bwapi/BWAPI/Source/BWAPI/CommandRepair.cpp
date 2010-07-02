#include "CommandRepair.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandRepair::CommandRepair(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandRepair::execute()
  {
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    executors[0]->self->order = BW::OrderID::Repair1;
    executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandRepair::getType()
  {
    return BWAPI::CommandTypes::Repair;
  }
  //----------------------------------------------------------------------------------------------------------
};
