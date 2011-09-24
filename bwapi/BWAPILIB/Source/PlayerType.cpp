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

    void init()
    {
      playerTypeName[None]                 = "None";
      playerTypeName[Computer]             = "Computer";
      playerTypeName[Player]               = "Player";
      playerTypeName[RescuePassive]        = "RescuePassive";
      playerTypeName[EitherPreferComputer] = "EitherPreferComputer";
      playerTypeName[EitherPreferHuman]    = "EitherPreferHuman";
      playerTypeName[Neutral]              = "Neutral";
      playerTypeName[Closed]               = "Closed";
      playerTypeName[PlayerLeft]           = "PlayerLeft";
      playerTypeName[ComputerLeft]         = "ComputerLeft";
      playerTypeName[Unknown]              = "Unknown";

      playerTypeSet.insert(None);
      playerTypeSet.insert(Computer);
      playerTypeSet.insert(Player);
      playerTypeSet.insert(RescuePassive);
      playerTypeSet.insert(EitherPreferComputer);
      playerTypeSet.insert(EitherPreferHuman);
      playerTypeSet.insert(Neutral);
      playerTypeSet.insert(Closed);
      playerTypeSet.insert(PlayerLeft);
      playerTypeSet.insert(ComputerLeft);
      playerTypeSet.insert(Unknown);

      foreach(PlayerType i, playerTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        playerTypeMap.insert(std::make_pair(name, i));
      }
      initializingPlayerType = false;
    }
  }
  PlayerType::PlayerType() : Type(PlayerTypes::None)
  {
  }
  int getValidPlayerTypeID(int id)
  {
    if ( !initializingPlayerType && (id < 0 || id >= 13 || playerTypeName[id].length() == 0) )
      return PlayerTypes::Unknown;
    return id;
  }
  PlayerType::PlayerType(int id) : Type( getValidPlayerTypeID(id) )
  {
  }
  const std::string &PlayerType::getName() const
  {
    return playerTypeName[this->getID()];
  }
  const char *PlayerType::c_str() const
  {
    return playerTypeName[this->getID()].c_str();
  }
  PlayerType PlayerTypes::getPlayerType(std::string name)
  {
    fixName(&name);
    std::map<std::string, PlayerType>::iterator i = playerTypeMap.find(name);
    if (i == playerTypeMap.end())
      return PlayerTypes::Unknown;
    return (*i).second;
  }
  const std::set<PlayerType>& PlayerTypes::allPlayerTypes()
  {
    return playerTypeSet;
  }
}
