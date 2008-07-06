#pragma once

#include "Base.h"

namespace Formula
{
  class Multiplication : public Base
  {
    public:
      Multiplication(void);
      virtual ~Multiplication(void);
      float evaluate();
      virtual void debugEvaluate(std::string& view);
  };
};