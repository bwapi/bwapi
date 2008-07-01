#pragma once

#include "ConditionType.h"
#include "ConditionRunType.h"

class TiXmlElement;

namespace BuildOrder
{
  /** Specifies condition of build order command. */
  class Condition
  {
    public :
      virtual ConditionType::Enum getType() = 0; /**< Get the Condition descenant identification. */
      virtual bool applies() = 0; /**< Returns if the condition is fullfiled (Game specific) */
      static Condition* load(TiXmlElement* xmlElement);
  };
}