#include "Task.h"

namespace BWAI
{
  //------------------------------- CONSTRUCTOR -------------------------------
  Task::Task(Unit* executor)
  :executor(executor)
  {
  }
  //-------------------------------- DESTRUCTOR -------------------------------
  Task::~Task()
  {
  }
  //------------------------------- GET EXECUTOR ------------------------------
  Unit* Task::getExecutor()
  {
    return this->executor;
  }
  //---------------------------------------------------------------------------
}