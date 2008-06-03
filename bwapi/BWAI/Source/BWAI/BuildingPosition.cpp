#include "BuildingPosition.h"

#include <Util/Strings.h>
#include <tinyxml.h>
#include <Util/Xml.h>

namespace BWAI
{      
  //------------------------------------- CONSTRUCTOR -------------------------
  BuildingPosition::BuildingPosition(TiXmlElement *xmlElement)
  {
    this->tileWidth  = Util::Xml::getRequiredU16Attribute(xmlElement, "width");
    this->tileHeight = Util::Xml::getRequiredU16Attribute(xmlElement, "height");
        
    const char * shortcutAttribute = xmlElement->Attribute("shortcut");
    if (shortcutAttribute != NULL)
      this->shortcut = shortcutAttribute;
  
    for (TiXmlElement* position = xmlElement->FirstChildElement("position"); position != NULL; position = position->NextSiblingElement("position"))
      this->positions.push_back(BW::TilePosition(position));
  }
}