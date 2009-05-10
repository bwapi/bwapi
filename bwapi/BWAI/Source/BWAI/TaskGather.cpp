#include "TaskGather.h"

#include <Util/Logger.h>
#include <BWAPI/Globals.h>

#include "Globals.h"
#include "Expansion.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskGather::TaskGather(Unit* mineral, Expansion* expansion)
  :Task()
  ,mineral(mineral)
  ,expansion(expansion)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskGather::~TaskGather()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool TaskGather::execute()
  {
    if (!this->mineral)
      return true;
    std::list<Unit*>::iterator i = this->executors.begin();
    while (i != this->executors.end())
      if ((*i)->getOrderID() != BW::OrderID::PlayerGuard &&
          (*i)->getOrderID() != BW::OrderID::MoveToMinerals &&
          (*i)->getOrderID() != BW::OrderID::HarvestMinerals2 &&
          (*i)->getOrderID() != BW::OrderID::MiningMinerals &&
          (*i)->getOrderID() != BW::OrderID::ResetCollision2 &&
          (*i)->getOrderID() != BW::OrderID::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be remmoved from the gather because order is (%s) Unit:", BW::OrderID::orderName((*i)->getOrderID()).c_str(), 
                                                                                                (*i)->getName().c_str());
        this->freeExecutor(*i++);
        ai->expansionsSaturated = false;
      }
      else
        i++;

    for each (Unit* i in this->executors)
    {
      //BWAI::ai->log->log("TaskGather::execute::onWorker");
      Unit* miningBuddy = NULL;
      if (i == this->executors.front() && 
          i != this->executors.back())
        miningBuddy = *(--this->executors.end());
      else
        if (i != this->executors.front() && 
            i == this->executors.back())
        miningBuddy = *this->executors.begin();
      if (
           (
             i->getOrderID() == BW::OrderID::MoveToMinerals || 
             i->getOrderID() == BW::OrderID::HarvestMinerals2 || 
             i->getOrderID() == BW::OrderID::PlayerGuard
           ) &&
           i->getTarget() != this->expansion->gatherCenter &&
             (
               i->getTarget() != this->mineral
               ||
               (
                 i->getDistance(this->mineral) <= BWAPI::Broodwar.getLatency()*3 &&
                 miningBuddy != NULL &&
                 miningBuddy->getOrderTimer() >= BWAPI::Broodwar.getLatency() &&
                 (
                   miningBuddy->getOrderTimer() == BWAPI::Broodwar.getLatency()-1 ||
                   BWAPI::Broodwar.getFrameCount() - i->lastFrameSpam > 4
                 )
               )
             )
           )
        {
          i->lastFrameSpam = BWAPI::Broodwar.getFrameCount();
          i->orderRightClick(mineral);
        }
    }
    return false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskGather::getType()
  {
    return TaskType::Gather;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  Unit* TaskGather::getMineral()
  {
    return this->mineral;
  }
  //----------------------------------------------------------------------------------------------------------
  Expansion* TaskGather::getExpansion()
  {
    return expansion;
  }
  //----------------------------------------------------------------------------------------------------------
  void TaskGather::freeExecutor(Unit* unit)
  {
    unit->clearTask();
    this->executors.erase(unit->taskListIterator);
    BWAI::ai->mineralGatherers--;
  }
  //----------------------------------------------------------------------------------------------------------
  void TaskGather::addExecutor(Unit* unit)
  {
    this->executors.push_back(unit);
    unit->taskListIterator = --this->executors.end();
    unit->setTask(this);
    BWAI::ai->mineralGatherers++;
  }
  //----------------------------------------------------------------------------------------------------------
}