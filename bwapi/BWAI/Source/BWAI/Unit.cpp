#include "Unit.h"

#include <Logger.h>

#include "Task.h"

#include "../../BWAPI/Source/Types.h"
#include "../../BWAPI/Source/BW/Offsets.h"
#include "../../BWAPI/Source/BW/Unit.h"
#include "../../BWAPI/Source/BWAPI/Globals.h"
#include "Globals.h"
namespace BWAI
{
  //-------------------------------- CONSTRUCTOR ------------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :BWAPI::Unit(*unit) /**< default autoconstructor usage */
  ,task(0)
  ,selected(false)
  ,lastTrainedUnit(BW::UnitID::None)
  ,lastFrameSpam(0)
  ,expansion(0)
  {
  }
  //----------------------------- BW Unit TO BWAI Unit -----------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return ai->getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  //----------------------------- BW Unit TO BWAI Unit -----------------------
  Unit* Unit::BWAPIUnitToBWAIUnit(BWAPI::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return BWAI::ai->getUnit(unit->getIndex());
  }
  #pragma warning (pop)
  //------------------------------- GET TARGET --------------------------------
  Unit* Unit::getTarget()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getRawData()->targetUnit);
  }
  //----------------------------- GET TARGET LOCAL --------------------------
  Unit* Unit::getTargetLocal()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getRawDataLocal()->targetUnit);
  }
  //------------------------------- GET TARGET --------------------------------
  Unit* Unit::getOrderTarget()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getRawData()->orderTargetUnit);
  }
  //----------------------------- GET TARGET LOCAL --------------------------
  Unit* Unit::getOrderTargetLocal()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getRawDataLocal()->orderTargetUnit);
  }
  //--------------------------------- GET NEXT ---------------------------------
  Unit *Unit::getNext()
  {
    return this->next;
  }
  //-------------------------------- UPDATE NEXT -------------------------------
  void Unit::updateNext()
  {
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
    this->next = Unit::BWUnitToBWAIUnit(this->getOriginalRawData()->nextUnit);
    if (this->next != NULL)
      this->next->updateNext();
  }
  //------------------------------- GET TASK ----------------------------------
  Task* Unit::getTask()
  {
    return this->task;
  }
  //------------------------------- REMOVE TASK -------------------------------
  void Unit::removeTask()
  {
    delete this->task;
    this->task = NULL;
  }
  //-------------------------------- SET TASK ---------------------------------
  void Unit::setTask(Task* task)
  {
    delete this->task;
    this->task = task;
  }
  //------------------------------ PERFORM TASK -------------------------------
  void Unit::performTask()
  {
    /*if (
         this->getTask() == NULL &&
         (
           this->getOrderIDLocal() == BW::OrderID::MoveToMinerals &&
           this->getOrderIDLocal() == BW::OrderID::HarvestMinerals2 &&
           this->getOrderIDLocal() == BW::OrderID::MiningMinerals
         )
       )
    this->orderStop(); */
    if (this->getTask() &&
        this->getTask()->execute())
      this->removeTask();
  }
  //---------------------------------------------------------------------------
}