#pragma once

#include <list>
#include <Util/Types.h>
#include "Condition.h"

class TiXmlElement;
namespace Formula { class Base; }

namespace BuildOrder
{
  class ConditionAnd : public Condition
  {
    public :
      ConditionAnd(TiXmlElement* xmlElement);
      ~ConditionAnd();
      bool applies();
      ConditionType::Enum getType();
      virtual void debugEvaluate(std::string& view);
    private :
      std::list<Condition*> nested; 
  };
}
