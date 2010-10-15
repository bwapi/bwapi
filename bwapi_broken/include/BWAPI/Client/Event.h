#pragma once

#include <BWAPI\Event.h>
#include <BWAPI\EventType.h>

namespace BWAPIC
{ 
  struct Event
  {
    BWAPI::EventType::Enum type;
    int v1;
    int v2;
  };
}