#include "TaskGather.h"

#include "Mineral.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskGather::TaskGather(Unit* executor, Mineral* mineral)
  :Task(executor)
  ,mineral(mineral)
  {
    this->getMineral()->assignGatherer(this->getExecutor());
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskGather::~TaskGather()
  {
    if (this->getMineral() != NULL)
      this->getMineral()->removeGatherer(this->getExecutor());
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskGather::execute()
  {
    return this->mineral != NULL;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskGather::getType()
  {
    return TaskType::Gather;
  }
  //------------------------------- GET MINERAL -------------------------------
  Mineral* TaskGather::getMineral()
  {
    return this->mineral;
  }
  //---------------------------------------------------------------------------
  void TaskGather::clearMineralPointer()
  {
    this->mineral = NULL;
  }
  //---------------------------------------------------------------------------
}