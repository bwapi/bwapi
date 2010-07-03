#include "CommandCancelMorph.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelMorph::CommandCancelMorph(UnitImpl* executor)
      : Command(executor)
  {
    startFrame = Broodwar->getFrameCount();
    morphType = executor->getBuildType();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelMorph::execute()
  {
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
    {
      p->spend(-morphType.mineralPrice(), -morphType.gasPrice());
      //-p->planToMake(toMorph);
    }
    if (morphType.isTwoUnitsInOneEgg())
      p->useSupplies(-(morphType.supplyRequired()*2-morphType.whatBuilds().first.supplyRequired()), (u8)morphType.getRace().getID());
    else
      p->useSupplies(-(morphType.supplyRequired()-morphType.whatBuilds().first.supplyRequired()), (u8)morphType.getRace().getID());

    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
    executors[0]->self->type = morphType.whatBuilds().first.getID();
    executors[0]->self->buildType = UnitTypes::None.getID();
    executors[0]->self->remainingBuildTime = 0;
    executors[0]->self->isMorphing = false;
    executors[0]->self->isConstructing = false;
    executors[0]->self->isIdle = true;

  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelMorph::getType()
  {
    return BWAPI::CommandTypes::CancelMorph;
  }
  //----------------------------------------------------------------------------------------------------------
};
