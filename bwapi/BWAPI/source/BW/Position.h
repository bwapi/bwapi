#pragma once

#include "../Types.h"

namespace BW
{
  /** Direct mapping of bw representation of map position. */
  class Position
  {
    public :
      Position();
      Position(u16 x, u16 y);
      u16 x;
      u16 y;
  };
};
