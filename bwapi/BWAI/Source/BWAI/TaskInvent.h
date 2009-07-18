#pragma once
#include "Task.h"

#include <BWAPI.h>

namespace BWAI
{
  /** Represents task to invent tech given by the ai. */
  class TaskInvent : public Task
  {
    public :
      TaskInvent(BWAPI::TechType techType, u16 priority);
      virtual ~TaskInvent();
      bool execute();
      TaskType::Enum getType();
      BWAPI::TechType getTechType();
    private :
      BWAPI::TechType techType;
  };
}