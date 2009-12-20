#pragma once
/************************************************************
 *  several data types containing details on KnownUnitEntry
 *  array events.
 *********/

#include "SharedStuff.h"
#include <BWAPITypes\UnitEvent.h>

namespace Bridge
{
  struct KnownUnitAddEventEntry
  {
    BWAPI::UnitAddEvent data;
  };
  struct KnownUnitRemoveEventEntry
  {
    BWAPI::UnitRemoveEvent data;
  };
}
