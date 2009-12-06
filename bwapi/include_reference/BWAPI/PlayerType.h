#pragma once
#include <string>
#include <set>
namespace BWAPI
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
    extern const PlayerType NotUsed;
    extern const PlayerType Computer;
    extern const PlayerType Human;
    extern const PlayerType Rescuable;
    extern const PlayerType ComputerSlot;
    extern const PlayerType OpenSlot;
    extern const PlayerType Neutral;
    extern const PlayerType ClosedSlot;
    extern const PlayerType HumanDefeated;
    extern const PlayerType ComputerDefeated;
    extern const PlayerType None;
    extern const PlayerType Unknown;
  }
}
