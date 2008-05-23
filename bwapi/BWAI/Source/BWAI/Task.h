#pragma once

#include <list>

#include "TaskType.h"

#include "Unit.h"

namespace BWAI
{
  class Task
  {
    public :
      Task();
      Task(Unit* executor);
      virtual ~Task();
      
      virtual TaskType::Enum getType() = 0;
      virtual bool execute() = 0;
      void freeExecutor(Unit* unit);
      void addExecutor(Unit* unit);
      
      /** 
       * List of executors of this task.
       * As standard stl doesn't contain hash_map and the microsoft version is
       * bad (sorted associative container instead of Hashed Associative Container)
       * I will just maintain executors of task in simple list, while every
       * #AI#Unit will have it's own iterator pointing to this list, so removal
       * operation will take contant time.
       */
      std::list<Unit*> executors;
  };
}