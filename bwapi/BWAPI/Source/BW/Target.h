#pragma once

#include "Position.h"
#pragma pack(1)

namespace BW
{
  class CUnit;

  /** The BW::Target structure is a position followed by a unit pointer. This structure is common
   *  and seen in CUnit, CBullet, COrder, CAIControl, AI Captain, and others.
   */
  struct Target
  {
    BW::Position  pt;       // official name
    CUnit*        pUnit;
  };

}
#pragma pack()
