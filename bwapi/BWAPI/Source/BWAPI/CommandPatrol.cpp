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
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Patrol;
        this->executors[i]->getRawDataLocal()->position = this->targetPosition;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandPatrol::getType()
  {
    return BWAPI::CommandTypes::Patrol;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandPatrol::describe()
  {
    return this->executors[0]->getName() + " patroled to location";
  }
  //----------------------------------------------------------------------------------------------------------
};
