#include "Task.h"

namespace BWAI { class Expansion; }

namespace BWAI
{
  /** 
   * Task to gather minerals, note that one instance per mineral is excepted.
   * This task doesn't obtain it's executors itself, as it is maintained externally.
   */
  class TaskGather : public Task
  {
    public :
      TaskGather(Unit* mineral, Expansion* expansion);
      virtual ~TaskGather();
      bool execute();
      TaskType::Enum getType();
      
      Unit* getMineral();
      Expansion* getExpansion();
    private :  
      BWAI::Unit* mineral;
      Expansion* expansion;
  };
}