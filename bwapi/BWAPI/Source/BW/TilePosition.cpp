#include "TilePosition.h"

#include <tinyXml.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>
#include "Position.h"
#include "Offsets.h"

namespace BW
{
  TilePosition TilePosition::Invalid = TilePosition(1000,1000);
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition()
  :x(0)
  ,y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(TiXmlElement* xmlElement)
  {
    this->x = Util::Xml::getRequiredU16Attribute(xmlElement, "x");
    this->y = Util::Xml::getRequiredU16Attribute(xmlElement, "y");
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(u16 x, u16 y)
  :x(x)
  ,y(y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(const Position& position)
  :x(position.x/BW::TILE_SIZE)
  ,y(position.y/BW::TILE_SIZE)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TilePosition::operator == (TilePosition& TilePosition)
  {
    return this->x == TilePosition.x &&
           this->y == TilePosition.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool TilePosition::operator != (TilePosition& TilePosition)
  {
    return this->x != TilePosition.x ||
           this->y != TilePosition.y;
  }
  //----------------------------------------------------------------------------------------------------------
  bool TilePosition::isValid()
  {
    return *this != Invalid;
  }
  //----------------------------------------------------------------------------------------------------------
};
