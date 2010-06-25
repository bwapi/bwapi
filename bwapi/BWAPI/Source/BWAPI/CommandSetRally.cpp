#include "CommandSetRally.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandSetRally::CommandSetRally(UnitImpl* executor, BW::Position targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
      , targetUnit(NULL)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandSetRally::CommandSetRally(UnitImpl* executor, UnitImpl* targetUnit)
      : Command(executor)
      , targetPosition(0, 0)
      , targetUnit(targetUnit)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandSetRally::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canProduce()))
      {
        if (!this->executors[i]->_exists()) continue;
        if (this->targetUnit != NULL && this->targetUnit->_exists())
        {
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::RallyPointUnit;
          this->executors[i]->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyUnit = this->targetUnit->getOriginalRawData();
        }
        else
        {
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::RallyPointTile;
          this->executors[i]->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyX = this->targetPosition.x;
          this->executors[i]->getRawDataLocal()->rallyPsiProviderUnion.rally.rallyY = this->targetPosition.y;
        }
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandSetRally::getType()
  {
    return BWAPI::CommandTypes::SetRally;
  }
  //----------------------------------------------------------------------------------------------------------
};
