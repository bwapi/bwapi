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
    executors[0]->getRawDataLocal()->orderID = BW::OrderID::Cloak;
    if (executors[0]->getRawDataLocal()->unitID==BW::UnitID::Terran_Ghost)
      executors[0]->getRawDataLocal()->energy-=(u16)(BWAPI::TechTypes::Personnel_Cloaking.energyUsed()*256);
    else
      executors[0]->getRawDataLocal()->energy-=(u16)(BWAPI::TechTypes::Cloaking_Field.energyUsed()*256);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCloak::getType()
  {
    return BWAPI::CommandTypes::Cloak;
  }
  //----------------------------------------------------------------------------------------------------------
};
