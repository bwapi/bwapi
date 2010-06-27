#include "CommandMorphUnit.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandMorphUnit::CommandMorphUnit(UnitImpl* executor, BW::UnitType toMorph)
      : Command(executor)
      , toMorph(toMorph)
  {
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
    p->spend(toMorph.mineralPrice(), toMorph.gasPrice());
    p->useSupplies(toMorph.supplyRequired(), toMorph._getRace());
    p->planToMake(toMorph);
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
