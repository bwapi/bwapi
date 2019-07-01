#include <BWAPI/Event.h>
#include <BWAPI/EventType.h>
#include <tuple>

namespace BWAPI
{
  namespace { std::string emptyString; }
  bool Event::operator==(const Event& other) const
  {
    return std::tie(type, position, unit, player, winner, text)
      == std::tie(other.type, other.position, other.unit, other.player, other.winner, other.text);
  }
  Event Event::MatchStart()
  {
    Event e;
    e.type = EventType::MatchStart;
    return e;
  }
  Event Event::MatchEnd(bool isWinner)
  {
    Event e;
    e.type   = EventType::MatchEnd;
    e.winner = isWinner;
    return e;
  }
  Event Event::MatchFrame()
  {
    Event e;
    e.type = EventType::MatchFrame;
    return e;
  }
  Event Event::MenuFrame()
  {
    Event e;
    e.type = EventType::MenuFrame;
    return e;
  }
  Event Event::SendText(const std::string& text)
  {
    Event e;
    e.type = EventType::SendText;
    e.text = text;
    return e;
  }
  Event Event::ReceiveText(Player player, const std::string& text)
  {
    Event e;
    e.type   = EventType::ReceiveText;
    e.player = player;
    e.text = text;
    return e;
  }
  Event Event::PlayerLeft(Player player)
  {
    Event e;
    e.type   = EventType::PlayerLeft;
    e.player = player;
    return e;
  }
  Event Event::NukeDetect(Position target)
  {
    Event e;
    e.type     = EventType::NukeDetect;
    e.position = target;
    return e;
  }
  Event Event::UnitDiscover(Unit unit)
  {
    Event e;
    e.type = EventType::UnitDiscover;
    e.unit = unit;
    return e;
  }
  Event Event::UnitEvade(Unit unit)
  {
    Event e;
    e.type = EventType::UnitEvade;
    e.unit = unit;
    return e;
  }
  Event Event::UnitShow(Unit unit)
  {
    Event e;
    e.type = EventType::UnitShow;
    e.unit = unit;
    return e;
  }
  Event Event::UnitHide(Unit unit)
  {
    Event e;
    e.type = EventType::UnitHide;
    e.unit = unit;
    return e;
  }
  Event Event::UnitCreate(Unit unit)
  {
    Event e;
    e.type = EventType::UnitCreate;
    e.unit = unit;
    return e;
  }
  Event Event::UnitDestroy(Unit unit)
  {
    Event e;
    e.type = EventType::UnitDestroy;
    e.unit = unit;
    return e;
  }
  Event Event::UnitMorph(Unit unit)
  {
    Event e;
    e.type = EventType::UnitMorph;
    e.unit = unit;
    return e;
  }
  Event Event::UnitRenegade(Unit unit)
  {
    Event e;
    e.type = EventType::UnitRenegade;
    e.unit = unit;
    return e;
  }
  Event Event::SaveGame(const std::string& gameName)
  {
    Event e;
    e.type = EventType::SaveGame;
    e.text = gameName;
    return e;
  }
  Event Event::UnitComplete(Unit unit)
  {
    Event e;
    e.type = EventType::UnitComplete;
    e.unit = unit;
    return e;
  }
  EventType::Enum Event::getType() const
  {
    return type;
  }
  Position Event::getPosition() const
  {
    return *position;
  }
  const std::string& Event::getText() const
  {
    if (!text)
      return emptyString;
    return *text;
  }
  Unit Event::getUnit() const
  {
    return *unit;
  }
  Player Event::getPlayer() const
  {
    return *player;
  }
  bool Event::isWinner() const
  {
    return *winner;
  }
  Event& Event::setType(EventType::Enum type_)
  {
    this->type = type_;
    return *this;
  }
  Event& Event::setPosition(Position position_)
  {
    this->position = position_;
    return *this;
  }
  Event& Event::setText(const std::string& text_)
  {
    this->text = text_;
    return *this;
  }
  Event& Event::setUnit(Unit unit_)
  {
    this->unit = unit_;
    return *this;
  }
  Event& Event::setPlayer(Player player_)
  {
    this->player = player_;
    return *this;
  }
  Event& Event::setWinner(bool isWinner)
  {
    this->winner = isWinner;
    return *this;
  }
}