#include "CommandUnburrow.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnburrow::CommandUnburrow(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnburrow::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Unburrow;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnburrow::getType()
  {
    return BWAPI::CommandTypes::Unburrow;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandUnburrow::describe()
  {
    return this->executors[0]->getName() + " unburrowed";
  }
  //----------------------------------------------------------------------------------------------------------
}