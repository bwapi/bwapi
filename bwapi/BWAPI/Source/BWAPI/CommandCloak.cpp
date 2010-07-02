#include "CommandCloak.h"
#include "UnitImpl.h"
#include <BWAPI/TechType.h>
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCloak::CommandCloak(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCloak::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Cloak;
    if (executors[0]->self->type==BW::UnitID::Terran_Ghost)
      executors[0]->self->energy-=BWAPI::TechTypes::Personnel_Cloaking.energyUsed();
    else
      executors[0]->self->energy-=BWAPI::TechTypes::Cloaking_Field.energyUsed();
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCloak::getType()
  {
    return BWAPI::CommandTypes::Cloak;
  }
  //----------------------------------------------------------------------------------------------------------
};
