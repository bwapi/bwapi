#include "CommandMorphBuilding.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandMorphBuilding::CommandMorphBuilding(UnitImpl* executor, BW::UnitType toMorph)
      : Command(executor)
      , toMorph(toMorph)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandMorphBuilding::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::ZergBuildingMorph;
    executors[0]->self->buildType = toMorph.getID();
    executors[0]->self->remainingBuildTime = toMorph.buildTime();
    executors[0]->self->isMorphing = true;
    executors[0]->self->isConstructing = true;
    executors[0]->self->isIdle = false;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    p->spend(toMorph.mineralPrice(), toMorph.gasPrice());
    p->planToMake(toMorph);
    p->useSupplies(toMorph.supplyRequired(), toMorph._getRace());
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandMorphBuilding::getType()
  {
    return BWAPI::CommandTypes::MorphBuilding;
  }
  //----------------------------------------------------------------------------------------------------------
};
