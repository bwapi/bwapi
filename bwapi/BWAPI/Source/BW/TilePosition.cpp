#include "TilePosition.h"

#include <tinyXml.h>
#include <Exceptions.h>

namespace BW
{
  //------------------------------------- CONSTRUCTOR ------------------------------
  TilePosition::TilePosition()
  :x(0)
  ,y(0)
  {
  }
  //------------------------------------- CONSTRUCTOR ------------------------------
  TilePosition::TilePosition(TiXmlElement* xmlElement)
  {
    const char * xAttribute = xmlElement->Attribute("x");
    if (xAttribute == NULL)
      throw XmlException("Expected attribute x in <tile-position> element");
    
    if(sscanf(xAttribute, "%d", &this->x) == EOF)
      throw XmlException((std::string)"attribute x in <tile-position> is not numeric but '" + xAttribute + "'");

    const char * yAttribute = xmlElement->Attribute("y");
    if (yAttribute == NULL)
      throw XmlException("Expected attribute y in <tile-position> element");
    if(sscanf(yAttribute, "%d", &this->y) == EOF)
      throw XmlException((std::string)"attribute y in <tile-position> is not numeric but '" + yAttribute + "'");
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
};
