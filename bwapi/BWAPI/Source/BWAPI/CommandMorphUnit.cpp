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
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists)
        continue;
      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::ZergUnitMorph;
    }
    int slotToAffect = this->executors[0]->getBuildQueueSlot();
    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
      slotToAffect  = (slotToAffect + 1) % 5;

    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
    {
      this->failed = true;
      return;
    }

    executors[0]->getBuildQueue()[slotToAffect] = this->toMorph.getID();
    this->executors[0]->getRawDataLocal()->buildQueueSlot = (u8)slotToAffect;
    PlayerImpl* p = static_cast<PlayerImpl*>(this->executors[0]->getPlayer());
    executors[0]->getRawDataLocal()->remainingBuildTime = (u16)this->toMorph.buildTime();
    if (toMorph==BW::UnitID::Zerg_Lurker)
      executors[0]->getRawDataLocal()->unitID = BW::UnitID::Zerg_LurkerEgg;
    else if (toMorph==BW::UnitID::Zerg_Devourer || toMorph==BW::UnitID::Zerg_Guardian)
      executors[0]->getRawDataLocal()->unitID = BW::UnitID::Zerg_Cocoon;
    else
      executors[0]->getRawDataLocal()->unitID = BW::UnitID::Zerg_Egg;
    p->spend(this->toMorph.mineralPrice(), this->toMorph.gasPrice());
    p->useSupplies(toMorph.supplyRequired(), toMorph._getRace());
    p->planToMake(toMorph);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandMorphUnit::getType()
  {
    return BWAPI::CommandTypes::MorphUnit;
  }
  //----------------------------------------------------------------------------------------------------------
};
