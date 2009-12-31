#include "CommandReturnCargo.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandReturnCargo::CommandReturnCargo(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandReturnCargo::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        if (this->executors[i]->isCarryingGas())
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::ReturnGas;
        else
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::ReturnMinerals;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandReturnCargo::getType()
  {
    return BWAPI::CommandTypes::Stop;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandReturnCargo::describe()
  {
    return this->executors[0]->getName() + " returned cargo";
  }
  //----------------------------------------------------------------------------------------------------------
};
