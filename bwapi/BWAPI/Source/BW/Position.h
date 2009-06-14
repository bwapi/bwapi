#pragma once

#include <Util/Types.h>
namespace BW { class TilePosition; }

namespace BW
{
  /** Direct mapping of bw representation of map position. */
  class Position
  {
    public :
      Position();
      Position(const BW::TilePosition& position);
      Position(u16 x, u16 y);
      bool operator == (const BW::Position& position) const;
      bool operator != (const BW::Position& position) const;
      bool operator  < (const BW::Position& position) const;
      u16 getDistance(const Position &position) const;
      u16 x;
      u16 y;
  };
};

