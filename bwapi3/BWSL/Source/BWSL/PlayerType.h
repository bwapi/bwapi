#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
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
    BWSL_EXPORT extern const PlayerType NotUsed;
    BWSL_EXPORT extern const PlayerType Computer;
    BWSL_EXPORT extern const PlayerType Human;
    BWSL_EXPORT extern const PlayerType Rescuable;
    BWSL_EXPORT extern const PlayerType ComputerSlot;
    BWSL_EXPORT extern const PlayerType OpenSlot;
    BWSL_EXPORT extern const PlayerType Neutral;
    BWSL_EXPORT extern const PlayerType ClosedSlot;
    BWSL_EXPORT extern const PlayerType HumanDefeated;
    BWSL_EXPORT extern const PlayerType ComputerDefeated;
    BWSL_EXPORT extern const PlayerType None;
    BWSL_EXPORT extern const PlayerType Unknown;
  }
}
