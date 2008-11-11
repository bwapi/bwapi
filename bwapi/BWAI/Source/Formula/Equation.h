#include "EqualityType.h"
#include <string>

namespace Formula { class Base; }

namespace Formula
{
  class Equation
  {
    public :
      Equation(Base* left, Base* right, EqualityType::Enum equalityType);
      bool evaluate();
      static Equation* parse(const std::string& input);
    private :
      EqualityType::Enum equalityType;
      Base* left, * right;
  };
}