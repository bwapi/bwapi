#include "MapStartingPosition.h"
#include "MapInfo.h"
#include "../../TinyXml//tinyXml.h"
#include "../../Util/Exceptions.h"
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
    
    TiXmlNode* nonProducing3X2= root->FirstChild("non-producting-3x2");
    if (nonProducing3X2 == NULL)
      throw XmlException("Expected element <non-producting-3x2> in <standard-building-placement>");
    
    for (TiXmlElement* position = nonProducing3X2->FirstChildElement("position"); position != NULL; position = position->NextSiblingElement("position"))
      this->nonProducing3X2BuildingPositions.push_back(BW::Position(position));

  }
  //---------------------------------------------------------------------------
}