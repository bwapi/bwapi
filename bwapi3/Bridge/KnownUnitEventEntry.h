#pragma once
/************************************************************
 *  several data types containing details on KnownUnitEntry
 *  array events.
 *********/

#include "SharedStuff.h"

namespace Bridge
{
  struct KnownUnitAddEventEntry
  {
    SharedStuff::KnownUnitSet::Index unitIndex;
  };
  struct KnownUnitRemoveEventEntry
  {
    SharedStuff::KnownUnitSet::Index unitIndex;
  };
}
