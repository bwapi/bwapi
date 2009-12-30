#pragma once
/**
 * These entries are packets in the per-frame stack SharedStuff::EventStack
 */

#include "SharedStuff.h"

#include <BWAPITypes\UnitEvent.h>

#include <Util\TypeHead.h>

namespace Bridge
{
  namespace EventEntry
  {
#pragma pack(push, 1)
#define UNIQUE_ID __LINE__
    struct KnownUnitAdd : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::UnitAddEvent data;
    };

    struct KnownUnitRemove : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::UnitRemoveEvent data;
    };

    struct PlayerLeave : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::PlayerId player;
    };
#undef UNIQUE_ID
#pragma pack(pop)
  }
}
