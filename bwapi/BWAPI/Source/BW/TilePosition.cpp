#include "TilePosition.h"

#include <tinyXml.h>
#include <Util/Exceptions.h>
#include <Util/Xml.h>

namespace BW
{
  TilePosition TilePosition::Invalid = TilePosition(256,256);
  //------------------------------------- CONSTRUCTOR ------------------------------
  TilePosition::TilePosition()
  :x(0)
  ,y(0)
  {
  }
  //------------------------------------- CONSTRUCTOR ------------------------------
  TilePosition::TilePosition(TiXmlElement* xmlElement)
  {
    this->x = Util::Xml::getRequiredU16Attribute(xmlElement, "x");
    this->y = Util::Xml::getRequiredU16Attribute(xmlElement, "y");
  }
  //------------------------------------- DESTRUCTOR -------------------------------
  TilePosition::TilePosition(u16 x, u16 y)
  :x(x)
  ,y(y)
  {
  }
  //------------------------------------- OPERATOR == ------------------------------
  bool TilePosition::operator == (BW::TilePosition& TilePosition)
  {
    return this->x == TilePosition.x &&
           this->y == TilePosition.y;
  }
  //------------------------------------- OPERATOR != ------------------------------
  bool TilePosition::operator != (BW::TilePosition& TilePosition)
  {
    return this->x != TilePosition.x ||
           this->y != TilePosition.y;
  }
  //--------------------------------------------------------------------------------
  bool TilePosition::isValid()
  {
    return *this != Invalid;
  }
  //--------------------------------------------------------------------------------
};
