#include "MapExpansion.h"

#include <tinyXml.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>

namespace BWAI
{
  //---------------------------------- CONSTRUCTOR ----------------------------
  MapExpansion::MapExpansion(TiXmlElement* xmlElement)
  {
    this->id = Util::Xml::getRequiredAttribute(xmlElement, "id");

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
  //--------------------------------- GET ID ----------------------------------
  const std::string& MapExpansion::getID(void) const
  {
    return this->id;
  }
  //---------------------------------------------------------------------------
}
