#include "BuildWeight.h"

#include <tinyxml.h>
#include <BWAPI.h>
#include <Util/Xml.h>
#include "Condition.h"

namespace BuildOrder
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BuildWeight::BuildWeight(TiXmlElement* element)
  {
    this->unitType = BWAPI::Broodwar->getUnitType(Util::Xml::getRequiredAttribute(element, "name"));
    this->weight = Util::Xml::getRequiredU16Attribute(element, "weight");
    TiXmlElement* conditionElement = element->FirstChildElement("condition");
    if (conditionElement != NULL)
      this->condition = Condition::load(conditionElement);
    else 
      this->condition = NULL;
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  BuildWeight::~BuildWeight()
  {
    delete condition;
  }
  //----------------------------------------------------------------------------------------------------------
}