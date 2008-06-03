#include "Xml.h"

#include "Exceptions.h"
#include "../../../TinyXml/Source/tinyxml.h"

namespace Util
{
  //------------------------------------ GET REQUIRED ATTRIBUTE ----------------------------------------------
  std::string Xml::getRequiredAttribute(TiXmlElement* element, const char* name)
  {
    const char* attribute = element->Attribute(name);
    if (attribute == NULL)
      throw XmlException(std::string("Expected attribute '") + name + "' in " + std::string(element->ValueTStr().c_str()) + " element");
    return attribute;
  }
  //------------------------------------- GET REQUIRED ATTRIBUTE ---------------------------------------------
  u16 Xml::getRequiredU16Attribute(TiXmlElement* element, const char* name)
  {
    const char* attribute = element->Attribute(name);
      if (attribute == NULL)
        throw XmlException(std::string("Expected attribute '") + name + "' in <" + std::string(element->ValueTStr().c_str()) + "> element");
  
    u16 result;
    if(sscanf(attribute, "%d", &result) == EOF)
      throw XmlException((std::string)"attribute '" + name + "' in <" + std::string(element->ValueTStr().c_str()) + "> element is not numeric but '" + attribute + "'");
    return result;      
  }
  //----------------------------------------------------------------------------------------------------------
}