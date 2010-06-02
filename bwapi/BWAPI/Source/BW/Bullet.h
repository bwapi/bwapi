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
    /*0x00*/ BW::Bullet*  nextBullet;
	  /*0x04*/ BW::Bullet*  previousBullet;
	  /*0x08*/ u32          __unknown00[7];
	  /*0x24*/ u16          type; // if this is 0x9D, this is a psi storm
	  /*0x26*/ u16          __unknown01;
	  /*0x28*/ u16          pos_x;
	  /*0x2A*/ u16          pos_y;
	  /*0x2C*/ s32          pos4_x; // (pos4_x >> 4) == pos_x
	  /*0x30*/ s32          pos4_y;
	  /*0x34*/ s32          __unknown02[11];
	  /*0x60*/ s32          time_left; // use time_left>>8 for frames
    /*0x64*/ BW::Unit*    owner;
    /*0x68*/ u32          __unknown03[2];
  };
#pragma pack()
  /** Direct maping of the UnitNodeTable in bw memory. */
  struct BulletArray
  {
    Bullet bullet[BULLET_ARRAY_MAX_LENGTH];
  };
};