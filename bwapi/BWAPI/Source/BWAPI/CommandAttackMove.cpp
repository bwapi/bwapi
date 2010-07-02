#include "CommandAttackMove.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandAttackMove::CommandAttackMove(UnitImpl* executor, const BW::Position& targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandAttackMove::execute()
  {
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canMove()) return;
    executors[0]->self->order  = BW::OrderID::AttackMove;
    executors[0]->self->targetPositionX = targetPosition.x;
    executors[0]->self->targetPositionY = targetPosition.y;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandAttackMove::getType()
  {
    return BWAPI::CommandTypes::AttackMove;
  }
  //----------------------------------------------------------------------------------------------------------
};
