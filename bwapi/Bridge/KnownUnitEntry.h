#pragma once
/************************************************************
 *  KnownUnitEntry capsulates a shared memory unit entry of
 *  units that the AI can possibly know about. When a unit
 *  becomes unaccessible, there is no special clearance level,
 *  it just gets removed
 *
 *  Note that this is the "information behaviour". BWAgent's
 *  compatibility Unit layer will provide another.
 *********/

#include <BWAPITypes\UnitState.h>

namespace Bridge
{
  struct KnownUnitEntry
  {
    BWAPI::ClearanceLevel clearance;
    BWAPI::UnitState state;

    // TODO: add issued command
  };
}
