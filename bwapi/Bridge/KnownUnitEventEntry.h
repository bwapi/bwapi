#pragma once
/************************************************************
 *  several data types containing details on KnownUnitEntry
 *  array events.
 *********/

#include "SharedStuff.h"

namespace Bridge
{
  struct KnownUnitAdded
  {
    SharedStuff::KnownUnitEntry::Index unit;
  };
  struct KnownUnitRemoved
  {
    SharedStuff::KnownUnitEntry::Index unit;
  };
}
