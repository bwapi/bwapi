#include "MapStartingPosition.h"

#include <Exceptions.h>
#include <tinyXml.h>

#include "MapInfo.h"
#include "BuildingPosition.h"
namespace BWAI
{
  //------------------------------- CONSTRUCTOR -------------------------------
  MapStartingPosition::MapStartingPosition(TiXmlElement* xmlElement, MapInfo* mapInfo)
  {
    const char * expansionIDAttribute = xmlElement->Attribute("expansion-id");
    if (expansionIDAttribute == NULL)
      throw XmlException("Expected attribute expansion-id in <starting-position> element");
    this->expansion = mapInfo->getExpansion(expansionIDAttribute);

    TiXmlNode* node = xmlElement->FirstChild("standard-building-placement");
    if (node == NULL)
      throw XmlException("Expected element <standard-building-placement> in <starting-position>");
    TiXmlElement* root = node->ToElement();
    
    for (TiXmlElement* buildPositionElement = root->FirstChildElement("build-position"); buildPositionElement != NULL; buildPositionElement = buildPositionElement->NextSiblingElement("build-position"))
    {
      const char * nameAttribute = buildPositionElement->Attribute("name");
      
      if (nameAttribute == NULL)
        throw XmlException("Expected attribute name in <build-position> element");
      
      this->positions.insert(std::pair<std::string, BuildingPosition* >(nameAttribute, new BuildingPosition(buildPositionElement)));
    }
  }
  //---------------------------------------------------------------------------
  MapStartingPosition::~MapStartingPosition()
  {
    for (std::map<std::string, BuildingPosition*>::iterator i = this->positions.begin();
         i != this->positions.end();
         i++)
     delete (*i).second;
  }
  //---------------------------------------------------------------------------
}