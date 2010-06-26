#include "CommandLift.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLift::CommandLift(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLift::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists) continue;
      this->executors[i]->getRawDataLocal->orderID = BW::OrderID::BuildingLiftOff;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandLift::getType()
  {
    return BWAPI::CommandTypes::Lift;
  }
  //----------------------------------------------------------------------------------------------------------
};
