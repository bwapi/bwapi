#pragma once

#include <string>
#include <BW/UnitType.h>

class TiXmlElement;

namespace BuildOrder
{
  
  namespace UnitMaxType
  {
    enum Enum
    {
      AutoEffectiveGather,
      None
    };
  }
  
  class BuildWeight
  {
    public : 
      BuildWeight(TiXmlElement *element);
      BW::UnitType unitType;
      u16 weight;
      UnitMaxType::Enum maxType;
  };
}