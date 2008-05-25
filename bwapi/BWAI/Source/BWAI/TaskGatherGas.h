#include "Task.h"

namespace BWAI { class Expansion; }

namespace BWAI
{
  class TaskGatherGas : public Task
  {
    public :
      TaskGatherGas(Unit* refinery, Expansion* expansion);
      virtual ~TaskGatherGas();
      bool execute();
      TaskType::Enum getType();
      
      Unit* getRefinery();
      Expansion* getExpansion();
    private :  
      BWAI::Unit* refinery;
      Expansion* expansion;
  };
}