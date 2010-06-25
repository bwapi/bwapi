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
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::AttackMove;
        this->executors[i]->getRawDataLocal()->moveToPos = this->targetPosition;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandAttackMove::getType()
  {
    return BWAPI::CommandTypes::AttackMove;
  }
  //----------------------------------------------------------------------------------------------------------
};
