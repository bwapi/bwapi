#include "MapStartingPosition.h"

#include <tinyXml.h>
#include <Util/Xml.h>
#include <Util/Exceptions.h>

#include "MapInfo.h"
#include "BuildingPositionSet.h"

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  MapStartingPosition::MapStartingPosition(TiXmlElement* xmlElement, MapInfo* mapInfo)
  {
    std::string expansionIDAttribute = Util::Xml::getRequiredAttribute(xmlElement, "expansion-id");
    this->expansion = mapInfo->getExpansion(expansionIDAttribute);

    TiXmlElement* root = xmlElement->FirstChildElement("standard-building-placement");
    if (root == NULL)
      throw XmlException("Expected element <standard-building-placement> in <starting-position>");
    
    for (TiXmlElement* buildPositionElement = root->FirstChildElement("build-position"); buildPositionElement != NULL; buildPositionElement = buildPositionElement->NextSiblingElement("build-position"))
    {
      const char * nameAttribute = buildPositionElement->Attribute("name");
      
      if (nameAttribute == NULL)
        throw XmlException("Expected attribute name in <build-position> element");
      
      this->positions.insert(std::pair<std::string, BuildingPositionSet* >(nameAttribute, new BuildingPositionSet(buildPositionElement)));
    }
  }
  //----------------------------------------------------------------------------------------------------------
  MapStartingPosition::~MapStartingPosition()
  {
    for each (std::pair<std::string, BuildingPositionSet*> i in this->positions)
     delete i.second;
  }
  //----------------------------------------------------------------------------------------------------------
}