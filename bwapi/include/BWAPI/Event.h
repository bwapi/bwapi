#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/EventType.h>
#include <BWAPI/Unit.h>

#include <string>
#include <optional>

namespace BWAPI
{
  class Player;
  
  class Event
  {
    public:
      Event() = default;
      bool operator==(const Event& other) const;
      static Event MatchStart();
      static Event MatchEnd(bool isWinner);
      static Event MatchFrame();
      static Event MenuFrame();
      static Event SendText(const std::string& text);
      static Event ReceiveText(Player player, const std::string& text);
      static Event PlayerLeft(Player player);
      static Event NukeDetect(Position target);
      static Event UnitDiscover(Unit unit);
      static Event UnitEvade(Unit unit);
      static Event UnitShow(Unit unit);
      static Event UnitHide(Unit unit);
      static Event UnitCreate(Unit unit);
      static Event UnitDestroy(Unit unit);
      static Event UnitMorph(Unit unit);
      static Event UnitRenegade(Unit unit);
      static Event SaveGame(const std::string& gameName);
      static Event UnitComplete(Unit unit);
      EventType::Enum getType() const;
      Position getPosition() const;
      const std::string& getText() const;
      Unit getUnit() const;
      Player getPlayer() const;
      bool isWinner() const;

      Event& setType(EventType::Enum type);
      Event& setPosition(Position position);
      Event& setText(const std::string& text);
      Event& setUnit(Unit unit);
      Event& setPlayer(Player player);
      Event& setWinner(bool isWinner);
    private:
      std::optional<Position>    position;
      std::optional<std::string> text;
      std::optional<Unit>        unit;
      std::optional<Player>      player;
      EventType::Enum            type = EventType::None;
      std::optional<bool>        winner = false;
  };
}