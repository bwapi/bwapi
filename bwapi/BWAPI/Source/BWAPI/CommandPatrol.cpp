#include "CommandPatrol.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandPatrol::CommandPatrol(UnitImpl* executor, const BW::Position& targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandPatrol::execute()
  {
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    executors[0]->self->order = BW::OrderID::Patrol;
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandPatrol::getType()
  {
    return BWAPI::CommandTypes::Patrol;
  }
  //----------------------------------------------------------------------------------------------------------
};
