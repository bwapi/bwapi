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
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelResearch::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
};
