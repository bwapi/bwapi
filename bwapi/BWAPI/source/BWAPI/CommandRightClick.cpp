#include "CommandRightClick.h"
#include "UnitPrototype.h"
#include "UnitPrototypeDefinitions.h"
#include "Unit.h"
#include "../BW/Unit.h"
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
             (this->executors[i]->getPrototype()->isWorker()) && 
              targetUnit->isMineral()
           )
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::MoveToMinerals;
 
        else if ((this->executors[i]->getPrototype()->canAttack()) && 
                 targetUnit->getOwner() != executors[i]->getOwner())
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::AttackUnit;

        else if ((this->executors[i]->getPrototype()->canMove()))
          executors[i]->getRawDataLocal()->orderID = BW::OrderID::Move;

        executors[i]->getRawDataLocal()->targetUnit = targetUnit->getOriginalRawData();
      }
      else // targetUnit == NULL -> targetPosition is relevant
      {
       if ((this->executors[i]->getPrototype()->canMove()))
         executors[i]->getRawDataLocal()->orderID = BW::OrderID::Move;
      }
    }
  }
  //--------------------------------- GET TYPE --------------------------------
  BWAPI::CommandTypes::Enum CommandRightClick::getType()
  {
    return BWAPI::CommandTypes::RightClick;
  }
  //---------------------------------------------------------------------------
  std::string CommandRightClick::describe()
  {
    if (this->targetUnit != NULL)
      return this->executors[0]->getName() + " right-clicked on (" + this->targetUnit->getPrototype()->getName() + ")";
    else
      return this->executors[0]->getName() + " right-clicked to position";
  }
  //---------------------------------------------------------------------------
}