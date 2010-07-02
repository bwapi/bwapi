#include "CommandDecloak.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandDecloak::CommandDecloak(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandDecloak::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Decloak;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandDecloak::getType()
  {
    return BWAPI::CommandTypes::Decloak;
  }
  //----------------------------------------------------------------------------------------------------------
};
