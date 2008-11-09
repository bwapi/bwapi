#pragma once

class TiXmlElement;
namespace Formula { class Base; }

#include <Util/Types.h>
#include "Condition.h"

namespace BuildOrder
{
  class ConditionFormula : public Condition
  {
    public :
      ConditionFormula(TiXmlElement* element);
      bool applies();
      ConditionType::Enum getType();
      virtual void debugEvaluate(std::string& view);
    private :
      Formula::Base* formula; 
  };
}
