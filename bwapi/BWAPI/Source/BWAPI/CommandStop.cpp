#include "CommandStop.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandStop::CommandStop(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandStop::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Stop;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandStop::getType()
  {
    return BWAPI::CommandTypes::Stop;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandStop::describe()
  {
    return this->executors[0]->getName() + " stopped";
  }
  //----------------------------------------------------------------------------------------------------------
};
