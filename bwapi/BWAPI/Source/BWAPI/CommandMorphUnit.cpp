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
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Morph1;
      }
    }
    PlayerImpl* p = static_cast<PlayerImpl*>(this->executors[0]->getPlayer());
    p->spend(this->toMorph.mineralPrice(), this->toMorph.gasPrice());
    p->useSupplies(toMorph.supplyRequired(), toMorph.getRace());
    p->planToMake(toMorph);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandMorphUnit::getType()
  {
    return BWAPI::CommandTypes::MorphUnit;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandMorphUnit::describe()
  {
    return this->executors[0]->getName() + " is morphing into (" + this->toMorph.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
}