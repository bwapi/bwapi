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

      /** Returns the unique ID for this player type. */
      int getID() const;

      /** Returns the name of the player type. For example PlayerTypes::Computer.getName() will return an
       * std::string object containing "Computer". */
      std::string getName() const;
    private:
      int id;
  };
  namespace PlayerTypes
  {
    /** Given the name of a player type, this function will return the playertype. For example:
     *  PlayerTypes::getPlayerType("Human") will return PlayerTypes::Human. */
    PlayerType getPlayerType(std::string name);

    /** Returns the set of all the PlayerTypes. */
    std::set<PlayerType>& allPlayerTypes();
    void init();
    extern const PlayerType None;
    extern const PlayerType Computer;
    extern const PlayerType Player;
    extern const PlayerType RescuePassive;
    extern const PlayerType EitherPreferComputer;
    extern const PlayerType EitherPreferHuman;
    extern const PlayerType Neutral;
    extern const PlayerType Closed;
    extern const PlayerType PlayerLeft;
    extern const PlayerType ComputerLeft;
    extern const PlayerType Unknown;
  }
}
