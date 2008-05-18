#include "MapExpansion.h"
#include "../../TinyXml/tinyXml.h"
#include "../../Util/Exceptions.h"
namespace BWAI
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  MapExpansion::MapExpansion(TiXmlElement* xmlElement)
  {
    const char * idAttribute = xmlElement->Attribute("id");
    if (idAttribute == NULL)
      throw XmlException("Expected attribute id in <expansion> element");
    this->setID(idAttribute);   

    TiXmlElement* positionElement = xmlElement->FirstChildElement("position");
    if (positionElement == NULL)
      throw XmlException("Excepted sub-element <position> in element <expansion>");
    this->setPosition(BW::Position(positionElement));
  }
  //------------------------------- SET POSITION ------------------------------
  void MapExpansion::setPosition(BW::Position position)
  {
    this->position = position;
  }
  //------------------------------- GET POSITION ------------------------------
  const BW::Position& MapExpansion::getPosition(void) const
  {
    return this->position; 
  }
  //--------------------------------- SET ID ----------------------------------
  void MapExpansion::setID(const std::string& id)
  {
    this->id = id;
  }
  //---------------------------------------------------------------------------
  const std::string& MapExpansion::getID(void) const
  {
    return this->id;
  }
  //---------------------------------------------------------------------------
}
