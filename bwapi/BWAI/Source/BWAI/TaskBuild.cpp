#include "TaskBuild.h"

#include <list>

#include "BuildingToMake.h"
#include "AI.h"
#include "Globals.h"

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskBuild::TaskBuild(Unit* executor, BuildingToMake* buildingToMake)
  :Task(executor)
  ,buildingToMake(buildingToMake)
  {
    //BWAI::ai->plannedBuildings.push_back(buildingToMake);
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskBuild::~TaskBuild()
  {
    /*for (std::list<BuildingToMake*>::iterator i = BWAI::ai->plannedBuildings.begin();
         i != BWAI::ai->plannedBuildings.end();
         ++i)
      if ((*i) == buildingToMake)
      {
        BWAI::ai->plannedBuildings.erase(i);
        break;
      }
    delete buildingToMake; */
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskBuild::execute()
  {
    return buildingToMake == NULL; 
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskBuild::getType()
  {
    return TaskType::Build;
  }
  //------------------------------- GET MINERAL -------------------------------
  BuildingToMake* TaskBuild::getBuildingToMake()
  {
    return this->buildingToMake;
  }
  //--------------------------- CLEAR BUILDING TO MAKE POINTER ----------------
  void TaskBuild::clearBuildingToMakePointer()
  {
    this->buildingToMake = NULL;
  }
  //---------------------------------------------------------------------------
}