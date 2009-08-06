#include "CommandSiege.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandSiege::CommandSiege(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandSiege::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::SiegeMode;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandSiege::getType()
  {
    return BWAPI::CommandTypes::Siege;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandSiege::describe()
  {
    return this->executors[0]->getName() + " sieged";
  }
  //----------------------------------------------------------------------------------------------------------
}