#include <string>
#include <map>
#include <set>
#include "PlayerType.h"

namespace BWSL
{
  bool initializingPlayerType = true;
  std::string playerTypeName[14];
  std::map<std::string, PlayerType> playerTypeMap;
  std::set< PlayerType > playerTypeSet;
  namespace PlayerTypes
  {
    const PlayerType NotUsed(0);
    const PlayerType Computer(1);
    const PlayerType Human(2);
    const PlayerType Rescuable(3);
    const PlayerType ComputerSlot(5);
    const PlayerType OpenSlot(6);
    const PlayerType Neutral(7);
    const PlayerType ClosedSlot(8);
    const PlayerType HumanDefeated(10);
    const PlayerType ComputerDefeated(11);
    const PlayerType None(12);
    const PlayerType Unknown(13);
    void init()
    {
      playerTypeName[NotUsed.getID()] = "NotUsed";
      playerTypeName[Computer.getID()] = "Computer";
      playerTypeName[Human.getID()] = "Human";
      playerTypeName[Rescuable.getID()] = "Rescuable";
      playerTypeName[ComputerSlot.getID()] = "ComputerSlot";
      playerTypeName[OpenSlot.getID()] = "OpenSlot";
      playerTypeName[Neutral.getID()] = "Neutral";
      playerTypeName[ClosedSlot.getID()] = "ClosedSlot";
      playerTypeName[HumanDefeated.getID()] = "HumanDefeated";
      playerTypeName[ComputerDefeated.getID()] = "ComputerDefeated";
      playerTypeName[None.getID()] = "None";
      playerTypeName[Unknown.getID()] = "Unknown";
      playerTypeSet.insert(NotUsed);
      playerTypeSet.insert(Computer);
      playerTypeSet.insert(Human);
      playerTypeSet.insert(Rescuable);
      playerTypeSet.insert(ComputerSlot);
      playerTypeSet.insert(OpenSlot);
      playerTypeSet.insert(Neutral);
      playerTypeSet.insert(ClosedSlot);
      playerTypeSet.insert(HumanDefeated);
      playerTypeSet.insert(ComputerDefeated);
      playerTypeSet.insert(None);
      playerTypeSet.insert(Unknown);
      for(std::set<PlayerType>::iterator i = playerTypeSet.begin(); i != playerTypeSet.end(); i++)
      {
        playerTypeMap.insert(std::make_pair((*i).getName(), *i));
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
      if (id < 0 || id >= 14 || playerTypeName[id].length() == 0)
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
    std::map<std::string, PlayerType>::iterator i = playerTypeMap.find(name);
    if (i == playerTypeMap.end()) return PlayerTypes::Unknown;
    return (*i).second;
  }
  std::set<PlayerType>& PlayerTypes::allPlayerTypes()
  {
    return playerTypeSet;
  }
}
