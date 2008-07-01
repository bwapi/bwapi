#pragma once

#include "Base.h"

namespace Formula
{
  class Summation : public Base
  {
    public:
      Summation(void);
      virtual ~Summation(void);
      virtual float evaluate();
      virtual void debugEvaluate(std::string& view);
  };
};
