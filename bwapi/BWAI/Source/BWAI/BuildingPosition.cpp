#include "BuildingPosition.h"

#include <Strings.h>
#include <tinyxml.h>

namespace BWAI
{      
  //------------------------------------- CONSTRUCTOR -------------------------
  BuildingPosition::BuildingPosition(TiXmlElement *xmlElement)
  {
    const char * widthAttribute = xmlElement->Attribute("width");
    if (widthAttribute == NULL)
      throw XmlException("Expected attribute width in <build-position> element");
    this->tileWidth = Util::Strings::stringToInt(widthAttribute);
    
    const char * heightAttribute = xmlElement->Attribute("height");
    if (heightAttribute == NULL)
      throw XmlException("Expected attribute height in <build-position> element");
    this->tileHeight = Util::Strings::stringToInt(heightAttribute);
    
    const char * shortcutAttribute = xmlElement->Attribute("shortcut");
    if (shortcutAttribute != NULL)
      this->shortcut = shortcutAttribute;
  
    for (TiXmlElement* position = xmlElement->FirstChildElement("position"); position != NULL; position = position->NextSiblingElement("position"))
      this->positions.push_back(BW::TilePosition(position));
  }
}