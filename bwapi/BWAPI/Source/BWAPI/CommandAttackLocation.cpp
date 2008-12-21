#include "CommandAttackLocation.h"
#include "Unit.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandAttackLocation::CommandAttackLocation(Unit* executor, const BW::Position& targetPosition)
  :Command(executor)
  ,targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandAttackLocation::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canMove()))
        {
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::AttackMove;
          executors[i]->getRawDataLocal()->position = this->targetPosition;
        }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandAttackLocation::getType()
  {
    return BWAPI::CommandTypes::AttackLocation;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandAttackLocation::describe()
  {
	  return this->executors[0]->getName() + " attacked location";
  }
  //----------------------------------------------------------------------------------------------------------
}