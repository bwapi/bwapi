#include "CommandUnload.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnload::CommandUnload(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnload::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Unload;
    executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnload::getType()
  {
    return BWAPI::CommandTypes::Unload;
  }
  //----------------------------------------------------------------------------------------------------------
};
