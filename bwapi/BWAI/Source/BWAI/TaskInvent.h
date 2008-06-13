#pragma once
#include "Task.h"

#include <BW/TechType.h>

namespace BWAI
{
  /** Represents task to invent tech given by the ai. */
  class TaskInvent : public Task
  {
    public :
      TaskInvent(BW::TechType techType);
      virtual ~TaskInvent();
      bool execute();
      TaskType::Enum getType();
      BW::TechType getTechType();
    private :  
      BW::TechType techType;
  };
}