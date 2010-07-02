#include "CommandBurrow.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandBurrow::CommandBurrow(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandBurrow::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Burrow;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandBurrow::getType()
  {
    return BWAPI::CommandTypes::Burrow;
  }
  //----------------------------------------------------------------------------------------------------------
};
