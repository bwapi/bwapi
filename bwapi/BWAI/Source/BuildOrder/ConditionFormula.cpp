#include "ConditionFormula.h"

#include <tinyxml.h>
#include <Util/Strings.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include <Util/Logger.h>

#include <BWAI/Globals.h>

#include <BWAPI/Player.h>

#include <Formula/Base.h>

namespace BuildOrder
{
  //---------------------------------------- CONSTRUCTOR -----------------------------------------------------
  ConditionFormula::ConditionFormula(TiXmlElement* element)
  {
    this->formula = Formula::Base::parse(Util::Xml::getRequiredAttribute(element, "formula"));
  }
  //------------------------------------------ GET TYPE ------------------------------------------------------
  ConditionType::Enum ConditionFormula::getType()
  {
    return ConditionType::Formula;
  }
  //------------------------------------------ APPLIES -------------------------------------------------------
  bool ConditionFormula::applies()
  {
    std::string view;
    this->formula->debugEvaluate(view);
    BWAI::ai->log->log("Formula '%s' = %f", view.c_str(), this->formula->evaluate());
    return this->formula->evaluate() > 0;
  }
}