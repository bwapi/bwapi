#include "CommandCancelAddon.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelAddon::CommandCancelAddon(UnitImpl* executor)
      : Command(executor)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelAddon::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelAddon::getType()
  {
    return BWAPI::CommandTypes::CancelAddon;
  }
  //----------------------------------------------------------------------------------------------------------
};
