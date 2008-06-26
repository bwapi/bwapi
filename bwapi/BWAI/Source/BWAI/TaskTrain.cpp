#include "TaskTrain.h"
#include <BW/Unit.h>
#include <BWAI/Globals.h>
#include <BWAPI/Globals.h>
#include <BWAPI/Player.h>
#include <BuildOrder/BuildWeights.h>

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskTrain::TaskTrain(BW::UnitType buildingType, BuildOrder::BuildWeights* weights)
  :Task()
  ,buildingType(buildingType)
  ,weights(weights)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskTrain::~TaskTrain()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskTrain::execute()
  {
    for (std::list<Unit*>::iterator i = this->executors.begin(); i != this->executors.end(); ++i)
      if (
           (*i)->hasEmptyBuildQueueLocal() ||
           (
             (*i)->getBuildQueueLocal()[((*i)->getBuildQueueSlotLocal() + 1) % 5] == BW::UnitID::None &&
             (*i)->getRawDataLocal()->currentBuildUnit != NULL &&
             (*i)->getRawData()->currentBuildUnit->remainingBuildTime <= BWAPI::Broodwar.getLatency()
           )
         )
      { 
        if (!this->weights->weights.empty())
        {
          std::pair<BW::UnitType, int> best = weights->weights.front();
          
          for (std::list<std::pair<BW::UnitType, int> >::iterator j = this->weights->weights.begin()++;
               j != this->weights->weights.end();
               ++j)
            if (BWAI::ai->player->canBuild((*j).first) &&
                ((float)BWAI::ai->player->allUnitTypeCount[best.first.getID()])/((float)best.second) >
                ((float)BWAI::ai->player->allUnitTypeCount[(*j).first.getID()])/((float)(*j).second))
              best = *j;
          if (BWAI::ai->player->canAfford(best.first, BWAPI::ReservedResources()))
            (*i)->trainUnit(best.first);
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
    return this->buildingType;
  }
  //---------------------------------------------------------------------------
  BWAPI::ReservedResources TaskTrain::getReserved()
  {
    return BWAPI::ReservedResources();
  }
  //---------------------------------------------------------------------------
}