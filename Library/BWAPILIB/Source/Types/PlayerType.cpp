#include <string>
#include <BWAPI/PlayerType.h>

namespace BWAPI
{
  template <>
  const std::string Type<PlayerType, PlayerTypes::Enum::Unknown>::typeNames[PlayerTypes::Enum::MAX] =
  {
    "None",
    "Computer",
    "Player",
    "RescuePassive",
    "RescueActive",
    "EitherPreferComputer",
    "EitherPreferHuman",
    "Neutral",
    "Closed",
    "Observer",
    "PlayerLeft",
    "ComputerLeft",
    "Unknown"
  };
  namespace PlayerTypeSet
  {
    using namespace PlayerTypes;
    const PlayerType::set playerTypeSet = { None, Computer, Player, RescuePassive, EitherPreferComputer, EitherPreferHuman,
      Neutral, Closed, PlayerLeft, ComputerLeft, Unknown };
  }
  bool PlayerType::isLobbyType() const
  {
    return this->getID() == PlayerTypes::Enum::EitherPreferComputer ||
           this->getID() == PlayerTypes::Enum::EitherPreferHuman    ||
           this->getID() == PlayerTypes::Enum::RescuePassive        ||
           this->getID() == PlayerTypes::Enum::RescueActive         ||
           this->getID() == PlayerTypes::Enum::Neutral;
  }
  bool PlayerType::isGameType() const
  {
    return this->getID() == PlayerTypes::Enum::Player         ||
           this->getID() == PlayerTypes::Enum::Computer       ||
           this->getID() == PlayerTypes::Enum::RescuePassive  ||
           this->getID() == PlayerTypes::Enum::RescueActive   ||
           this->getID() == PlayerTypes::Enum::Neutral;
  }
  const PlayerType::set& PlayerTypes::allPlayerTypes()
  {
    return PlayerTypeSet::playerTypeSet;
  }
}
