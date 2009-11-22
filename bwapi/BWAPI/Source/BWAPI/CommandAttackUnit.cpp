#include "CommandAttackUnit.h"
#include "UnitImpl.h"
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
    if (this->target==NULL || !this->target->_exists()) return;
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canAttack()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::AttackUnit;
        this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandAttackUnit::getType()
  {
    return BWAPI::CommandTypes::AttackUnit;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandAttackUnit::describe()
  {
    return this->executors[0]->getName() + " attacked unit";
  }
  //----------------------------------------------------------------------------------------------------------
}