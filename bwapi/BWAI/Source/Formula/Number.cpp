#include "Number.h"
#include <Util/Strings.h>

namespace Formula
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Number::Number(float value)
      : value(value)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Number::Number(const std::string& value)
  {
    sscanf(value.c_str(), "%f", &this->value);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Number::~Number(void)
  {
  }
  //------------------------------------------------ EVALUATE ------------------------------------------------
  float Number::evaluate()
  {
    return this->value;
  }
  //----------------------------------------------------------------------------------------------------------
  void Number::debugEvaluate(std::string& view)
  {
    view += Util::Strings::ssprintf("%f", this->value);
  }
  //----------------------------------------------------------------------------------------------------------
}