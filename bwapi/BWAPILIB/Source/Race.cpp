#include <string>
#include <map>
#include <set>
#include <BWAPI/Race.h>

namespace BWAPI
{
  bool initializingRace=true;
  std::string raceName[7];
  std::map<std::string, Race> raceMap;
  std::set< Race > raceSet;
  namespace Races
  {
    const Race Zerg(0);
    const Race Terran(1);
    const Race Protoss(2);
    const Race Random(3);
    const Race Other(4);
    const Race None(5);
    const Race Unknown(6);
    void init()
    {
      raceName[Zerg.getID()]="Zerg";
      raceName[Terran.getID()]="Terran";
      raceName[Protoss.getID()]="Protoss";
      raceName[Random.getID()]="Random";
      raceName[Other.getID()]="Other";
      raceName[None.getID()]="None";
      raceName[Unknown.getID()]="Unknown";

      raceSet.insert(Zerg);
      raceSet.insert(Terran);
      raceSet.insert(Protoss);
      raceSet.insert(Other);
      raceSet.insert(None);
      raceSet.insert(Unknown);

      for(std::set<Race>::iterator i=raceSet.begin();i!=raceSet.end();i++)
      {
        raceMap.insert(std::make_pair((*i).getName(),*i));
      }
      initializingRace=false;
    }
  }
  Race::Race()
  {
    this->id=Races::None.id;
  }
  Race::Race(int id)
  {
    this->id=id;
    if (!initializingRace)
    {
      if (id<0 || id>=7)
      {
        this->id=Races::Unknown.id;
      }
    }
  }
  Race::Race(const Race &other)
  {
    this->id=other.id;
  }
  Race& Race::operator=(const Race &other)
  {
    this->id=other.id;
    return *this;
  }
  bool Race::operator==(const Race &other) const
  {
    return this->id==other.id;
  }
  bool Race::operator!=(const Race &other) const
  {
    return this->id!=other.id;
  }
  bool Race::operator<(const Race &other) const
  {
    return this->id<other.id;
  }
  int Race::getID() const
  {
    return this->id;
  }
  std::string Race::getName() const
  {
    return raceName[this->id];
  }
  Race Races::getRace(std::string &name)
  {
    std::map<std::string, Race>::iterator i=raceMap.find(name);
    if (i==raceMap.end()) return Races::Unknown;
    return (*i).second;
  }
  std::set<Race>& Races::allRaces()
  {
    return raceSet;
  }
}
