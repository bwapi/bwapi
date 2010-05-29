#pragma once
#include <Util/Types.h>

namespace BW
{
#pragma pack(1)
  struct BINDialog
  {
    u8 _unknown1[2652];
    u8 player1Race1;
    u8 _unknown2[5];
    u8 player1Race2;
  };
#pragma pack()
};