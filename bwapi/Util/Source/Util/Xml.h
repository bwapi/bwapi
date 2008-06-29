#include "Types.h"
#include <string>

class TiXmlElement;

namespace Util
{
  class Xml
  {
    public :
      /**
       * Gets element required attribute
       * @param element tiny xml representation of element that should contain the attribute
       * @name Name of the attribute to load
       * @throws XmlException if the attribute isn't present.
       */
      static std::string getRequiredAttribute(TiXmlElement* element, const char* name);
      static std::string getOptionalAttribute(TiXmlElement* element, const char* name, const char* defaultValue);
      static u16 getRequiredU16Attribute(TiXmlElement* element, const char* name);
      static u16 getOptionalU16Attribute(TiXmlElement* element, const char* name, u16 defaultValue);
  };
}