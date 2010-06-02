#pragma once

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include <BW/Offsets.h>

namespace BW
{
#pragma pack(1)
  struct Bullet
  {
	  Bullet *next;
	  Bullet *previous;
	  u32 __unknown00[7];
	  u16 type;	// if this is 0x9D, this is a psi storm
	  u16 __unknown01;
	  u16 pos_x;
	  u16 pos_y;
	  u32 pos4_x;	// (pos4_x >> 4) == pos_x
	  u32 pos4_y;
	  u32 __unknown02[11];
	  u32 time_left;  // use time_left>>8 for frames
  };
#pragma pack()
  /** Direct maping of the UnitNodeTable in bw memory. */
  struct BulletArray
  {
    Bullet bullet[BULLET_ARRAY_MAX_LENGTH];
  };
};