#pragma once

#include "../Types.h"
class TiXmlElement;

namespace BW
{
  /** Direct mapping of bw representation of map position. */
  class Position
  {
    public :
      Position();
      Position(TiXmlElement* xmlElement);
      Position(u16 x, u16 y);
      bool operator == (BW::Position& position);
      bool operator != (BW::Position& position);
      u16 x;
      u16 y;
  };
};

