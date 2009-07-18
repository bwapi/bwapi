#include "MapExpansion.h"

#include <tinyXml.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  MapExpansion::MapExpansion(TiXmlElement* xmlElement)
  {
    this->id = Util::Xml::getRequiredAttribute(xmlElement, "id");

    TiXmlElement* positionElement = xmlElement->FirstChildElement("position");
    if (positionElement == NULL)
      throw XmlException("Excepted sub-element <position> in element <expansion>");
    this->setPosition(BWAPI::Position(Util::Xml::getRequiredU16Attribute(positionElement, "x"), Util::Xml::getRequiredU16Attribute(positionElement, "y")));
  }
  //---------------------------------------------- SET POSITION ----------------------------------------------
  void MapExpansion::setPosition(BWAPI::Position position)
  {
    this->position = position;
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  const BWAPI::Position& MapExpansion::getPosition(void) const
  {
    return this->position;
  }
  //------------------------------------------------- SET ID -------------------------------------------------
  void MapExpansion::setID(const std::string& id)
  {
    this->id = id;
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  const std::string& MapExpansion::getID(void) const
  {
    return this->id;
  }
  //----------------------------------------------------------------------------------------------------------
}
