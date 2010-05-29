#include <string>
#include <map>
#include <set>
#include <BWAPI/PlayerType.h>

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
    const PlayerType EitherPreferComputer(5);
    const PlayerType EitherPreferHuman(6);
    const PlayerType Neutral(7);
    const PlayerType Closed(8);
    const PlayerType PlayerLeft(10);
    const PlayerType ComputerLeft(11);
    const PlayerType Unknown(12);
    void init()
    {
      playerTypeName[None.getID()] = "None";
      playerTypeName[Computer.getID()] = "Computer";
      playerTypeName[Player.getID()] = "Player";
      playerTypeName[RescuePassive.getID()] = "RescuePassive";
      playerTypeName[EitherPreferComputer.getID()] = "EitherPreferComputer";
      playerTypeName[EitherPreferHuman.getID()] = "EitherPreferHuman";
      playerTypeName[Neutral.getID()] = "Neutral";
      playerTypeName[Closed.getID()] = "Closed";
      playerTypeName[PlayerLeft.getID()] = "PlayerLeft";
      playerTypeName[ComputerLeft.getID()] = "ComputerLeft";
      playerTypeName[Unknown.getID()] = "Unknown";
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
      for(std::set<PlayerType>::iterator i = playerTypeSet.begin(); i != playerTypeSet.end(); i++)
      {
        std::string name=(*i).getName();
        for(int j=0;j<(int)name.length();j++)
        {
          if (name[j]==' ') name[j]='_';
          if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
        }
        playerTypeMap.insert(std::make_pair(name, *i));
      }
      initializingPlayerType = false;
    }
  }
  PlayerType::PlayerType()
  {
    this->id = PlayerTypes::None.id;
  }
  PlayerType::PlayerType(int id)
  {
    this->id = id;
    if (!initializingPlayerType)
    {
      if (id < 0 || id >= 13 || playerTypeName[id].length() == 0)
      {
        this->id = PlayerTypes::Unknown.id;
      }
    }
  }
  PlayerType::PlayerType(const PlayerType& other)
  {
    this->id = other.id;
  }
  PlayerType& PlayerType::operator=(const PlayerType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool PlayerType::operator==(const PlayerType& other) const
  {
    return this->id == other.id;
  }
  bool PlayerType::operator!=(const PlayerType& other) const
  {
    return this->id != other.id;
  }
  bool PlayerType::operator<(const PlayerType& other) const
  {
    return this->id < other.id;
  }
  int PlayerType::getID() const
  {
    return this->id;
  }
  std::string PlayerType::getName() const
  {
    return playerTypeName[this->id];
  }
  PlayerType PlayerTypes::getPlayerType(std::string& name)
  {
    for(int j=0;j<(int)name.length();j++)
    {
      if (name[j]==' ') name[j]='_';
      if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
    }
    std::map<std::string, PlayerType>::iterator i = playerTypeMap.find(name);
    if (i == playerTypeMap.end()) return PlayerTypes::Unknown;
    return (*i).second;
  }
  std::set<PlayerType>& PlayerTypes::allPlayerTypes()
  {
    return playerTypeSet;
  }
}
