#pragma once

namespace BuildOrder { class BuildWeights; }

#include <BWAPI/ReservedResources.h>

#include "Task.h"

namespace BWAI
{
  /** Represents task to train units in certain type of factory building */
  class TaskTrain : public Task
  {
    public :
      TaskTrain(BW::UnitType buildingType, BuildOrder::BuildWeights* weights);
      virtual ~TaskTrain();
      bool execute();
      BW::UnitType getBuildingType();
      TaskType::Enum getType();
      BWAPI::ReservedResources getReserved();
    private :  
      BW::UnitType buildingType;
      BuildOrder::BuildWeights* weights;
  };
}