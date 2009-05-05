#include "Unit.h"

#include <Util/Logger.h>
#include <Util/Types.h>
#include <BW/Offsets.h>
#include <BW/Unit.h>
#include <BWAPI/Globals.h>

#include "Task.h"
#include "Globals.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :BWAPI::Unit(*unit) /**< default autoconstructor usage */
  ,task(0)
  ,lastFrameSpam(0)
  ,expansion(0)
  {
  }
  //------------------------------------------ BW Unit TO BWAI Unit ------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return ai->getUnit(((int)unit - (int)BW::BWDATA_UnitNodeTable)/336);
  }
  //------------------------------------------ BW Unit TO BWAI Unit ------------------------------------------
  Unit* Unit::BWAPIUnitToBWAIUnit(BWAPI::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return BWAI::ai->getUnit(unit->getIndex());
  }
  #pragma warning (pop)
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* Unit::getTarget()
  {
    return BWAI::Unit::BWAPIUnitToBWAIUnit(this->BWAPI::Unit::getTarget());
  }
  //-------------------------------------------- GET TARGET LOCAL --------------------------------------------
  Unit* Unit::getTargetLocal()
  {
    return BWAI::Unit::BWAPIUnitToBWAIUnit(this->BWAPI::Unit::getTargetLocal());
  }
  //----------------------------------------------- GET TARGET -----------------------------------------------
  Unit* Unit::getOrderTarget()
  {
    return BWAI::Unit::BWAPIUnitToBWAIUnit(this->BWAPI::Unit::getOrderTarget());
  }
  //-------------------------------------------- GET TARGET LOCAL --------------------------------------------
  Unit* Unit::getOrderTargetLocal()
  {
    return BWAI::Unit::BWAPIUnitToBWAIUnit(this->BWAPI::Unit::getOrderTargetLocal());
  }
  //------------------------------------------------ GET NEXT ------------------------------------------------
  Unit* Unit::getNext()
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
    return Unit::BWUnitToBWAIUnit(this->getOriginalRawData()->nextUnit);
  }
  //------------------------------------------------ GET TASK ------------------------------------------------
  Task* Unit::getTask()
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