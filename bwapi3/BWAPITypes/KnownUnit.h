#pragma once
/************************************************************
 *  KnownUnit capsulates a shared memory unit entry of
 *  units that the AI can possibly know about. When a unit
 *  becomes unaccessible, there is no special clearance level,
 *  it just gets removed
 *
 *  Note that this is the "information behaviour". BWAgent's
 *  compatibility layer's (BWAPI2) Unit will provide another.
 *********/

#include "UnitState.h"

namespace BWAPI
{
  struct KnownUnit
  {
    BWAPI::UnitState state;
  };
}
