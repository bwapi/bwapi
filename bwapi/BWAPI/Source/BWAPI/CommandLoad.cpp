#include "CommandLoad.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandLoad::CommandLoad(UnitImpl* executor, UnitImpl* target)
      : Command(executor)
      , target(target)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandLoad::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists()) continue;
      if (this->executors[i]->getType() == UnitTypes::Terran_Bunker)
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Pickup3;
        this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
      else if (this->executors[i]->getType() == UnitTypes::Terran_Dropship
               || this->executors[i]->getType() == UnitTypes::Protoss_Shuttle
               || this->executors[i]->getType() == UnitTypes::Zerg_Overlord)
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Pickup2;
        this->executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
      else if (this->target->getType() == UnitTypes::Terran_Bunker
               || this->target->getType() == UnitTypes::Terran_Dropship
               || this->target->getType() == UnitTypes::Protoss_Shuttle
               || this->target->getType() == UnitTypes::Zerg_Overlord)
      {
        executors[i]->getRawDataLocal()->orderID = BW::OrderID::EnterTransport;
        executors[i]->getRawDataLocal()->targetUnit = this->target->getOriginalRawData();
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandLoad::getType()
  {
    return BWAPI::CommandTypes::Load;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandLoad::describe()
  {
    return this->executors[0]->getName() + " loaded unit";
  }
  //----------------------------------------------------------------------------------------------------------
}