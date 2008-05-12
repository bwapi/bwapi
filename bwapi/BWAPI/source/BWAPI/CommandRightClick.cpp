#include "CommandRightClick.h"
#include "UnitPrototype.h"
#include "UnitPrototypeDefinitions.h"
#include "Unit.h"
#include "../BW/Unit.h"
#include "AbilityFlags.h"
namespace BWAPI
{
  //-------------------------------- CONSTRUCTOR ------------------------------
  CommandRightClick::CommandRightClick(Unit* executor, const BW::Position& targetPosition)
  :Command(executor)
  ,targetPosition(targetPosition)
  ,targetUnit(NULL)
  {
  }
  //-------------------------------- CONSTRUCTOR ------------------------------
  CommandRightClick::CommandRightClick(Unit* executor, BWAPI::Unit* targetUnit)
  :Command(executor)
  ,targetUnit(targetUnit)
  {
  }
  //---------------------------------- EXECUTE --------------------------------
  void CommandRightClick::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (targetUnit != NULL)
      {
        if (
             (this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) 
             && 
             (
               targetUnit->getPrototype()->getUnitID() == BW::UnitType::Resource_MineralPatch1 ||
               targetUnit->getPrototype()->getUnitID() == BW::UnitType::Resource_MineralPatch2 ||
               targetUnit->getPrototype()->getUnitID() == BW::UnitType::Resource_MineralPatch3
             )
           )
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::ApproachingMinerals;
 
        else if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Attack) && 
                 targetUnit->getOwner() != executors[i]->getOwner())
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Attacking;

        else if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Move))
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Moving;

        executors[i]->getRawDataLocal()->targetUnit = targetUnit->getOriginalRawData();
      }
      else // targetUnit == NULL -> targetPosition is relevant
      {
       if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Move))
         executors[i]->getRawDataLocal()->orderID = BW::OrderID::Moving;
      }
    }
  }
  //--------------------------------- GET TYPE --------------------------------
  BWAPI::CommandTypes::Enum CommandRightClick::getType()
  {
    return BWAPI::CommandTypes::RightClick;
  }
  //---------------------------------------------------------------------------
}