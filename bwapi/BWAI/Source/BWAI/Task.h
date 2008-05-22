#pragma once

#include "TaskType.h"

namespace BWAI { class Unit; }

namespace BWAI
{
  class Task
  {
    public :
      Task(Unit* executor);
      virtual ~Task();
      virtual TaskType::Enum getType() = 0;
      virtual bool execute() = 0;
      Unit* getExecutor();
    private :
      Unit* executor;
  };
}