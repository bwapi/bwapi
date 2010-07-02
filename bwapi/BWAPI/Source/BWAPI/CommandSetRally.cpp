#include "CommandSetRally.h"
#include "UnitImpl.h"
#include "GameImpl.h"
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
    if (!executors[0]->_exists) return;
    if (!executors[0]->getType().canProduce()) return;
    if (targetUnit != NULL && targetUnit->_exists)
    {
      executors[0]->self->order = BW::OrderID::RallyPointUnit;
      executors[0]->self->rallyUnit = BroodwarImpl.server.getUnitID(targetUnit);
    }
    else
    {
      executors[0]->self->order = BW::OrderID::RallyPointTile;
      executors[0]->self->rallyPositionX = targetPosition.x;
      executors[0]->self->rallyPositionY = targetPosition.y;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandSetRally::getType()
  {
    return BWAPI::CommandTypes::SetRally;
  }
  //----------------------------------------------------------------------------------------------------------
};
