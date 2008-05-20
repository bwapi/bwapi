#include "Position.h"
#include "..//..//..//TinyXml//tinyXml.h"
#include <Exceptions.h>

namespace BW
{
  //------------------------------------- CONSTRUCTOR ------------------------------
  Position::Position()
  :x(0)
  ,y(0)
  {
  }
  //------------------------------------- CONSTRUCTOR ------------------------------
  Position::Position(TiXmlElement* xmlElement)
  {
    const char * xAttribute = xmlElement->Attribute("x");
    if (xAttribute == NULL)
      throw XmlException("Expected attribute x in <position> element");
    
    if(sscanf(xAttribute, "%d", &this->x) == EOF)
      throw XmlException((std::string)"attribute x in <position> is not numeric but '" + xAttribute + "'");

    const char * yAttribute = xmlElement->Attribute("y");
    if (yAttribute == NULL)
      throw XmlException("Expected attribute y in <position> element");
    if(sscanf(xAttribute, "%d", &this->y) == EOF)
      throw XmlException((std::string)"attribute y in <position> is not numeric but '" + yAttribute + "'");
  }
  //------------------------------------- DESTRUCTOR -------------------------------
  Position::Position(u16 x, u16 y)
  :x(x)
  ,y(y)
  {
  }
};
