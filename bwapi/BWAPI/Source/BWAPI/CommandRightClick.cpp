#include "CommandRightClick.h"
#include "UnitImpl.h"
#include "GameImpl.h"
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
    if (!executors[0]->_exists) return;
    if (targetUnit != NULL && targetUnit->_exists)
    {
      executors[0]->self->target = BroodwarImpl.server.getUnitID(targetUnit);
      if (executors[0]->getType().isWorker() &&
          targetUnit->getType()==UnitTypes::Resource_Mineral_Field)
        executors[0]->self->order = BW::OrderID::MoveToMinerals;
      else if (executors[0]->getType().isWorker() &&
               targetUnit->getType().isRefinery())
        executors[0]->self->order = BW::OrderID::MoveToGas;
      else if (executors[0]->getType().isWorker() &&
               targetUnit->getType().getRace()==Races::Terran &&
               targetUnit->getType().whatBuilds().first==executors[0]->getType() &&
               !targetUnit->isCompleted())
      {
        executors[0]->self->order = BW::OrderID::ConstructingBuilding;
        executors[0]->self->buildUnit = BroodwarImpl.server.getUnitID(targetUnit);
        targetUnit->self->buildUnit = BroodwarImpl.server.getUnitID(executors[0]);
      }
      else if ((executors[0]->getType().canAttack()) &&
               targetUnit->getPlayer() != executors[0]->getPlayer() &&
               !targetUnit->getType().isNeutral())
        executors[0]->self->order = BW::OrderID::AttackUnit;
      else if ((this->executors[0]->getType().canMove()))
        executors[0]->self->order = BW::OrderID::Follow;
    }
    else // targetUnit == NULL -> targetPosition is relevant
    {
      if (isPosition && executors[0]->getType().canMove())
        executors[0]->self->order = BW::OrderID::Move;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandRightClick::getType()
  {
    return BWAPI::CommandTypes::RightClick;
  }
  //----------------------------------------------------------------------------------------------------------
};
