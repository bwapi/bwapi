#include "ConditionEquation.h"
#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include <Util/Logger.h>
#include <BWAI/Globals.h>
#include <BWAPI/Player.h>
#include <Formula/Equation.h>

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ConditionEquation::ConditionEquation(TiXmlElement* element)
  {
    this->equation = Formula::Equation::parse(Util::Xml::getRequiredAttribute(element, "equation"));
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  ConditionType::Enum ConditionEquation::getType()
  {
    return ConditionType::Equation;
  }
  //------------------------------------------------ APPLIES -------------------------------------------------
  bool ConditionEquation::applies()
  {
    return this->equation->evaluate();
  }
  //-------------------------------------------- DEBUG EVALUATE ----------------------------------------------
  void ConditionEquation::debugEvaluate(std::string& view)
  {
    //this->formula->debugEvaluate(view);
  }
  //----------------------------------------------------------------------------------------------------------
}