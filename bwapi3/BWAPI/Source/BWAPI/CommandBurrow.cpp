#include "CommandBurrow.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandBurrow::CommandBurrow(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandBurrow::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Burrow;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandBurrow::getType()
  {
    return BWAPI::CommandTypes::Burrow;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandBurrow::describe()
  {
    return this->executors[0]->getName() + " burrowed";
  }
  //----------------------------------------------------------------------------------------------------------
};
