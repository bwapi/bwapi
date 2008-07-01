#include "TaskTrain.h"
#include <BW/Unit.h>
#include <BWAI/Globals.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Player.h>
#include <BuildOrder/BuildWeights.h>

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskTrain::TaskTrain(BuildOrder::BuildWeights* weights, u16 priority)
  :Task(priority)
  ,weights(weights)
  {
    BWAI::ai->prioritisedTasks.insert(this);
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskTrain::~TaskTrain()
  {
    BWAI::ai->prioritisedTasks.erase(this);
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskTrain::execute()
  {
    for each (Unit* i in this->executors)
      if (
           i->hasEmptyBuildQueueLocal() ||
           (
             i->getBuildQueueLocal()[(i->getBuildQueueSlotLocal() + 1) % 5] == BW::UnitID::None &&
             i->getRawDataLocal()->currentBuildUnit != NULL &&
             i->getRawData()->currentBuildUnit->remainingBuildTime <= BWAPI::Broodwar.getLatency()
           )
         )
      { 
        if (!this->weights->weights.empty())
        {
          BuildOrder::BuildWeight* best = weights->weights.front();
          
          for each (BuildOrder::BuildWeight* j in this->weights->weights)
            if (BWAI::ai->player->canBuild(j->unitType) &&
                ((float)BWAI::ai->player->allUnitTypeCount[best->unitType.getID()])/((float)best->weight) >
                ((float)BWAI::ai->player->allUnitTypeCount[j->unitType.getID()])/((float)j->weight))
              best = j;
          if (BWAI::ai->player->canAfford(best->unitType, BWAPI::ReservedResources()))
            i->trainUnit(best->unitType);
        }
      }  
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskTrain::getType()
  {
    return TaskType::Train;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::UnitType TaskTrain::getBuildingType()
  {
    return this->weights->factory;
  }
  //---------------------------------------------------------------------------
  BWAPI::ReservedResources TaskTrain::getReserved()
  {
    return BWAPI::ReservedResources();
  }
  //---------------------------------------------------------------------------
}