#pragma once

#include <Util/Types.h>
namespace BW { class Position; }
class TiXmlElement;

namespace BW
{
  /** Direct mapping of bw representation of map TilePosition. */
  class TilePosition
  {
    public :
      TilePosition();
      TilePosition(const Position& position);
      TilePosition(TiXmlElement* xmlElement);
      TilePosition(u16 x, u16 y);
      bool operator == (const BW::TilePosition& TilePosition) const;
      bool operator != (const BW::TilePosition& TilePosition) const;
      bool operator  < (const BW::TilePosition& TilePosition) const;
      u16 x;
      u16 y;
      bool isValid();
      static TilePosition Invalid;
  };
};

