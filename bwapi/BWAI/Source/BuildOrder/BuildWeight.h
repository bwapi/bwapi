#pragma once

#include <string>
#include <BWAPI.h>

class TiXmlElement;
namespace BuildOrder { class Condition; }

namespace BuildOrder
{
  class BuildWeight
  {
    public : 
      BuildWeight(TiXmlElement *element);
      ~BuildWeight();
      BWAPI::UnitType unitType;
      u16 weight;
      Condition* condition;
  };
}