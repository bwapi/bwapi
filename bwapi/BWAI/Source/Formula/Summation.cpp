#include "Summation.h"
#include <Util/Strings.h>

namespace Formula
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Summation::Summation(void)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Summation::~Summation(void)
  {
  }
  //------------------------------------------------ EVALUATE ------------------------------------------------
  float Summation::evaluate()
  {
    float result = 0;
    for each (Base* i in this->nested)
      result += i->evaluate();
    return result;
  }
  //----------------------------------------------------------------------------------------------------------
  void Summation::debugEvaluate(std::string& view)
  {
    view += "(";
    for each (Base* i in this->nested)
    {
      if (i != this->nested.front())
        view += " + ";
      i->debugEvaluate(view);
    }
    view += Util::Strings::ssprintf("=%f)", this->evaluate());
  }
  //----------------------------------------------------------------------------------------------------------
}