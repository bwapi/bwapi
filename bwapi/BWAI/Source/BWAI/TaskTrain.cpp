#include "TaskTrain.h"
#include <BWAI/Globals.h>
#include <BWAI/Player.h>
#include <BuildOrder/BuildWeights.h>
#include <BuildOrder/Condition.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskTrain::TaskTrain(BuildOrder::BuildWeights* weights, u16 priority)
      : Task(priority)
      , weights(weights)
  {
    BWAI::ai->prioritisedTasks.insert(this);
    for each (Unit* i in BWAI::ai->units)
      if (this->weights->factory == i->getType() &&
          i->getTask() == NULL)
        this->addExecutor(i);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskTrain::~TaskTrain()
  {
    BWAI::ai->prioritisedTasks.erase(this);
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool TaskTrain::execute()
  {
    for each (Unit* i in this->executors)
      if (i->isCompleted() &&
          !i->isLifted() &&
          (!i->isTraining() ||
           (
             i->getTrainingQueue().size() < 2 &&
             i->getBuildUnit() != NULL &&
             i->getBuildUnit()->getRemainingBuildTime() <= BWAPI::Broodwar->getLatency()
           ))
         )
      {
        BuildOrder::BuildWeight* best = NULL;
        for each (BuildOrder::BuildWeight* j in this->weights->weights)
          if (BWAI::ai->player->canBuild(j->unitType) &&
              (j->condition == NULL || j->condition->applies()) &&
              (
                best == NULL ||
                ((float)BWAI::ai->player->getAllUnits(best->unitType)) / ((float)best->weight) >
                ((float)BWAI::ai->player->getAllUnits(j->unitType)) / ((float)j->weight))
             )
          {
            best = j;
          }
        if (best != NULL &&
            BWAI::ai->player->canAfford(best->unitType))
        {
          BWAPI::Broodwar->printPublic("Attempting to train %s from %s.", best->unitType.getName().c_str(), i->getType().getName().c_str());
          i->trainUnit(best->unitType);
          if (i->getType() == BWAPI::UnitTypes::Zerg_Larva ||
              i->getType() == BWAPI::UnitTypes::Zerg_Hydralisk ||
              i->getType() == BWAPI::UnitTypes::Zerg_Mutalisk)
            i->clearTask();
        }
      }
    return false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskTrain::getType()
  {
    return TaskType::Train;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  BWAPI::UnitType TaskTrain::getBuildingType()
  {
    return this->weights->factory;
  }
  //----------------------------------------------------------------------------------------------------------
  BWAI::ReservedResources TaskTrain::getReserved()
  {
    return BWAI::ReservedResources();
  }
  //----------------------------------------------------------------------------------------------------------
}