#pragma once
/***********************************************************************
 *  The BWAPI::Unit struct is a proxy of the BW::Unit struct, with
 *  additional data for lifetime detection
 *
 *******/

#include <Util\Types.h>
#include <Bridge\SharedStuff.h>

namespace Bridge { struct KnownUnitEntry; };

namespace BWAPI
{
  struct Unit
  {
    bool                                      isInChain;
    bool                                      wasInChain;
    bool                                      isDying;
    Bridge::KnownUnitEntry*                   knownUnit;
    Bridge::SharedStuff::KnownUnitSet::Index  knownUnitIndex; // needed for removing from .knownUnits
  };
}
