#pragma once

#include "Position.h"

#include <Util\Types.h>

namespace BW
{
  struct Bullet
  {
/*00*/  Bullet *previous;
/*04*/  Bullet *next;
/*08*/  u32 __unknown00[7];
/*24*/  u16 type;	// if this is 0x9D, this is a psi storm
/*26*/  u16 __unknown01;
/*28*/  BW::Position pos;
/*2C*/  u32 pos4_x;	// (pos4_x >> 4) == pos.x
/*30*/  u32 pos4_y;
/*34*/  u32 __unknown02[11];
/*60*/  u32 leftTime;  // use leftTime>>8 for frames
/*64*/  u32 __unknown03[3];
/*70*/

// size should be 112, 0x70

        int inline getLeftFrames()
        {
          return leftTime>>8;
        }
  };
}