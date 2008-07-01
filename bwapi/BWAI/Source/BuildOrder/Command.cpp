#include "Command.h"

#include <tinyxml.h>
#include <Util/Xml.h>
#include <Util/Types.h>

#include "ConditionMinimalPopulation.h"

namespace BuildOrder
{
   //--------------------------------------------- CONSTRUCTOR -----------------------------------------------
   Command::Command(TiXmlElement *element)
   :condition(NULL)
   {
     TiXmlElement* conditionElement = element->FirstChildElement("condition");
     if (conditionElement != NULL)
     {
       std::string conditionType = Util::Xml::getRequiredAttribute(conditionElement, "type");
       if (conditionType == "minimal-population")
         this->condition = new ConditionMinimalPopulation(conditionElement);
     }
     this->priority = Util::Xml::getOptionalU16Attribute(element, "priority", 0);
   }
   //------------------------------------------- CONDITION APPLIES -------------------------------------------
   bool Command::conditionApplies()
   {
     return this->condition == NULL ||
            this->condition->applies();
   }
   //---------------------------------------------------------------------------------------------------------
}