#include "CommandPatrol.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandPatrol::CommandPatrol(UnitImpl* executor, const BW::Position& targetPosition)
  :Command(executor)
  ,targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandPatrol::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canMove()))
        {
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Patrol;
          executors[i]->getRawDataLocal()->position = this->targetPosition;
        }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandPatrol::getType()
  {
    return BWAPI::CommandTypes::AttackLocation;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandPatrol::describe()
  {
	  return this->executors[0]->getName() + " patroled to location";
  }
  //----------------------------------------------------------------------------------------------------------
}