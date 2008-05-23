#include "Task.h"

#include "Unit.h"

namespace BWAI
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Task::Task()
  {
  }
  //------------------------------- CONSTRUCTOR -------------------------------
  Task::Task(Unit* executor)
  {
    if (executor != NULL)
      this->addExecutor(executor);
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  Task::~Task()
  {
    for (std::list<Unit*>::iterator i = this->executors.begin(); i != this->executors.end(); ++i)
    {
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