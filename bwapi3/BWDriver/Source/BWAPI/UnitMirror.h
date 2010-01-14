#pragma once
/***********************************************************************
 *  The UnitMirror struct is a local proxy for the BW::Unit struct, with
 *  variables for lifetime detection
 *
 *******/

#include "UnitChainId.h"

#include <Util\Types.h>

#include <Bridge\SharedStuff.h>

#include <BWAPITypes\UnitId.h>

namespace BWAPI
{
  struct UnitMirror
  {
    // chain holds the chain in which this unit slot was found to
    // detect chain changes
    UnitChainId chain;

    // is true several frames before the unit gets removed from the chain
    bool        isDying;

    // is true then the unit is visible or cloaked but was been spotted
    bool        isNoticed;

    // position is needed to detect changes, for noticing the unit (isNoticed).
    Position    position;

    // if this unit is known, this is it's personal KnownUnit entry
    KnownUnit*  knownUnit;       // saving, to not to have to recompute
    UnitId      knownUnitIndex;  // needed for removing from units
  };
}
