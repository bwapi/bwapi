#include "CommandCancelConstruction.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelConstruction::CommandCancelConstruction(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelConstruction::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing2;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandCancelConstruction::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelConstruction::describe()
  {
    return this->executors[0]->getName() + " cancelled construction";
  }
  //----------------------------------------------------------------------------------------------------------
}