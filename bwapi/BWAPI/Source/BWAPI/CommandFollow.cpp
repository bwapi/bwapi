#include "CommandFollow.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandFollow::CommandFollow(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandFollow::execute()
  {
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    executors[0]->self->order = BW::OrderID::Follow;
    executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandFollow::getType()
  {
    return BWAPI::CommandTypes::Follow;
  }
  //----------------------------------------------------------------------------------------------------------
};
