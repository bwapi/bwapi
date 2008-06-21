#include "Task.h"

#include <Util/Logger.h>

#include "Globals.h"
#include "Unit.h"

namespace BWAI
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Task::Task(u8 priority)
  :priority(priority)
  {
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  Task::Task(Unit* executor, u8 priority)
  :priority(priority)
  {
    if (executor != NULL)
      this->addExecutor(executor);
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  Task::~Task()
  {
    for (std::list<Unit*>::iterator i = this->executors.begin(); i != this->executors.end(); ++i)
    {
      BWAI::ai->log->log("Freeing [%d] from the task", (*i)->getIndex());
      (*i)->clearTask();
      (*i)->expansion = NULL;
    }
  }
  //------------------------------- FREE EXECUTOR -----------------------------
  void Task::freeExecutor(Unit* unit)
  {
    unit->clearTask();
    this->executors.erase(unit->taskListIterator);
  }
  //-------------------------------- ADD EXECUTOR -----------------------------
  void Task::addExecutor(Unit* unit)
  {
    this->executors.push_back(unit);
    unit->taskListIterator = --this->executors.end();
    unit->setTask(this);
  }
  //---------------------------------------------------------------------------
}