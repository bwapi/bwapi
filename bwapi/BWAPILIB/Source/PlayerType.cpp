#include <string>
#include <map>
#include <set>
#include <BWAPI/PlayerType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingPlayerType = true;
  std::string playerTypeName[13];
  std::map<std::string, PlayerType> playerTypeMap;
  std::set< PlayerType > playerTypeSet;
  namespace PlayerTypes
  {
    const PlayerType None(0);
    const PlayerType Computer(1);
    const PlayerType Player(2);
    const PlayerType RescuePassive(3);
    // Rescue Active
    const PlayerType EitherPreferComputer(5);
    const PlayerType EitherPreferHuman(6);
    const PlayerType Neutral(7);
    const PlayerType Closed(8);
    // Observer
    const PlayerType PlayerLeft(10);
    const PlayerType ComputerLeft(11);
    const PlayerType Unknown(12);

#define SET_PLAYER_TYPE(x) playerTypeName[x] = underscoresToSpaces(#x); playerTypeSet.insert(x);
    void init()
    {
      SET_PLAYER_TYPE(None);
      SET_PLAYER_TYPE(Computer);
      SET_PLAYER_TYPE(Player);
      SET_PLAYER_TYPE(RescuePassive);
      SET_PLAYER_TYPE(EitherPreferComputer);
      SET_PLAYER_TYPE(EitherPreferHuman);
      SET_PLAYER_TYPE(Neutral);
      SET_PLAYER_TYPE(Closed);
      SET_PLAYER_TYPE(PlayerLeft);
      SET_PLAYER_TYPE(ComputerLeft);
      SET_PLAYER_TYPE(Unknown);

      foreach(PlayerType i, playerTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        playerTypeMap.insert(std::make_pair(name, i));
      }
      initializingPlayerType = false;
    }
  }
  PlayerType::PlayerType()
  {
    this->id = PlayerTypes::None;
  }
  PlayerType::PlayerType(int id)
  {
    this->id = id;
    if (!initializingPlayerType && (id < 0 || id >= 13 || playerTypeName[id].length() == 0) )
      this->id = PlayerTypes::Unknown;
  }
  PlayerType::PlayerType(const PlayerType& other)
  {
    this->id = other;
  }
  PlayerType& PlayerType::operator=(const PlayerType& other)
  {
    this->id = other;
    return *this;
  }
  PlayerType::operator int() const
  {
    return id;
  }
  int PlayerType::getID() const
  {
    return this->id;
  }
  std::string PlayerType::getName() const
  {
    return playerTypeName[this->id];
  }
  const char *PlayerType::c_str() const
  {
    return playerTypeName[this->id].c_str();
  }
  PlayerType PlayerTypes::getPlayerType(std::string name)
  {
    fixName(&name);
    std::map<std::string, PlayerType>::iterator i = playerTypeMap.find(name);
    if (i == playerTypeMap.end())
      return PlayerTypes::Unknown;
    return (*i).second;
  }
  std::set<PlayerType>& PlayerTypes::allPlayerTypes()
  {
    return playerTypeSet;
  }
}
