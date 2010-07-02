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
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelUpgrade::getType()
  {
    return BWAPI::CommandTypes::CancelUpgrade;
  }
  //----------------------------------------------------------------------------------------------------------
};
