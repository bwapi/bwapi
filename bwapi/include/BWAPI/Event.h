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
      ~Event();
      Event(const Event& other);
      Event& operator=(const Event& other);
      bool operator==(const Event& other) const;
      static Event MatchStart();
      static Event MatchEnd(bool isWinner);
      static Event MatchFrame();
      static Event MenuFrame();
      static Event SendText(const char* text = NULL);
      static Event ReceiveText(Player* player, const char* text = NULL);
      static Event PlayerLeft(Player* player);
      static Event PlayerDropped(Player* player);
      static Event NukeDetect(Position target);
      static Event UnitDiscover(Unit* unit);
      static Event UnitEvade(Unit* unit);
      static Event UnitShow(Unit* unit);
      static Event UnitHide(Unit* unit);
      static Event UnitCreate(Unit* unit);
      static Event UnitDestroy(Unit* unit);
      static Event UnitMorph(Unit* unit);
      static Event UnitRenegade(Unit* unit);
      static Event SaveGame(const char* gameName = NULL);
      static Event UnitComplete(Unit *unit);
      EventType::Enum getType() const;
      Position getPosition() const;
      const std::string& getText() const;
      Unit* getUnit() const;
      Player* getPlayer() const;
      bool isWinner() const;

      Event& setType(EventType::Enum type);
      Event& setPosition(Position position);
      Event& setText(const char* text);
      Event& setUnit(Unit* unit);
      Event& setPlayer(Player* player);
      Event& setWinner(bool isWinner);
      //static Event TriggerAction();
    private:
      EventType::Enum type;
      Position position;
      std::string* text;
      Unit* unit;
      Player* player;
      bool winner;
  };
}