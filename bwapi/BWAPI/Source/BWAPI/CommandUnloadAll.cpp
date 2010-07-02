#include "CommandUnloadAll.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnloadAll::CommandUnloadAll(UnitImpl* executor)
      : Command(executor)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnloadAll::CommandUnloadAll(UnitImpl* executor, BW::Position targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnloadAll::execute()
  {
    if (!executors[0]->_exists) return;
    if (executors[0]->getType() == UnitTypes::Terran_Bunker)
    {
      executors[0]->self->order = BW::OrderID::Unload;
    }
    else
    {
      executors[0]->self->order = BW::OrderID::MoveUnload;
      executors[0]->self->targetPositionX = targetPosition.x;
      executors[0]->self->targetPositionY = targetPosition.y;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnloadAll::getType()
  {
    return BWAPI::CommandTypes::UnloadAll;
  }
  //----------------------------------------------------------------------------------------------------------
};
