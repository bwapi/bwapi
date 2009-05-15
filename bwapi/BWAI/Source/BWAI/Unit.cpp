#include "Unit.h"

#include <BWAPI/Unit.h>

#include "Task.h"
#include "Globals.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :unit(unit)
  ,task(0)
  ,lastFrameSpam(0)
  ,expansion(0)
  {
  }
  //------------------------------------------ BW Unit TO BWAI Unit ------------------------------------------
  Unit* Unit::BWAPIUnitToBWAIUnit(BWAPI::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return BWAI::ai->getUnit(unit->getIndex());
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BW::UnitType Unit::getType() const
  {
    return this->unit->getType();
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int Unit::getHealthPoints() const
  {
    return this->unit->getHealthPoints();
  }
  //------------------------------------------- GET SHIELD POINTS --------------------------------------------
  int Unit::getShieldPoints() const
  {
    return this->unit->getShieldPoints();
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BW::Position& Unit::getPosition() const
  {
    return this->unit->getPosition();
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  BW::TilePosition Unit::getTilePosition() const
  {
    return this->unit->getTilePosition();
  }
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* Unit::getTarget() const
  {
    return Unit::BWAPIUnitToBWAIUnit(this->unit->getTarget());
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  Unit* Unit::getOrderTarget() const
  {
    return Unit::BWAPIUnitToBWAIUnit(this->unit->getOrderTarget());
  }
  //--------------------------------------------- GET BUILD UNIT ---------------------------------------------
  Unit* Unit::getBuildUnit() const
  {
    return Unit::BWAPIUnitToBWAIUnit(this->unit->getBuildUnit());
  }
  //----------------------------------------------- GET CHILD ------------------------------------------------
  Unit* Unit::getChild() const
  {
    return Unit::BWAPIUnitToBWAIUnit(this->unit->getChild());
  }
  //------------------------------------------ GET TARGET POSITION -------------------------------------------
  BW::Position Unit::getTargetPosition() const
  {
    return this->unit->getTargetPosition();
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  int Unit::getDistance(Unit *unit) const
  {
    return (int)(this->unit->getDistance(unit->getUnit()));
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  int Unit::getDistance(BW::Position position) const
  {
    return (int)(this->unit->getDistance(position));
  }
  //------------------------------------------- GET CENTER DISTANCE ------------------------------------------
  int Unit::getCenterDistance(Unit *unit) const
  {
    return (int)this->unit->getCenterDistance(unit->getUnit());
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  BW::OrderID::Enum Unit::getOrderID() const
  {
    return this->unit->getOrderID();
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  BW::OrderID::Enum Unit::getSecondaryOrderID() const
  {
    return this->unit->getSecondaryOrderID();
  }
  //----------------------------------------------- GET INDEX ------------------------------------------------
  int Unit::getIndex() const
  {
    return this->unit->getIndex();
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Unit::getName() const
  {
    return this->unit->getName();
  }
  //----------------------------------------------- IS MINERAL -----------------------------------------------
  bool Unit::isMineral() const
  {
    return this->unit->isMineral();
  }
  //---------------------------------------------- IS COMPLETED ----------------------------------------------
  bool Unit::isCompleted() const
  {
    return this->unit->isCompleted();
  }
  //----------------------------------------------- IS LIFTED ------------------------------------------------
  bool Unit::isLifted() const
  {
    return this->unit->isLifted();
  }
  //------------------------------------------------ IS IDLE -------------------------------------------------
  bool Unit::isIdle() const
  {
    return this->unit->isIdle();
  }
  //------------------------------------------------ GET UNIT ------------------------------------------------
  BWAPI::Unit* Unit::getUnit()
  {
    return this->unit;
  }
  //------------------------------------------ GET BUILD QUEUE SLOT ------------------------------------------
  int Unit::getBuildQueueSlot() const
  {
    return this->unit->getBuildQueueSlot();
  }
  //--------------------------------------------- GET BUILD QUEUE --------------------------------------------
  BW::UnitType* Unit::getBuildQueue() const
  {
    return this->unit->getBuildQueue();
  }
  //------------------------------------------ HAS EMPTY BUILD QUEUE -----------------------------------------
  bool Unit::hasEmptyBuildQueue() const
  {
    return this->unit->hasEmptyBuildQueue();
  }
  //------------------------------------------ HAS FULL BUILD QUEUE ------------------------------------------
  bool Unit::hasFullBuildQueue() const
  {
    return this->unit->hasFullBuildQueue();
  }
  //---------------------------------------- GET REMAINING BUILD TIME ----------------------------------------
  int Unit::getRemainingBuildTime() const
  {
    return this->unit->getRemainingBuildTime();
  }
  //--------------------------------------------- GET ORDER TIMER --------------------------------------------
  int Unit::getOrderTimer() const
  {
    return this->unit->getOrderTimer();
  }
  //------------------------------------------------ GET NEXT ------------------------------------------------
  Unit* Unit::getNext() const
  {
    #ifdef PARANOID_DEBUG
    #pragma warning(push)
    #pragma warning(disable:4311)
    if (this->getOriginalRawData()->nextUnit != NULL)
    {
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable)/BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
        BWAPI::Broodwar.fatalError->log("Unit array too small, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
      if ((int)this->getOriginalRawData()->nextUnit < (int)BW::BWXFN_UnitNodeTable)
        BWAPI::Broodwar.fatalError->log("Unit array begins at bad location, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
    }
    #pragma warning(push)
    #endif PARANOID_DEBUG
    return Unit::BWAPIUnitToBWAIUnit(this->unit->getNext());
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  Player* Unit::getOwner() const
  {
    return ai->getPlayer(this->unit->getOwner());
  }
  //----------------------------------------------- IS SELECTED ----------------------------------------------
  bool Unit::isSelected() const
  {
    return this->unit->isSelected();
  }

  void Unit::orderAttackLocation(BW::Position position, int OrderID)
  {
    return this->unit->orderAttackLocation(position, OrderID);
  }
  void Unit::orderRightClick(BW::Position position)
  {
    return this->unit->orderRightClick(position);
  }
  void Unit::orderRightClick(Unit *target)
  {
    this->unit->orderRightClick(target->getUnit());
  }
  void Unit::trainUnit(BW::UnitType type)
  {
    this->unit->trainUnit(type);
  }
  void Unit::build(BW::TilePosition position, BW::UnitType type)
  {
    this->unit->build(position,type);
  }
  void Unit::invent(BW::TechType tech)
  {
    this->unit->invent(tech);
  }
  void Unit::upgrade(BW::UpgradeType upgrade)
  {
    this->unit->upgrade(upgrade);
  }
  //------------------------------------------------ GET TASK ------------------------------------------------
  Task* Unit::getTask() const
  {
    return this->task;
  }
  //--------------------------------------------- FREE FROM TASK ---------------------------------------------
  void Unit::freeFromTask()
  {
    if (this->getTask())
      this->getTask()->freeExecutor(this);
  }
  //---------------------------------------------- REMOVE TASK -----------------------------------------------
  void Unit::clearTask()
  {
    this->task = NULL;
  }
  //------------------------------------------------ SET TASK ------------------------------------------------
  void Unit::setTask(Task* task)
  {
    this->task = task;
  }
  //----------------------------------------------------------------------------------------------------------
}