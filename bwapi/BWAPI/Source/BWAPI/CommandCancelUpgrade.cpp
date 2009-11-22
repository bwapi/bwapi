#include "CommandCancelUpgrade.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelUpgrade::CommandCancelUpgrade(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelUpgrade::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing2;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelUpgrade::getType()
  {
    return BWAPI::CommandTypes::CancelUpgrade;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelUpgrade::describe()
  {
    return this->executors[0]->getName() + " cancelled upgrade";
  }
  //----------------------------------------------------------------------------------------------------------
}