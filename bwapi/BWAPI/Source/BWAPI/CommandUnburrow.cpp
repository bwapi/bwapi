#include "CommandUnburrow.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnburrow::CommandUnburrow(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnburrow::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Unburrow;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnburrow::getType()
  {
    return BWAPI::CommandTypes::Unburrow;
  }
  //----------------------------------------------------------------------------------------------------------
};
