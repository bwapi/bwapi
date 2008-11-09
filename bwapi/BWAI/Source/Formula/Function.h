#pragma once

#include <BW/UnitType.h>
#include "Base.h"
#include "FunctionType.h"

namespace Formula
{
  class Function : public Base
  {
    public:
      Function(const std::string& input, size_t& position);
      virtual ~Function(void);
      float evaluate();
      virtual void debugEvaluate(std::string& view);
    private :
      bool needParameter;
      std::string functionParameter;
      FunctionType::Enum type;
      // I'm too lazy to make another 5 classes for every function with different data, so I put it all here
      BW::UnitType unitType;
      float configValue;
      BW::Race::Enum race;
  };
};