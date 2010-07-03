#include "CommandCancelResearch.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelResearch::CommandCancelResearch(UnitImpl* executor)
      : Command(executor)
  {
    startFrame = Broodwar->getFrameCount();
    techType = executor->getTech();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelResearch::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
    TechType tech(executors[0]->self->tech);
    executors[0]->self->tech = TechTypes::None.getID();
    executors[0]->self->isIdle = true;
    executors[0]->self->remainingResearchTime = 0;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      p->spend(-tech.mineralPrice(),-tech.gasPrice());
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelResearch::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
};
