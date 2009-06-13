#pragma once

namespace BuildOrder { class BuildWeights; }

#include <BWAPI.h>
#include <BWAI/ReservedResources.h>

#include "Task.h"

namespace BWAI
{
  /** Represents task to train units in certain type of factory building */
  class TaskTrain : public Task
  {
    public :
      TaskTrain(BuildOrder::BuildWeights* weights, u16 priority);
      virtual ~TaskTrain();
      bool execute();
      BWAPI::UnitType getBuildingType();
      TaskType::Enum getType();
      BWAI::ReservedResources getReserved();
      BuildOrder::BuildWeights* weights;
  };
}