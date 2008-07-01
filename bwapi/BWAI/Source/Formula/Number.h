#pragma once

#include "Base.h"

namespace Formula
{
  class Number : public Base
  {
    public:
      Number(float value);
      Number(const std::string& value);
      virtual ~Number(void);
      float evaluate();
      virtual void debugEvaluate(std::string& view);
    private :
      float value;
  };
};
