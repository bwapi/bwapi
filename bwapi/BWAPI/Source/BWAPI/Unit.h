#pragma once
/***********************************************************************
 *  The BWAPI::Unit struct is a proxy of the BW::Unit struct, with
 *  additional data for lifetime detection
 *
 *******/

#include <Util\Types.h>

#include <Bridge\KnownUnitEntry.h>

namespace BWAPI
{
  struct Unit
  {
    bool                    exists;
    u16                     bwId;
    Bridge::KnownUnitEntry* knownUnit;
  };
}
