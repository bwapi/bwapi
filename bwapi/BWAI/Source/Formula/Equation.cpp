#include "Equation.h"
#include <Formula/Base.h>
#include <Util/Exceptions.h>

namespace Formula
{
  //------------------------------------------------ CONSTRUCTOR ---------------------------------------------
  Equation::Equation(Base* left, Base* right, EqualityType::Enum equalityType)
  :left(left)
  ,right(right)
  ,equalityType(equalityType)
  {
  }
  //------------------------------------------------- EVALUATE -----------------------------------------------
  bool Equation::evaluate()
  {
    switch (this->equalityType)
    {
      case EqualityType::EqualityRelation             : return this->left->evaluate() == this->right->evaluate();
      case EqualityType::InequalityRelation           : return this->left->evaluate() != this->right->evaluate();
      case EqualityType::LeftIsBiggerRelation         : return this->left->evaluate() >  this->right->evaluate();
      case EqualityType::LeftIsBiggerOrEqualRelation  : return this->left->evaluate() >= this->right->evaluate();
      case EqualityType::RightIsBiggerRelation        : return this->left->evaluate() <  this->right->evaluate();
      case EqualityType::RightIsBiggerOrEqualRelation : return this->left->evaluate() <= this->right->evaluate();
      default : throw GeneralException("This should never happen (unknown Equality type)");
    }
  }
  //-------------------------------------------------- PARSE -------------------------------------------------
  Equation* Equation::parse(const std::string& input)
  {
    std::string::size_type equationMark = input.find_first_of("<>=!");
    if (equationMark == std::string::npos)
      throw ParseException("Error in eqution input '" + input + "' - equation sides and relation not recognized");
    if (input.substr(equationMark, 2) == "<=")
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 2, input.size() - equationMark - 2)),
                          EqualityType::RightIsBiggerOrEqualRelation);
    if (input.substr(equationMark, 2) == ">=")
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 2, input.size() - equationMark - 2)),
                          EqualityType::LeftIsBiggerOrEqualRelation);
    if (input.substr(equationMark, 2) == "!=")
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 2, input.size() - equationMark - 2)),
                          EqualityType::InequalityRelation);
    if (input[equationMark] == '>')
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 1, input.size() - equationMark - 1)),
                          EqualityType::LeftIsBiggerRelation);
    if (input[equationMark] == '<')
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 1, input.size() - equationMark - 1)),
                          EqualityType::RightIsBiggerRelation);
    if (input[equationMark] == '=')
      return new Equation(Base::parse(input.substr(0, equationMark)),
                          Base::parse(input.substr(equationMark + 1, input.size() - equationMark - 1)),
                          EqualityType::EqualityRelation);
    throw ParseException("Error in eqution input '" + input + "' - equation sides and relation not recognized");                          
  }
  //----------------------------------------------------------------------------------------------------------
}