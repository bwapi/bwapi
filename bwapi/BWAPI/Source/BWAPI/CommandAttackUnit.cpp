#include "CommandAttackUnit.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandAttackUnit::CommandAttackUnit(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandAttackUnit::execute()
  {
    if (target==NULL || !target->_exists) return;
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canAttack()) return;
    executors[0]->self->order = BW::OrderID::AttackUnit;
    executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandAttackUnit::getType()
  {
    return BWAPI::CommandTypes::AttackUnit;
  }
  //----------------------------------------------------------------------------------------------------------
};
