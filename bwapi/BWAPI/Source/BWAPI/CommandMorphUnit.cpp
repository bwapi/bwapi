#include "CommandMorphUnit.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandMorphUnit::CommandMorphUnit(UnitImpl* executor, BW::UnitType toMorph)
      : Command(executor)
      , toMorph(toMorph)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandMorphUnit::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::ZergUnitMorph;
    executors[0]->self->buildType = toMorph.getID();
    executors[0]->self->remainingBuildTime = toMorph.buildTime();
    executors[0]->self->isMorphing = true;
    executors[0]->self->isConstructing = true;
    executors[0]->self->isIdle = false;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
    {
      p->spend(toMorph.mineralPrice(), toMorph.gasPrice());
      p->planToMake(toMorph);
    }
    if (toMorph.isTwoUnitsInOneEgg())
      p->useSupplies(toMorph.supplyRequired()*2-UnitType(toMorph.getID()).whatBuilds().first.supplyRequired(), toMorph._getRace());
    else
      p->useSupplies(toMorph.supplyRequired()-UnitType(toMorph.getID()).whatBuilds().first.supplyRequired(), toMorph._getRace());

    if (toMorph==BW::UnitID::Zerg_Lurker)
      executors[0]->self->type = BW::UnitID::Zerg_LurkerEgg;
    else if (toMorph==BW::UnitID::Zerg_Devourer || toMorph==BW::UnitID::Zerg_Guardian)
      executors[0]->self->type = BW::UnitID::Zerg_Cocoon;
    else
      executors[0]->self->type = BW::UnitID::Zerg_Egg;

  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandMorphUnit::getType()
  {
    return BWAPI::CommandTypes::MorphUnit;
  }
  //----------------------------------------------------------------------------------------------------------
};
