#pragma once

class TiXmlElement;
namespace Formula { class Equation; }

#include <Util/Types.h>
#include "Condition.h"

namespace BuildOrder
{
  class ConditionEquation : public Condition
  {
    public :
      ConditionEquation(TiXmlElement* element);
      bool applies();
      ConditionType::Enum getType();
      virtual void debugEvaluate(std::string& view);
    private :
      Formula::Equation* equation;
  };
}
