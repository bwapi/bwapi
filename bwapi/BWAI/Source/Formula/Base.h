#pragma once

#include <string>
#include <list>

namespace Formula
{
  class Base
  {
    public:
      Base();
      virtual ~Base();
      virtual float evaluate() = 0;
      virtual void debugEvaluate(std::string& view) = 0;
      static Base* parse(const std::string& input);
      std::list<Base*> nested;
    private :
      static Base* parseInternal(const std::string& input, size_t& position);
  };
}