#include "PlayerTypes.h"

namespace BWAPI
{
  namespace PlayerTypes
  {
    PlayerType playerTypeData[PlayerTypeIds::count];
    std::map<std::string, PlayerTypeId> playerTypeMap;
    std::set< PlayerTypeId > playerTypeSet;

    void fillPlayerType(PlayerTypeId id, const char* name)
    {
      PlayerType &target = playerTypeData[id];
      target.name = name;
    }

    void init()
    {
      fillPlayerType(PlayerTypeIds::NotUsed, "NotUsed");
      fillPlayerType(PlayerTypeIds::Computer, "Computer");
      fillPlayerType(PlayerTypeIds::Human, "Human");
      fillPlayerType(PlayerTypeIds::Rescuable, "Rescuable");
      fillPlayerType(PlayerTypeIds::ComputerSlot, "ComputerSlot");
      fillPlayerType(PlayerTypeIds::OpenSlot, "OpenSlot");
      fillPlayerType(PlayerTypeIds::Neutral, "Neutral");
      fillPlayerType(PlayerTypeIds::ClosedSlot, "ClosedSlot");
      fillPlayerType(PlayerTypeIds::HumanDefeated, "HumanDefeated");
      fillPlayerType(PlayerTypeIds::ComputerDefeated, "ComputerDefeated");
      fillPlayerType(PlayerTypeIds::None, "None");
      playerTypeSet.insert(PlayerTypeIds::NotUsed);
      playerTypeSet.insert(PlayerTypeIds::Computer);
      playerTypeSet.insert(PlayerTypeIds::Human);
      playerTypeSet.insert(PlayerTypeIds::Rescuable);
      playerTypeSet.insert(PlayerTypeIds::ComputerSlot);
      playerTypeSet.insert(PlayerTypeIds::OpenSlot);
      playerTypeSet.insert(PlayerTypeIds::Neutral);
      playerTypeSet.insert(PlayerTypeIds::ClosedSlot);
      playerTypeSet.insert(PlayerTypeIds::HumanDefeated);
      playerTypeSet.insert(PlayerTypeIds::ComputerDefeated);
      playerTypeSet.insert(PlayerTypeIds::None);
      for(std::set<PlayerTypeId>::iterator i = playerTypeSet.begin(); i != playerTypeSet.end(); i++)
      {
        playerTypeMap.insert(std::make_pair(std::string(playerTypeData[*i].name), *i));
      }
    }
    PlayerTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, PlayerTypeId>::iterator i = playerTypeMap.find(name);
      if (i == playerTypeMap.end()) return PlayerTypeIds::None;
      return (*i).second;
    }
  }
}
