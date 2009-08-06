#include "CommandCloak.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCloak::CommandCloak(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCloak::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Cloak;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandCloak::getType()
  {
    return BWAPI::CommandTypes::Cloak;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCloak::describe()
  {
    return this->executors[0]->getName() + " cloaked";
  }
  //----------------------------------------------------------------------------------------------------------
}