#include "CommandRepair.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandRepair::CommandRepair(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandRepair::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal->orderID = BW::OrderID::Repair1;
        this->executors[i]->getRawDataLocal->targetUnit = this->target->getOriginalRawData();
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandRepair::getType()
  {
    return BWAPI::CommandTypes::Repair;
  }
  //----------------------------------------------------------------------------------------------------------
};
