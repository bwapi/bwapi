#pragma once

#include <list>
#include <Util/Types.h>
#include "TaskType.h"

#include "Unit.h"

namespace BWAI
{
  /**
   * Pure virtual class that represents any task that the bwai wants to execute.
   * Every unit assigned to task has its #BWAI#Unit#task pointer set to the
   * appropriate task, but the unit is not 'owner' of the task as more units can
   * sh are the same task and the task doesn't die with the unit.
   * 
   * The owner of tasks is #BWAI#AI and it maintain it's execution and removal.
   *
   * The task anyway is meant to be something independent that should solve all 
   * problems connected with the task on it's own.
   *   
   * It is also important to know, that when unit needs to be removed from the
   * task The task is maintainer of that, it will remove the unit from it's
   * executor list and also frees the unit's pointer to the task.
   */
  class Task
  {
    public :
      /** Creates the without executors (the task itself can always obtain it)  */
      Task(u16 priority = 0);
      /** Creates the task giving it one executor to work with from the start. */
      Task(Unit* executor, u16 priority = 0);
      /** Removes the task and frees all executors from it. */
      virtual ~Task();
      virtual TaskType::Enum getType() = 0;
      /**
       * Executes the task
       * @return @c true if the task was finished and can be removed, @c false
       *         otherwise.
       */
      virtual bool execute() = 0;
      /** Frees the executor from the task (constant time) */
      virtual void freeExecutor(Unit* unit);
      /**
       * Adds executor to the task, note that in the moment you add executor to
       * the task, the task will maintain the unit itself and it shouldn't be
       * manipulated externally. 
       */
      virtual void addExecutor(Unit* unit);

      /** 
       * List of executors of this task.
       * As standard stl doesn't contain hash_map and the microsoft version is
       * bad (sorted associative container instead of Hashed Associative Container)
       * I will just maintain executors of task in simple list, while every
       * #AI#Unit will have it's own iterator pointing to this list, so removal
       * operation will take contant time.
       *
       * If anyone knows how to solve all problems that need to be solved in order
       * to use the stl-port (while having it's sources in the project, not externally)
       * contact me and I would like it to be used.
       */
      std::list<Unit*> executors;
      /** Priority of task - the higher the more important the task is. */
      u16 priority;
     
      struct TaskPriorityLess:
      public std::binary_function<const Task*, const Task*, bool>
      {
        bool operator()(const Task *task1, const Task *task2) const
        {
          return task1->priority > task2->priority;
        }
      };
  };
}