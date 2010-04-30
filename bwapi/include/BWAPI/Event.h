#pragma once
#include <BWAPI/EventType.h>
#include <list>

namespace BWAPI
{
  class Event
  {
    public:
      EventType::Enum type;
      Position position;
      Unit* unit;
      Player* player;
      bool isWinner;
  }
}