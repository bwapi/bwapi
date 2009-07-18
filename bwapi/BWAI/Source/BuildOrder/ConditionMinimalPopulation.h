#include "Condition.h"

class TiXmlElement;

#include <Util/Types.h>

namespace BuildOrder
{
  class ConditionMinimalPopulation : public Condition
  {
    public :
      ConditionMinimalPopulation(u16 minimalPopulation);
      ConditionMinimalPopulation(TiXmlElement* element);
      ConditionType::Enum getType();
      bool applies();
      virtual void debugEvaluate(std::string& view);
    private :
      u16 minimalPopulation;
  };
}
