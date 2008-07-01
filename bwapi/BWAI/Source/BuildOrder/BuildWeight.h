#pragma once

#include <string>
#include <BW/UnitType.h>

class TiXmlElement;
namespace BuildOrder { class Condition; }

namespace BuildOrder
{
  class BuildWeight
  {
    public : 
      BuildWeight(TiXmlElement *element);
      ~BuildWeight();
      BW::UnitType unitType;
      u16 weight;
      Condition* condition;
  };
}