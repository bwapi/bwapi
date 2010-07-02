#include "CommandCancelMorph.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelMorph::CommandCancelMorph(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelMorph::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelMorph::getType()
  {
    return BWAPI::CommandTypes::CancelMorph;
  }
  //----------------------------------------------------------------------------------------------------------
};
