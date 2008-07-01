#pragma once

class TiXmlElement;
namespace BuildOrder { class Condition; }
#include <Util/Types.h>
#include "ConditionRunType.h"

namespace BuildOrder
{
  /** Represens one command in the build order. */
  class Command
  {
    public :
      Command(TiXmlElement* element);
      /** 
       * Tries to execute the current command.
       * The code is specific for every type of the command.
       * @return @c true if the task was executed @false otherwise
       */
      virtual bool execute() = 0;
      /** Specifies condition of the command, @NULL means no condition. */
      Condition* condition;  
      /** Specifies the way the condition affect the run of the build order. */
      BuildOrder::ConditionRunType::Enum conditionRunType;
      bool conditionApplies();
      u16 priority;
  };
}