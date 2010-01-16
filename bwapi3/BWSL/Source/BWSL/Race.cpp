#include <string>
#include <map>
#include <set>
#include "Race.h"
#include "UnitType.h"

namespace BWSL
{
  bool initializingRace = true;
  class RaceInternal
  {
    public:
      void set(const char* name, const UnitType* worker, const UnitType* center, const UnitType* refinery, const UnitType* transport, const UnitType* supplyProvider)
      {
        if (initializingRace)
        {
          this->name = name;
          this->worker = worker;
          this->center = center;
          this->refinery = refinery;
          this->transport = transport;
          this->supplyProvider = supplyProvider;
        }
      }
      std::string name;
      const UnitType* worker;
      const UnitType* center;
      const UnitType* refinery;
      const UnitType* transport;
      const UnitType* supplyProvider;
  };
  RaceInternal raceData[7];
  std::map<std::string, Race> raceMap;
  std::set< Race > raceSet;
  namespace Races
  {
    BWSL_EXPORT const Race Zerg(0);
    BWSL_EXPORT const Race Terran(1);
    BWSL_EXPORT const Race Protoss(2);
    BWSL_EXPORT const Race Random(3);
    BWSL_EXPORT const Race Other(4);
    BWSL_EXPORT const Race None(5);
    BWSL_EXPORT const Race Unknown(6);
    void init()
    {
      raceData[Zerg.getID()].set("Zerg", &(UnitTypes::Zerg_Drone), &(UnitTypes::Zerg_Hatchery), &(UnitTypes::Zerg_Extractor), &(UnitTypes::Zerg_Overlord), &(UnitTypes::Zerg_Overlord));
      raceData[Terran.getID()].set("Terran", &(UnitTypes::Terran_SCV), &(UnitTypes::Terran_Command_Center), &(UnitTypes::Terran_Refinery), &(UnitTypes::Terran_Dropship), &(UnitTypes::Terran_Supply_Depot));
      raceData[Protoss.getID()].set("Protoss", &(UnitTypes::Protoss_Probe), &(UnitTypes::Protoss_Nexus), &(UnitTypes::Protoss_Assimilator), &(UnitTypes::Protoss_Shuttle), &(UnitTypes::Protoss_Pylon));
      raceData[Random.getID()].set("Random", &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown));
      raceData[Other.getID()].set("Other", &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown));
      raceData[None.getID()].set("None", &(UnitTypes::None), &(UnitTypes::None), &(UnitTypes::None), &(UnitTypes::None), &(UnitTypes::None));
      raceData[Unknown.getID()].set("Unknown", &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown), &(UnitTypes::Unknown));

      raceSet.insert(Zerg);
      raceSet.insert(Terran);
      raceSet.insert(Protoss);
      raceSet.insert(Other);
      raceSet.insert(None);
      raceSet.insert(Unknown);

      for(std::set<Race>::iterator i = raceSet.begin(); i != raceSet.end(); i++)
      {
        raceMap.insert(std::make_pair((*i).getName(), *i));
      }
      initializingRace = false;
    }
  }
  Race::Race()
  {
    this->id = Races::None.id;
  }
  Race::Race(int id)
  {
    this->id = id;
    if (!initializingRace)
    {
      if (id < 0 || id >= 7)
      {
        this->id = Races::Unknown.id;
      }
    }
  }
  Race::Race(const Race& other)
  {
    this->id = other.id;
  }
  Race& Race::operator=(const Race& other)
  {
    this->id = other.id;
    return *this;
  }
  bool Race::operator==(const Race& other) const
  {
    return this->id == other.id;
  }
  bool Race::operator!=(const Race& other) const
  {
    return this->id != other.id;
  }
  bool Race::operator<(const Race& other) const
  {
    return this->id < other.id;
  }
  int Race::getID() const
  {
    return this->id;
  }
  std::string Race::getName() const
  {
    return raceData[this->id].name;
  }

  const UnitType* Race::getWorker() const
  {
    return raceData[this->id].worker;
  }
  const UnitType* Race::getCenter() const
  {
    return raceData[this->id].center;
  }
  const UnitType* Race::getRefinery() const
  {
    return raceData[this->id].refinery;
  }
  const UnitType* Race::getTransport() const
  {
    return raceData[this->id].transport;
  }
  const UnitType* Race::getSupplyProvider() const
  {
    return raceData[this->id].supplyProvider;
  }
  Race Races::getRace(std::string& name)
  {
    std::map<std::string, Race>::iterator i = raceMap.find(name);
    if (i == raceMap.end()) return Races::Unknown;
    return (*i).second;
  }
  std::set<Race>& Races::allRaces()
  {
    return raceSet;
  }
}
