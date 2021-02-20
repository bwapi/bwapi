#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/EventType.h>

#include <string>

namespace BWAPI4
{
  // Forward Declarations
  class PlayerImpl;
  typedef PlayerImpl* Player;

  class UnitImpl;
  typedef UnitImpl* Unit;

  class Event
  {
    public:
      Event() = default;
      Event(const Event& other);
      Event(Event&& other);
      Event& operator=(const Event& other);
      Event& operator=(Event &&other);
      bool operator==(const Event& other) const;
      static Event MatchStart();
      static Event MatchEnd(bool isWinner);
      static Event MatchFrame();
      static Event MenuFrame();
      static Event SendText(const std::string& text);
      static Event ReceiveText(Player player, const std::string& text);
      static Event PlayerLeft(Player player);
      static Event NukeDetect(BWAPI::Position target);
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
      BWAPI::EventType::Enum getType() const;
      BWAPI::Position getPosition() const;
      const std::string& getText() const;
      Unit getUnit() const;
      Player getPlayer() const;
      bool isWinner() const;

      Event& setType(BWAPI::EventType::Enum type);
      Event& setPosition(BWAPI::Position position);
      Event& setText(const std::string& text);
      Event& setUnit(Unit unit);
      Event& setPlayer(Player player);
      Event& setWinner(bool isWinner);
      //static Event TriggerAction();
    private:
      BWAPI::Position        position = BWAPI::Positions::None;
      std::string     text;
      Unit            unit = nullptr;
      Player          player = nullptr;
      BWAPI::EventType::Enum type = BWAPI::EventType::None;
      bool            winner = false;
  };
}