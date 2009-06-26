#include "CommandLand.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLand::CommandLand(UnitImpl* executor, const BW::TilePosition& targetPosition)
  :Command(executor)
  ,targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLand::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::BuildingLand;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandLand::getType()
  {
    return BWAPI::CommandTypes::Land;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandLand::describe()
  {
	  return this->executors[0]->getName() + " landed";
  }
  //----------------------------------------------------------------------------------------------------------
}