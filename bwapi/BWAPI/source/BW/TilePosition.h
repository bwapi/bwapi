#pragma once

#include "../Types.h"
class TiXmlElement;

namespace BW
{
  /** Direct mapping of bw representation of map TilePosition. */
  class TilePosition
  {
    public :
      TilePosition();
      TilePosition(TiXmlElement* xmlElement);
      TilePosition(u16 x, u16 y);
      bool operator == (BW::TilePosition& TilePosition);
      bool operator != (BW::TilePosition& TilePosition);
      u16 x;
      u16 y;
  };
};

