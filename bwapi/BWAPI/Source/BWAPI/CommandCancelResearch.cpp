#include "CommandCancelResearch.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelResearch::CommandCancelResearch(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelResearch::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing2;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandCancelResearch::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelResearch::describe()
  {
    return this->executors[0]->getName() + " cancelled research";
  }
  //----------------------------------------------------------------------------------------------------------
}