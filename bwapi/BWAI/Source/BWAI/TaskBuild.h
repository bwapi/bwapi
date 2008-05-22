#include "Task.h"

namespace BWAI { class BuildingToMake; }

namespace BWAI
{
  class TaskBuild : public Task
  {
    public :
      TaskBuild(Unit* executor, BuildingToMake* buildingToMake);
      virtual ~TaskBuild();
      bool execute();
      BuildingToMake* getBuildingToMake();
      TaskType::Enum getType();
      
      void clearBuildingToMakePointer();
    private :  
      BuildingToMake* buildingToMake;
  };
}