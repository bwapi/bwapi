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
      if ((*i)->getOrderID() != BWAPI::Orders::PlayerGuard &&
          (*i)->getOrderID() != BWAPI::Orders::HarvestGas1 &&
          (*i)->getOrderID() != BWAPI::Orders::HarvestGas2 &&
          (*i)->getOrderID() != BWAPI::Orders::HarvestGas3 &&
          (*i)->getOrderID() != BWAPI::Orders::ReturnGas &&
          (*i)->getOrderID() != BWAPI::Orders::MoveToMinerals &&
          (*i)->getOrderID() != BWAPI::Orders::HarvestMinerals2 &&
          (*i)->getOrderID() != BWAPI::Orders::MiningMinerals &&
          (*i)->getOrderID() != BWAPI::Orders::ResetCollision2 &&
          (*i)->getOrderID() != BWAPI::Orders::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be removed from the gas gather because order is (%s) Unit:", 
                                   (*i)->getOrderID().getName().c_str(), 
                                   (*i)->getName().c_str());
        this->freeExecutor(*i++);
      }
      else
        i++;

    for each (Unit* i in this->executors)
      if (i->getOrderID() != BWAPI::Orders::HarvestGas1 &&
          i->getOrderID() != BWAPI::Orders::HarvestGas2 &&
          i->getOrderID() != BWAPI::Orders::HarvestGas3 &&
          i->getOrderID() != BWAPI::Orders::ReturnGas &&
          i->getOrderID() != BWAPI::Orders::ResetCollision2)
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