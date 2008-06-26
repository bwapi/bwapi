#include "TaskGather.h"

#include <Util/Logger.h>
#include <BWAPI/Globals.h>

#include "Globals.h"
#include "Expansion.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskGather::TaskGather(Unit* mineral, Expansion* expansion)
  :Task()
  ,mineral(mineral)
  ,expansion(expansion)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskGather::~TaskGather()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskGather::execute()
  {
    if (!this->mineral)
      return true;
    std::list<Unit*>::iterator i = this->executors.begin();
    while (i != this->executors.end())
      if ((*i)->getOrderIDLocal() != BW::OrderID::PlayerGuard &&
          (*i)->getOrderIDLocal() != BW::OrderID::MoveToMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::HarvestMinerals2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::MiningMinerals &&
          (*i)->getOrderIDLocal() != BW::OrderID::ResetCollision2 &&
          (*i)->getOrderIDLocal() != BW::OrderID::ReturnMinerals)
      {
        BWAI::ai->log->logDetailed("Unit will be remmoved from the gather because order is (%s) Unit:", BW::OrderID::orderName((*i)->getOrderIDLocal()).c_str(), 
                                                                                                (*i)->getName().c_str());
        this->freeExecutor(*i++);
        ai->expansionsSaturated = false;
      }
      else
        i++;

    for (std::list<Unit*>::iterator i = this->executors.begin(); i != this->executors.end(); ++i)
    {
      //BWAI::ai->log->log("TaskGather::execute::onWorker");
      Unit* miningBuddy = NULL;
      if (i == this->executors.begin() && 
          i != --this->executors.end())
        miningBuddy = *(--this->executors.end());
      else
        if (i != this->executors.begin() && 
            i == --this->executors.end())
        miningBuddy = *this->executors.begin();
      if (
           (
             (*i)->getOrderIDLocal() == BW::OrderID::MoveToMinerals || 
             (*i)->getOrderIDLocal() == BW::OrderID::HarvestMinerals2 || 
             (*i)->getOrderIDLocal() == BW::OrderID::PlayerGuard
           ) &&
           (*i)->getTargetLocal() != this->expansion->gatherCenter &&
             (
               (*i)->getTargetLocal() != this->mineral
               ||
               (
                 (*i)->getDistance(this->mineral) <= BWAPI::Broodwar.getLatency()*3 &&
                 miningBuddy != NULL &&
                 miningBuddy->getOrderTimer() >= BWAPI::Broodwar.getLatency() &&
                 (
                   miningBuddy->getOrderTimer() == BWAPI::Broodwar.getLatency()-1 ||
                   BWAPI::Broodwar.frameCount - (*i)->lastFrameSpam > 4
                 )
               )
             )
           )
        {
          (*i)->lastFrameSpam = BWAPI::Broodwar.frameCount;
          (*i)->orderRightClick(mineral);
        }
    }
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskGather::getType()
  {
    return TaskType::Gather;
  }
  //------------------------------- GET MINERAL -------------------------------
  Unit* TaskGather::getMineral()
  {
    return this->mineral;
  }
  //---------------------------------------------------------------------------
  Expansion* TaskGather::getExpansion()
  {
    return expansion;
  }
  //---------------------------------------------------------------------------
}