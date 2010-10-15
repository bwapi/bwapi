#include <string>
#include <map>
#include <set>
#include <BWAPI/GameType.h>
#include <Util/Foreach.h>

#include "Common.h"

namespace BWAPI
{
  bool initializingGameType = true;
  std::string gameTypeName[35];
  std::map<std::string, GameType> gameTypeMap;
  std::set< GameType > gameTypeSet;
  namespace GameTypes
  {
    const GameType Melee(2);
    const GameType Free_For_All(3);
    const GameType One_on_One(4);
    const GameType Capture_The_Flag(5);
    const GameType Greed(6);
    const GameType Slaughter(7);
    const GameType Sudden_Death(8);
    const GameType Ladder(9);
    const GameType Use_Map_Settings(10);
    const GameType Team_Melee(11);
    const GameType Team_Free_For_All(12);
    const GameType Team_Capture_The_Flag(13);
    const GameType Top_vs_Bottom(15);
    const GameType Pro_Gamer_League(32);
    const GameType None(33);
    const GameType Unknown(34);

    void init()
    {
      gameTypeName[Melee.getID()]                 = "Melee";
      gameTypeName[Free_For_All.getID()]          = "Free For All";
      gameTypeName[One_on_One.getID()]            = "One on One";
      gameTypeName[Capture_The_Flag.getID()]      = "Capture The Flag";
      gameTypeName[Greed.getID()]                 = "Greed";
      gameTypeName[Slaughter.getID()]             = "Slaughter";
      gameTypeName[Sudden_Death.getID()]          = "Sudden Death";
      gameTypeName[Ladder.getID()]                = "Ladder";
      gameTypeName[Use_Map_Settings.getID()]      = "Use Map Settings";
      gameTypeName[Team_Melee.getID()]            = "Team Melee";
      gameTypeName[Team_Free_For_All.getID()]     = "Team Free For All";
      gameTypeName[Team_Capture_The_Flag.getID()] = "Team Capture The Flag";
      gameTypeName[Top_vs_Bottom.getID()]         = "Top vs Bottom";
      gameTypeName[Pro_Gamer_League.getID()]      = "Pro Gamer League";
      gameTypeName[None.getID()]                  = "None";
      gameTypeName[Unknown.getID()]               = "Unknown";

      gameTypeSet.insert(Melee);
      gameTypeSet.insert(Free_For_All);
      gameTypeSet.insert(One_on_One);
      gameTypeSet.insert(Capture_The_Flag);
      gameTypeSet.insert(Greed);
      gameTypeSet.insert(Slaughter);
      gameTypeSet.insert(Sudden_Death);
      gameTypeSet.insert(Ladder);
      gameTypeSet.insert(Use_Map_Settings);
      gameTypeSet.insert(Team_Melee);
      gameTypeSet.insert(Team_Free_For_All);
      gameTypeSet.insert(Team_Capture_The_Flag);
      gameTypeSet.insert(Top_vs_Bottom);
      gameTypeSet.insert(Pro_Gamer_League);
      gameTypeSet.insert(None);
      gameTypeSet.insert(Unknown);

      foreach(GameType i, gameTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        gameTypeMap.insert(std::make_pair(name, i));
      }
      initializingGameType = false;
    }
  }
  GameType::GameType()
  {
    this->id = GameTypes::None.id;
  }
  GameType::GameType(int id)
  {
    this->id = id;
    if (!initializingGameType && (id < 0 || id >= 35 || gameTypeName[id].length() == 0))
      this->id = GameTypes::Unknown.id;
  }
  GameType::GameType(const GameType& other)
  {
    this->id = other.id;
  }
  GameType& GameType::operator=(const GameType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool GameType::operator==(const GameType& other) const
  {
    return this->id == other.id;
  }
  bool GameType::operator!=(const GameType& other) const
  {
    return this->id != other.id;
  }
  bool GameType::operator<(const GameType& other) const
  {
    return this->id < other.id;
  }
  int GameType::getID() const
  {
    return this->id;
  }
  std::string GameType::getName() const
  {
    return gameTypeName[this->id];
  }
  GameType GameTypes::getGameType(std::string name)
  {
    fixName(&name);
    std::map<std::string, GameType>::iterator i = gameTypeMap.find(name);
    if (i == gameTypeMap.end()) return GameTypes::Unknown;
    return (*i).second;
  }
  std::set<GameType>& GameTypes::allGameTypes()
  {
    return gameTypeSet;
  }
}
