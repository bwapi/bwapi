#include <string>
#include <BWAPI/PlayerType.h>

#include "Common.h"

#include "../../Debug.h"

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
    using namespace PlayerTypes::Enum;
    const PlayerType::set playerTypeSet = { None, Computer, Player, RescuePassive, EitherPreferComputer, EitherPreferHuman,
      Neutral, Closed, PlayerLeft, ComputerLeft, Unknown };
  }
  namespace PlayerTypes
  {
    BWAPI_TYPEDEF(PlayerType,None);
    BWAPI_TYPEDEF(PlayerType,Computer);
    BWAPI_TYPEDEF(PlayerType,Player);
    BWAPI_TYPEDEF(PlayerType,RescuePassive);
    // Rescue Active
    
    BWAPI_TYPEDEF(PlayerType,EitherPreferComputer);
    BWAPI_TYPEDEF(PlayerType,EitherPreferHuman);
    BWAPI_TYPEDEF(PlayerType,Neutral);
    BWAPI_TYPEDEF(PlayerType,Closed);
    // Observer
    BWAPI_TYPEDEF(PlayerType,PlayerLeft);
    BWAPI_TYPEDEF(PlayerType,ComputerLeft);
    BWAPI_TYPEDEF(PlayerType,Unknown);

  }
  PlayerType::PlayerType(int id) : Type( id )
  {
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
