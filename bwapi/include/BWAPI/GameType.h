#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class GameType
  {
    public:
      GameType();
      GameType(int id);
      GameType(const GameType& other);
      GameType& operator=(const GameType& other);
      operator int() const;

      /** Returns the unique ID for this game type. */
      int getID() const;

      /** Returns the name of the game type. For example GameTypes::Melee.getName() will return an
       * std::string object containing "Melee". */
      std::string getName() const;
    private:
      int id;
  };
  namespace GameTypes
  {
    /** Given the name of a game type, this function will return the GameType. For example:
     *  GameTypes::getGameType("Free For All") will return GameTypes::Free_For_All. */
    GameType getGameType(std::string name);

    /** Returns the set of all the GameTypes. */
    std::set<GameType>& allGameTypes();
    void init();
    extern const GameType Melee;
    extern const GameType Free_For_All;
    extern const GameType One_on_One;
    extern const GameType Capture_The_Flag;
    extern const GameType Greed;
    extern const GameType Slaughter;
    extern const GameType Sudden_Death;
    extern const GameType Ladder;
    extern const GameType Use_Map_Settings;
    extern const GameType Team_Melee;
    extern const GameType Team_Free_For_All;
    extern const GameType Team_Capture_The_Flag;
    extern const GameType Top_vs_Bottom;
    extern const GameType Pro_Gamer_League;
    extern const GameType None;
    extern const GameType Unknown;
  }
}