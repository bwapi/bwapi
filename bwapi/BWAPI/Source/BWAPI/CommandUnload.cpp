#include "CommandUnload.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnload::CommandUnload(UnitImpl* executor, UnitImpl* target)
  :Command(executor)
  ,target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnload::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Unload;
      this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandUnload::getType()
  {
    return BWAPI::CommandTypes::Unload;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandUnload::describe()
  {
	  return this->executors[0]->getName() + " unloaded unit";
  }
  //----------------------------------------------------------------------------------------------------------
}