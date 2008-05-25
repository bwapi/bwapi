#include "CommandBuild.h"

#include <Exceptions.h>

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
  }
  //---------------------------------------------------------------------------
}