#include <BWAPI.h>

#include "Unit.h"
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
    return BWAI::ai->getUnit(unit);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::UnitType Unit::getType() const
  {
    return this->unit->getType();
  }
  //------------------------------------------- GET HEALTH POINTS --------------------------------------------
  int Unit::getHealthPoints() const
  {
    return this->unit->health();
  }
  //------------------------------------------- GET SHIELD POINTS --------------------------------------------
  int Unit::getShieldPoints() const
  {
    return this->unit->shield();
  }
  //------------------------------------------- GET ENERGY POINTS --------------------------------------------
  int Unit::getEnergyPoints() const
  {
    return this->unit->energy();
  }
  //--------------------------------------------- GET KILL COUNT ---------------------------------------------
  int Unit::getKillCount() const
  {
    return this->unit->getKillCount();
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BWAPI::Position Unit::getPosition() const
  {
    return this->unit->getPosition();
  }
  //------------------------------------------- GET TILE POSITION --------------------------------------------
  const BWAPI::TilePosition Unit::getTilePosition() const
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
  BWAPI::Position Unit::getTargetPosition() const
  {
    return this->unit->getTargetPosition();
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  int Unit::getDistance(Unit *unit) const
  {
    u32 result;
    if (unit == this)
      return 0;
    if (this->getPosition().y() - this->getType().dimensionUp() <= unit->getPosition().y() + unit->getType().dimensionDown())
      if (this->getPosition().y() + this->getType().dimensionDown() >= unit->getPosition().y() - unit->getType().dimensionUp())
        if (this->getPosition().x() > unit->getPosition().x())
          result = this->getPosition().x() - this->getType().dimensionLeft() - unit->getPosition().x() - unit->getType().dimensionRight();
        else
          result = unit->getPosition().x() - unit->getType().dimensionRight() - this->getPosition().x() - this->getType().dimensionLeft();

    if (this->getPosition().x() - this->getType().dimensionLeft() <= unit->getPosition().x() + unit->getType().dimensionRight())
      if (this->getPosition().x() + this->getType().dimensionRight() >= unit->getPosition().x() - unit->getType().dimensionLeft())
        if (this->getPosition().y() > unit->getPosition().y())
          result = this->getPosition().y() - this->getType().dimensionUp() - unit->getPosition().y() - unit->getType().dimensionDown();
        else
          result = unit->getPosition().y() - unit->getType().dimensionDown() - this->getPosition().y() - this->getType().dimensionUp();

    if (this->getPosition().x() > unit->getPosition().x())
      if (this->getPosition().y() > unit->getPosition().y())
        result = (u32)BWAPI::Position(this->getPosition().x() - this->getType().dimensionLeft(),
                              this->getPosition().y() - this->getType().dimensionUp())
                 .getDistance(BWAPI::Position(unit->getPosition().x() + unit->getType().dimensionRight(),
                                           unit->getPosition().y() + unit->getType().dimensionDown()));
      else
        result = (u32)BWAPI::Position(this->getPosition().x() - this->getType().dimensionLeft(),
                              this->getPosition().y() + this->getType().dimensionDown())
                 .getDistance(BWAPI::Position(unit->getPosition().x() + unit->getType().dimensionRight(),
                                           unit->getPosition().y() - unit->getType().dimensionUp()));
    else
      if (this->getPosition().y() > unit->getPosition().y())
        result = (u32)BWAPI::Position(this->getPosition().x() + this->getType().dimensionRight(),
                              this->getPosition().y() - this->getType().dimensionUp())
                 .getDistance(BWAPI::Position(unit->getPosition().x() - unit->getType().dimensionLeft(),
                                           unit->getPosition().y() + unit->getType().dimensionDown()));
      else
        result = (u32)BWAPI::Position(this->getPosition().x() + this->getType().dimensionRight(),
                              this->getPosition().y() + this->getType().dimensionDown())
                 .getDistance(BWAPI::Position(unit->getPosition().x() - unit->getType().dimensionLeft(),
                                           unit->getPosition().y() - unit->getType().dimensionUp()));
    if (result > 0)
      return result;
    else
      return 0;
  }
  //---------------------------------------------- GET DISTANCE ----------------------------------------------
  int Unit::getDistance(BWAPI::Position position) const
  {
    return (int)this->unit->getPosition().getDistance(position);
  }
  //------------------------------------------- GET CENTER DISTANCE ------------------------------------------
  int Unit::getCenterDistance(Unit *unit) const
  {
    return (int)this->unit->getPosition().getDistance(unit->getPosition());
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  BWAPI::Order Unit::getOrder() const
  {
    return this->unit->getOrder();
  }
  //-------------------------------------------- GET ORDER TARGET --------------------------------------------
  BWAPI::Order Unit::getSecondaryOrder() const
  {
    return this->unit->getSecondaryOrder();
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  char position[100];
  char indexName[50];
  char targetIndex[50];
  char orderTargetIndex[50];
  char owner[100];
  char unitName[100];
  char orderName[100];
  char connectedUnit[100];
  char message[400];
  std::string Unit::getName() const
  {
    sprintf_s(position, 100, "Position = (%d,%d)", this->unit->getPosition().x(), 
                                              this->unit->getPosition().y());

    sprintf_s(indexName, 50, "[%d]", (int)(this->unit));

    if (this->getTarget() == NULL)
      strcpy_s(targetIndex, 50, "Target:[NULL]");
    else
      sprintf_s(targetIndex, 50, "Target:[%d](%s)", (int)(this->getTarget()), this->getTarget()->getType().getName().c_str());

    if (this->getOrderTarget() == NULL)
      strcpy_s(orderTargetIndex, 50, "OrderTarget:[NULL]");
    else
      sprintf_s(orderTargetIndex, 50, "OrderTarget:[%d](%s)", (int)(this->getOrderTarget()), this->getOrderTarget()->getType().getName().c_str());
  
    if (this->getOwner() != NULL)
      sprintf_s(owner, 100, "Player = (NULL)");
    else
      sprintf_s(owner, 100, "Player = (%s)", this->unit->getOwner()->getName().c_str());

    sprintf_s(unitName, 100, "(%s)", this->getType().getName().c_str());

    if (this->unit->getChild() == NULL)
      sprintf_s(connectedUnit, 100, "(childUnit1 = NULL)");
    else
      sprintf_s(connectedUnit, 100, "(childUnit1 = %s)", this->unit->getChild()->getType().getName().c_str());

    sprintf_s(orderName, 100, "(%s)", this->getOrder().getName().c_str());
    sprintf_s(message, 400, "%s %s %s %s %s %s %s %s", unitName,
                                              orderName,
                                              indexName,
                                              position,
                                              targetIndex,
                                              orderTargetIndex,
                                              owner,
                                              connectedUnit);

    return std::string(message);
  }
  //----------------------------------------------- IS MINERAL -----------------------------------------------
  bool Unit::isMineral() const
  {
    return this->unit->getType()==BWAPI::UnitTypes::Resource_Mineral_Field;
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
  //----------------------------------------------- IS MOVING ------------------------------------------------
  bool Unit::isMoving() const
  {
    return this->unit->isMoving();
  }
  //-------------------------------------------- IS ACCELERATING ---------------------------------------------
  bool Unit::isAccelerating() const
  {
    return this->unit->isAccelerating();
  }
  //----------------------------------------------- IS BRAKING -----------------------------------------------
  bool Unit::isBraking() const
  {
    return this->unit->isBraking();
  }
  //------------------------------------------ IS STARTING ATTACK --------------------------------------------
  bool Unit::isStartingAttack() const
  {
    return this->unit->isStartingAttack();
  }
  //------------------------------------------------ IS BLIND ------------------------------------------------
  bool Unit::isBlind() const
  {
    return this->unit->isBlind();
  }
  //--------------------------------------------- IS BEING HEALED --------------------------------------------
  bool Unit::isBeingHealed() const // Not working right now
  {
    return this->unit->isBeingHealed();
  }
  //--------------------------------------------- IS UNDER STORM ---------------------------------------------
  bool Unit::isUnderStorm() const
  {
    return this->unit->isUnderStorm();
  }
  //------------------------------------------------ GET UNIT ------------------------------------------------
  BWAPI::Unit* Unit::getUnit()
  {
    return this->unit;
  }
  //----------------------------------------------- IS TRAINING ----------------------------------------------
  bool Unit::isTraining() const
  {
    return this->unit->isTraining();
  }
  //-------------------------------------------- GET TRAINING QUEUE ------------------------------------------
  std::list<BWAPI::UnitType > Unit::getTrainingQueue() const
  {
    return this->unit->getTrainingQueue();
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

  void Unit::orderAttackMove(BWAPI::Position position)
  {
    return this->unit->attackMove(position);
  }
  void Unit::orderRightClick(BWAPI::Position position)
  {
    return this->unit->rightClick(position);
  }
  void Unit::orderRightClick(Unit *target)
  {
    this->unit->rightClick(target->getUnit());
  }
  void Unit::orderPatrol(BWAPI::Position position)
  {
    this->unit->patrol(position);
  }
  void Unit::stop()
  {
    this->unit->stop();
  }
  void Unit::holdPosition()
  {
    this->unit->holdPosition();
  }
  void Unit::useTech(BWAPI::TechType tech)
  {
    this->unit->useTech(tech);
  }
  void Unit::trainUnit(BWAPI::UnitType type)
  {
    this->unit->train(type);
  }
  void Unit::build(BWAPI::TilePosition position, BWAPI::UnitType type)
  {
    this->unit->build(position,type);
  }
  void Unit::invent(BWAPI::TechType tech)
  {
    this->unit->invent(tech);
  }
  void Unit::upgrade(BWAPI::UpgradeType upgrade)
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