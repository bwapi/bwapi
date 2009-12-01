#pragma once
#include "BW\Position.h"

namespace Bridge
{
  //---------------------------------------------------------
  // The main structure, unifiing everything
  //---------------------------------------------------------
  struct KnownUnit
  {
    // access for a unit the agent has knowledge of
    typedef int ClearanceLevel;
    enum ClearanceLevels
    {
      None = 0,
      Noticed = 1,
      Visible = 2,
      LoadedTame = 3,
      Tame = 4          // highest clearance level
    };
    // Unit's state we know about, depending on clearance level
    struct StateNoticed
    {
      int uniqueUnitID;
      BW::Position pos;
    };
    struct StateVisible : StateNoticed
    {
    };
    struct StateLoadedTame : StateVisible
    {
    };
    struct StateTame : StateLoadedTame
    {
    };
    typedef StateTame State;      // all states unified

    //---------------------------------------------------------
    // Finally, the data of KnownUnit
    //---------------------------------------------------------
    ClearanceLevel clearance;
    State state;
  };
}
