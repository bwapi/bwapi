#include <string>
#include <map>
#include <set>
#include <BWAPI/GameType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

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

#define SET_GAME_TYPE(x) gameTypeName[x] = underscoresToSpaces(#x); gameTypeSet.insert(x);
    void init()
    {
      SET_GAME_TYPE(Melee);
      SET_GAME_TYPE(Free_For_All);
      SET_GAME_TYPE(One_on_One);
      SET_GAME_TYPE(Capture_The_Flag);
      SET_GAME_TYPE(Greed);
      SET_GAME_TYPE(Slaughter);
      SET_GAME_TYPE(Sudden_Death);
      SET_GAME_TYPE(Ladder);
      SET_GAME_TYPE(Use_Map_Settings);
      SET_GAME_TYPE(Team_Melee);
      SET_GAME_TYPE(Team_Free_For_All);
      SET_GAME_TYPE(Team_Capture_The_Flag);
      SET_GAME_TYPE(Top_vs_Bottom);
      SET_GAME_TYPE(Pro_Gamer_League);
      SET_GAME_TYPE(None);
      SET_GAME_TYPE(Unknown);

      foreach(GameType i, gameTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        gameTypeMap.insert(std::make_pair(name, i));
      }
      initializingGameType = false;
    }
  }
  GameType::GameType()
  {
    this->id = GameTypes::None;
  }
  GameType::GameType(int id)
  {
    this->id = id;
    if (!initializingGameType && (id < 0 || id >= 35 || gameTypeName[id].length() == 0))
      this->id = GameTypes::Unknown;
  }
  GameType::GameType(const GameType& other)
  {
    this->id = other;
  }
  GameType& GameType::operator=(const GameType& other)
  {
    this->id = other;
    return *this;
  }
  GameType::operator int() const
  {
    return id;
  }
  int GameType::getID() const
  {
    return this->id;
  }
  std::string GameType::getName() const
  {
    return gameTypeName[this->id];
  }
  const char *GameType::c_str() const
  {
    return gameTypeName[this->id].c_str();
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
