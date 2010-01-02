#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class PlayerType
  {
    public:
      PlayerType();
      PlayerType(int id);
      PlayerType(const PlayerType& other);
      PlayerType& operator=(const PlayerType& other);
      bool operator==(const PlayerType& other) const;
      bool operator!=(const PlayerType& other) const;
      bool operator<(const PlayerType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace PlayerTypes
  {
    PlayerType getPlayerType(std::string& name);
    std::set<PlayerType>& allPlayerTypes();
    void init();
    BWAPI2_EXPORT extern const PlayerType NotUsed;
    BWAPI2_EXPORT extern const PlayerType Computer;
    BWAPI2_EXPORT extern const PlayerType Human;
    BWAPI2_EXPORT extern const PlayerType Rescuable;
    BWAPI2_EXPORT extern const PlayerType ComputerSlot;
    BWAPI2_EXPORT extern const PlayerType OpenSlot;
    BWAPI2_EXPORT extern const PlayerType Neutral;
    BWAPI2_EXPORT extern const PlayerType ClosedSlot;
    BWAPI2_EXPORT extern const PlayerType HumanDefeated;
    BWAPI2_EXPORT extern const PlayerType ComputerDefeated;
    BWAPI2_EXPORT extern const PlayerType None;
    BWAPI2_EXPORT extern const PlayerType Unknown;
  }
}
