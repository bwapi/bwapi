#include "Task.h"

namespace BWAI { class Mineral; }

namespace BWAI
{
  class TaskGather : public Task
  {
    public :
      TaskGather(Unit* executor, Mineral* mineral);
      virtual ~TaskGather();
      bool execute();
      Mineral* getMineral();
      TaskType::Enum getType();
      
      void clearMineralPointer();
    private :  
      Mineral* mineral;
  };
}