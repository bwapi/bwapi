#include "CommandMorphBuilding.h"
#include "UnitImpl.h"
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
      if (!this->executors[i]->_exists()) continue;
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Morph2;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandMorphBuilding::getType()
  {
    return BWAPI::CommandTypes::MorphBuilding;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandMorphBuilding::describe()
  {
    return this->executors[0]->getName() + " is morphing into (" + this->toMorph.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
}