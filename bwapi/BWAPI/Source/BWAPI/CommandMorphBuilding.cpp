#include "CommandMorphBuilding.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandMorphBuilding::CommandMorphBuilding(UnitImpl* executor, BW::UnitType toMorph)
      : Command(executor)
      , toMorph(toMorph)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandMorphBuilding::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists)
        continue;

      this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::ZergBuildingMorph;
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
    p->spend(this->toMorph.mineralPrice(), this->toMorph.gasPrice());
    p->useSupplies(toMorph.supplyRequired(), toMorph._getRace());
    p->planToMake(toMorph);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandMorphBuilding::getType()
  {
    return BWAPI::CommandTypes::MorphBuilding;
  }
  //----------------------------------------------------------------------------------------------------------
};
