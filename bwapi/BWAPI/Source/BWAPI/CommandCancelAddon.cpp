#include "CommandCancelAddon.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelAddon::CommandCancelAddon(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelAddon::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing2;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelAddon::getType()
  {
    return BWAPI::CommandTypes::CancelAddon;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelAddon::describe()
  {
    return this->executors[0]->getName() + " cancelled addon";
  }
  //----------------------------------------------------------------------------------------------------------
};
