#include "Task.h"

namespace BWAI { class Expansion; }

namespace BWAI
{
  /** 
   * Task to gather gas, note that one instance of this class 
   * per refinery/assimilator/extractor is expected.
   */
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