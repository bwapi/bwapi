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
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canAttack()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::AttackUnit;
        this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandAttackUnit::getType()
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