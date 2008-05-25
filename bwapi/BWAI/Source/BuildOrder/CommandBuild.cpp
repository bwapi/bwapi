#include "CommandBuild.h"

#include <Exceptions.h>
#include <StringUtil.h>

namespace BuildOrder
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  CommandBuild::CommandBuild(TiXmlElement* xmlElement)
  {
    const char * buildName = xmlElement->Attribute("name");
    if (buildName == NULL)
      throw XmlException("Expected attribute name in <build> element");
    this->name = buildName;
    
    const char * buildPlace = xmlElement->Attribute("place");
    if (buildPlace == NULL)
      throw XmlException("Expected attribute place in <build> element");
    this->place = buildPlace;
    
    const char * minimalPopulationAttribute = xmlElement->Attribute("minimal-population");
    if (minimalPopulationAttribute == NULL)
      throw XmlException("Expected attribute minimal-population in <build> element");
    this->minimalPopulation = StringUtil::stringToInt(minimalPopulationAttribute);
  }
  //---------------------------------------------------------------------------
}