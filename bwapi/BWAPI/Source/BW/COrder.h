#pragma once
#include "Target.h"

#pragma pack(1)
namespace BW
{
  // Must be on a 4-byte alignment
  class COrder
  {
  public:
    BW::COrder*  prev;
    BW::COrder*  next;
    u16         orderID;
    u16         unitType;
    BW::Target  target;
  };

  static_assert( sizeof(COrder) == 20, "BW::COrder is incorrect." );
};
#pragma pack()
