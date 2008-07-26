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
      if ((*i)->getOrderIDLocal() != BW::OrderID::PlayerGuard &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas1 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestGas3 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnGas &&
          (*i)->getOrderIDLocal() != BW::OrderID::MoveToMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestMinerals2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::MiningMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::ResetCollision2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be remmoved from the gas gather because order is (%s) Unit:", 
                                   BW::OrderID::orderName((*i)->getOrderIDLocal()).c_str(), 
                                   (*i)->getName().c_str());
        this->freeExecutor(*i++);
      }
      else
        i++;

    for each (Unit* i in this->executors)
      if (i->getOrderIDLocal() != BW::OrderID::HarvestGas1 &&
          i->getOrderIDLocal() != BW::OrderID::HarvestGas2 &&
          i->getOrderIDLocal() != BW::OrderID::HarvestGas3 &&
          i->getOrderIDLocal() != BW::OrderID::ReturnGas &&
          i->getOrderIDLocal() != BW::OrderID::ResetCollision2)
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