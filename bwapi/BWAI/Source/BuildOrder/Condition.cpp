#include "Condition.h"

#include <Util/Xml.h>
#include <Util/Exceptions.h>
#include "ConditionMinimalPopulation.h"
#include "ConditionFormula.h"
#include "ConditionAnd.h"

namespace BuildOrder
{
  //-------------------------------------------------- LOAD --------------------------------------------------
  Condition* Condition::load(TiXmlElement* xmlElement)
  {
    std::string conditionType = Util::Xml::getRequiredAttribute(xmlElement, "type");
    if (conditionType == "minimal-population")
      return new ConditionMinimalPopulation(xmlElement);
    if (conditionType == "formula")
      return new ConditionFormula(xmlElement);
    if (conditionType == "and")
      return new ConditionAnd(xmlElement);      
    throw new ParseException("Unknown condition type " + conditionType);
  }
  //----------------------------------------------------------------------------------------------------------
}