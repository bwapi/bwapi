#include "TaskGatherGas.h"

#include <Util/Logger.h>
#include <BWAI/Globals.h>

#include "Globals.h"
#include "Expansion.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskGatherGas::TaskGatherGas(Unit* refinery, Expansion* expansion)
  :Task()
  ,refinery(refinery)
  ,expansion(expansion)
  {
    this->refinery->expansion = expansion;
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskGatherGas::~TaskGatherGas()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool TaskGatherGas::execute()
  {
    std::list<Unit*>::iterator i = this->executors.begin();
    while (i != this->executors.end())
      if ((*i)->getOrder() != BWAPI::Orders::PlayerGuard &&
          (*i)->getOrder() != BWAPI::Orders::MoveToGas &&
          (*i)->getOrder() != BWAPI::Orders::WaitForGas &&
          (*i)->getOrder() != BWAPI::Orders::HarvestGas &&
          (*i)->getOrder() != BWAPI::Orders::ReturnGas &&
          (*i)->getOrder() != BWAPI::Orders::MoveToMinerals &&
          (*i)->getOrder() != BWAPI::Orders::WaitForMinerals &&
          (*i)->getOrder() != BWAPI::Orders::MiningMinerals &&
          (*i)->getOrder() != BWAPI::Orders::ResetCollision2 &&
          (*i)->getOrder() != BWAPI::Orders::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be removed from the gas gather because order is (%s) Unit:", 
                                   (*i)->getOrder().getName().c_str(), 
                                   (*i)->getName().c_str());
        this->freeExecutor(*i++);
      }
      else
        i++;

    for each (Unit* i in this->executors)
      if (i->getOrder() != BWAPI::Orders::MoveToGas &&
          i->getOrder() != BWAPI::Orders::WaitForGas &&
          i->getOrder() != BWAPI::Orders::HarvestGas &&
          i->getOrder() != BWAPI::Orders::ReturnGas &&
          i->getOrder() != BWAPI::Orders::ResetCollision2)
        i->orderRightClick(refinery);
    return false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskGatherGas::getType()
  {
    return TaskType::Gather;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  Unit* TaskGatherGas::getRefinery()
  {
    return this->refinery;
  }
  //----------------------------------------------------------------------------------------------------------
  Expansion* TaskGatherGas::getExpansion()
  {
    return expansion;
  }
  //----------------------------------------------------------------------------------------------------------
}