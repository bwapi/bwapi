#include "Multiplication.h"
#include <BWAI/Globals.h>
#include <Util/Logger.h>
#include <Util/Strings.h>

namespace Formula
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Multiplication::Multiplication(void)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Multiplication::~Multiplication(void)
  {
  }
  //------------------------------------------------ EVALUATE ------------------------------------------------
  float Multiplication::evaluate()
  {
    float result = 1;
    for each (Base* i in this->nested)
      result *= i->evaluate();
    return result;
  }
  //----------------------------------------------------------------------------------------------------------
  void Multiplication::debugEvaluate(std::string& view)
  {
    for each (Base* i in this->nested)
    {
      if (i != this->nested.front())
        view += "*";
      i->debugEvaluate(view);
    }
  }
  //----------------------------------------------------------------------------------------------------------
}