#pragma once

#include <BWAPI\EventType.h>

namespace BWAPIC
{ 
  struct Event
  {
    BWAPI::EventType::Enum type;
    int x;
    int y;
    int unitID;
    int playerID;
    bool isWinner;
  };
}