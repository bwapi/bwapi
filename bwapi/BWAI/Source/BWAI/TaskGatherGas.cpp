#include "TaskGatherGas.h"

#include <Util/Logger.h>
#include <BWAPI/Globals.h>

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
      if ((*i)->getOrderID() != BW::OrderID::PlayerGuard &&
          (*i)->getOrderID() != BW::OrderID::HarvestGas1 &&
          (*i)->getOrderID() != BW::OrderID::HarvestGas2 &&
          (*i)->getOrderID() != BW::OrderID::HarvestGas3 &&
          (*i)->getOrderID() != BW::OrderID::ReturnGas &&
          (*i)->getOrderID() != BW::OrderID::MoveToMinerals &&
          (*i)->getOrderID() != BW::OrderID::HarvestMinerals2 &&
          (*i)->getOrderID() != BW::OrderID::MiningMinerals &&
          (*i)->getOrderID() != BW::OrderID::ResetCollision2 &&
          (*i)->getOrderID() != BW::OrderID::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be removed from the gas gather because order is (%s) Unit:", 
                                   BW::OrderID::orderName((*i)->getOrderID()).c_str(), 
                                   (*i)->getName().c_str());
        this->freeExecutor(*i++);
      }
      else
        i++;

    for each (Unit* i in this->executors)
      if (i->getOrderID() != BW::OrderID::HarvestGas1 &&
          i->getOrderID() != BW::OrderID::HarvestGas2 &&
          i->getOrderID() != BW::OrderID::HarvestGas3 &&
          i->getOrderID() != BW::OrderID::ReturnGas &&
          i->getOrderID() != BW::OrderID::ResetCollision2)
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