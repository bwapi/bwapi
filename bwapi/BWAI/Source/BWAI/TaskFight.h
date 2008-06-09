#pragma once
#include "Task.h"

namespace BWAI
{
  /**
   * Represents task to fight, will be differenciated probably into more classes.
   */
  class TaskFight : public Task
  {
    public :
      TaskFight();
      virtual ~TaskFight();
      bool execute();
      TaskType::Enum getType();
  };
}