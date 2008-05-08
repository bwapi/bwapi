#include "CommandRightClick.h"
#include "UnitPrototype.h"
#include "UnitPrototypeDefinitions.h"
#include "Unit.h"
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
        if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Gather) && 
            targetUnit->getPrototype()->getUnitID() == BWAPI::Prototypes::Minerals->getUnitID())
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::ApproachingMinerals;
 
        else if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Attack) && 
                 targetUnit->getOwner() != executors[i]->getOwner())
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Attacking;

        else if ((this->executors[i]->getPrototype()->getAbilityFlags() & BWAPI::AbilityFlags::Move))
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Moving;
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