#pragma once
#pragma pack(1)
#include "Position.h"

namespace BW
{
  struct Path
  {
    u8 unknown[30];
    u8 stepNumber;
    u8 stepIndex;
    Position steps[4];
    //unsure of exact size
  };
};
#pragma pack()
