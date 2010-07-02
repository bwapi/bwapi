#include "CommandUnsiege.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnsiege::CommandUnsiege(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnsiege::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::TankMode;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnsiege::getType()
  {
    return BWAPI::CommandTypes::Unsiege;
  }
  //----------------------------------------------------------------------------------------------------------
};
