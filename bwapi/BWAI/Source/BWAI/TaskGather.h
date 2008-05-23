#include "Task.h"

namespace BWAI { class Mineral; }
namespace BWAI { class Expansion; }

namespace BWAI
{
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