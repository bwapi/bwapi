#include "CommandRightClick.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandRightClick::CommandRightClick(UnitImpl* executor, const BW::Position& targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
      , targetUnit(NULL)
      , isPosition(true)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandRightClick::CommandRightClick(UnitImpl* executor, UnitImpl* targetUnit)
      : Command(executor)
      , targetUnit(targetUnit)
      , isPosition(false)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandRightClick::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if (!this->executors[i]->_exists) continue;
      if (targetUnit != NULL && targetUnit->_exists)
      {
        if (this->executors[i]->getType().isWorker() &&
            targetUnit->getType()==UnitTypes::Resource_Mineral_Field)
          executors[i]->getRawDataLocal->orderID = BW::OrderID::MoveToMinerals;

        else if (this->executors[i]->getType().isWorker() &&
                 targetUnit->getType().isRefinery())
          executors[i]->getRawDataLocal->orderID = BW::OrderID::MoveToGas;

        else if (this->executors[i]->getType().isWorker() &&
                 targetUnit->getType().getRace()==Races::Terran &&
                 targetUnit->getType().whatBuilds().first==this->executors[i]->getType() &&
                 !targetUnit->isCompleted())
        {
          executors[i]->getRawDataLocal->orderID = BW::OrderID::ConstructingBuilding;
          executors[i]->buildUnit=targetUnit;
          targetUnit->buildUnit=executors[i];
        }

        else if ((this->executors[i]->getType().canAttack()) &&
                 targetUnit->getPlayer() != executors[i]->getPlayer() &&
                 !targetUnit->getType().isNeutral())
          executors[i]->getRawDataLocal->orderID = BW::OrderID::AttackUnit;

        else if ((this->executors[i]->getType().canMove()))
          executors[i]->getRawDataLocal->orderID = BW::OrderID::Move;

        executors[i]->getRawDataLocal->targetUnit = targetUnit->getOriginalRawData();
      }
      else // targetUnit == NULL -> targetPosition is relevant
      {
        if (this->isPosition)
        {
          if ((this->executors[i]->getType().canMove()))
          {
            executors[i]->getRawDataLocal->orderID = BW::OrderID::Move;
          }
        }
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandRightClick::getType()
  {
    return BWAPI::CommandTypes::RightClick;
  }
  //----------------------------------------------------------------------------------------------------------
};
