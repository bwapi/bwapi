#include "CommandFollow.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandFollow::CommandFollow(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandFollow::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Follow;
        this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandFollow::getType()
  {
    return BWAPI::CommandTypes::Follow;
  }
  //----------------------------------------------------------------------------------------------------------
};
