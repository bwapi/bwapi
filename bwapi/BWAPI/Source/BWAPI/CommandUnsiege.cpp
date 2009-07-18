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
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::TankMode;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandUnsiege::getType()
  {
    return BWAPI::CommandTypes::Unsiege;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandUnsiege::describe()
  {
    return this->executors[0]->getName() + " unsieged";
  }
  //----------------------------------------------------------------------------------------------------------
}