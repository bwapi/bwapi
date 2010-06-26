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
      if (!this->executors[i]->_exists) continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Nothing;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelResearch::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
};
