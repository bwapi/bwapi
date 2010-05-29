#include <string>
#include <map>
#include <set>
#include <BWAPI/GameType.h>

namespace BWAPI
{
  bool initializingGameType = true;
  std::string gameTypeName[14];
  std::map<std::string, GameType> gameTypeMap;
  std::set< GameType > gameTypeSet;
  namespace GameTypes
  {
    const GameType Top_vs_Bottom(0);
    const GameType Melee(1);
    const GameType Free_For_All(2);
    const GameType One_on_One(3);
    const GameType Use_Map_Settings(4);
    const GameType Capture_The_Flag(5);
    const GameType Greed(6);
    const GameType Slaughter(7);
    const GameType Sudden_Death(8);
    const GameType Team_Melee(9);
    const GameType Team_Free_For_All(10);
    const GameType Team_Capture_The_Flag(11);
    const GameType None(12);
    const GameType Unknown(13);
    void init()
    {
      gameTypeName[Top_vs_Bottom.getID()] = "Top vs Bottom";
      gameTypeName[Melee.getID()] = "Melee";
      gameTypeName[Free_For_All.getID()] = "Free For All";
      gameTypeName[One_on_One.getID()] = "One on One";
      gameTypeName[Use_Map_Settings.getID()] = "Use Map Settings";
      gameTypeName[Capture_The_Flag.getID()] = "Capture The Flag";
      gameTypeName[Greed.getID()] = "Greed";
      gameTypeName[Slaughter.getID()] = "Slaughter";
      gameTypeName[Sudden_Death.getID()] = "Sudden Death";
      gameTypeName[Team_Melee.getID()] = "Team Melee";
      gameTypeName[Team_Free_For_All.getID()] = "Team Free For All";
      gameTypeName[Team_Capture_The_Flag.getID()] = "Team Capture The Flag";
      gameTypeName[None.getID()] = "None";
      gameTypeName[Unknown.getID()] = "Unknown";
      gameTypeSet.insert(Top_vs_Bottom);
      gameTypeSet.insert(Melee);
      gameTypeSet.insert(Free_For_All);
      gameTypeSet.insert(One_on_One);
      gameTypeSet.insert(Use_Map_Settings);
      gameTypeSet.insert(Capture_The_Flag);
      gameTypeSet.insert(Greed);
      gameTypeSet.insert(Slaughter);
      gameTypeSet.insert(Sudden_Death);
      gameTypeSet.insert(Team_Melee);
      gameTypeSet.insert(Team_Free_For_All);
      gameTypeSet.insert(Team_Capture_The_Flag);
      gameTypeSet.insert(None);
      gameTypeSet.insert(Unknown);
      for(std::set<GameType>::iterator i = gameTypeSet.begin(); i != gameTypeSet.end(); i++)
      {
        std::string name=(*i).getName();
        for(int j=0;j<(int)name.length();j++)
        {
          if (name[j]==' ') name[j]='_';
          if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
        }
        gameTypeMap.insert(std::make_pair(name, *i));
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
    if (!initializingGameType)
    {
      if (id < 0 || id >= 14 || gameTypeName[id].length() == 0)
      {
        this->id = GameTypes::Unknown.id;
      }
    }
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
  GameType GameTypes::getGameType(std::string& name)
  {
    for(int j=0;j<(int)name.length();j++)
    {
      if (name[j]==' ') name[j]='_';
      if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
    }
    std::map<std::string, GameType>::iterator i = gameTypeMap.find(name);
    if (i == gameTypeMap.end()) return GameTypes::Unknown;
    return (*i).second;
  }
  std::set<GameType>& GameTypes::allGameTypes()
  {
    return gameTypeSet;
  }
}
