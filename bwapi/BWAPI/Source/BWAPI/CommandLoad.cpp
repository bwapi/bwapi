#include "CommandLoad.h"
#include "UnitImpl.h"
#include "GameImpl.h"
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
    if (!executors[0]->_exists) return;
    if (executors[0]->getType() == UnitTypes::Terran_Bunker)
    {
      executors[0]->self->order = BW::OrderID::PickupBunker;
      executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
    }
    else if (executors[0]->getType() == UnitTypes::Terran_Dropship
          || executors[0]->getType() == UnitTypes::Protoss_Shuttle
          || executors[0]->getType() == UnitTypes::Zerg_Overlord)
    {
      executors[0]->self->order = BW::OrderID::PickupTransport;
      executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
    }
    else if (target->getType() == UnitTypes::Terran_Bunker
          || target->getType() == UnitTypes::Terran_Dropship
          || target->getType() == UnitTypes::Protoss_Shuttle
          || target->getType() == UnitTypes::Zerg_Overlord)
    {
      executors[0]->self->order = BW::OrderID::EnterTransport;
      executors[0]->self->target = BroodwarImpl.server.getUnitID(target);
    }
    executors[0]->self->isIdle = false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandLoad::getType()
  {
    return BWAPI::CommandTypes::Load;
  }
  //----------------------------------------------------------------------------------------------------------
};
