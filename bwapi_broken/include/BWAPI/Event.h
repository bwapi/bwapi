#pragma once
#include <BWAPI/EventType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Position.h>
#include <string>

namespace BWAPI
{
  class Event
  {
    public:
      Event();
      bool operator==(const Event& other);
      static Event MatchStart();
      static Event MatchEnd(bool isWinner);
      static Event MatchFrame();
      static Event MenuFrame();
      static Event SendText(std::string text);
      static Event ReceiveText(Player* player, std::string text);
      static Event PlayerLeft(Player* player);
      static Event NukeDetect(Position target);
      static Event UnitDiscover(Unit* unit);
      static Event UnitEvade(Unit* unit);
      static Event UnitShow(Unit* unit);
      static Event UnitHide(Unit* unit);
      static Event UnitCreate(Unit* unit);
      static Event UnitDestroy(Unit* unit);
      static Event UnitMorph(Unit* unit);
      static Event UnitRenegade(Unit* unit);
      static Event SaveGame(std::string gameName);
      EventType::Enum type;
      Position position;
      std::string text;
      Unit* unit;
      Player* player;
      bool isWinner;
  };
}