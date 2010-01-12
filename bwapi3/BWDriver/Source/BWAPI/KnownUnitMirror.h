#pragma once
/***********************************************************************
 *  The KnownUnitMirror struct is a proxy of the BWAPI::KnownUnit struct,
 *  with additional private data the AI need not to know
 *
 *******/

#include <Util\Types.h>

namespace BWAPI
{
  struct KnownUnitMirror
  {
    int bwUnitId;  // look up the bw unit, when processing a command
  };
}
