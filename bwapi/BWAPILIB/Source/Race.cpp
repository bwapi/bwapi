#include <string>
#include <map>
#include <set>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingRace = true;
  class RaceInternal
  {
    public:
      void set(const char* name, UnitType worker, UnitType center, UnitType refinery, UnitType transport, UnitType supplyProvider)
      {
        if (initializingRace)
        {
          this->name           = name;
          this->worker         = worker;
          this->center         = center;
          this->refinery       = refinery;
          this->transport      = transport;
          this->supplyProvider = supplyProvider;
        }
      }
      std::string name;
      UnitType worker;
      UnitType center;
      UnitType refinery;
      UnitType transport;
      UnitType supplyProvider;
  };
  RaceInternal raceData[7];
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
      raceData[Zerg].set(   "Zerg",    UnitTypes::Zerg_Drone,    UnitTypes::Zerg_Hatchery,         UnitTypes::Zerg_Extractor,      UnitTypes::Zerg_Overlord,   UnitTypes::Zerg_Overlord);
      raceData[Terran].set( "Terran",  UnitTypes::Terran_SCV,    UnitTypes::Terran_Command_Center, UnitTypes::Terran_Refinery,     UnitTypes::Terran_Dropship, UnitTypes::Terran_Supply_Depot);
      raceData[Protoss].set("Protoss", UnitTypes::Protoss_Probe, UnitTypes::Protoss_Nexus,         UnitTypes::Protoss_Assimilator, UnitTypes::Protoss_Shuttle, UnitTypes::Protoss_Pylon);
      raceData[Random].set( "Random",  UnitTypes::Unknown,       UnitTypes::Unknown,               UnitTypes::Unknown,             UnitTypes::Unknown,         UnitTypes::Unknown);
      raceData[Other].set(  "Other",   UnitTypes::Unknown,       UnitTypes::Unknown,               UnitTypes::Unknown,             UnitTypes::Unknown,         UnitTypes::Unknown);
      raceData[None].set(   "None",    UnitTypes::None,          UnitTypes::None,                  UnitTypes::None,                UnitTypes::None,            UnitTypes::None);
      raceData[Unknown].set("Unknown", UnitTypes::Unknown,       UnitTypes::Unknown,               UnitTypes::Unknown,             UnitTypes::Unknown,         UnitTypes::Unknown);

      raceSet.insert(Zerg);
      raceSet.insert(Terran);
      raceSet.insert(Protoss);
      raceSet.insert(Other);
      raceSet.insert(None);
      raceSet.insert(Unknown);

      foreach(Race i, raceSet)
      {
        std::string name(i.getName());
        fixName(&name);
        raceMap.insert(std::make_pair(name, i));
      }
      initializingRace = false;
    }
  }
  Race::Race() : Type(Races::None)
  {
  }
  int getValidRaceID(int id)
  {
    if ( !initializingRace && (id < 0 || id >= 7) )
      return Races::Unknown;
    return id;
  }
  Race::Race(int id) : Type( getValidRaceID(id) )
  {
  }
  const std::string &Race::getName() const
  {
    return raceData[this->getID()].name;
  }
  const char *Race::c_str() const
  {
    return raceData[this->getID()].name.c_str();
  }
  UnitType Race::getWorker() const
  {
    return raceData[this->getID()].worker;
  }
  UnitType Race::getCenter() const
  {
    return raceData[this->getID()].center;
  }
  UnitType Race::getRefinery() const
  {
    return raceData[this->getID()].refinery;
  }
  UnitType Race::getTransport() const
  {
    return raceData[this->getID()].transport;
  }
  UnitType Race::getSupplyProvider() const
  {
    return raceData[this->getID()].supplyProvider;
  }
  Race Races::getRace(std::string name)
  {
    fixName(&name);
    std::map<std::string, Race>::iterator i = raceMap.find(name);
    if (i == raceMap.end())
      return Races::Unknown;
    return (*i).second;
  }
  const std::set<Race>& Races::allRaces()
  {
    return raceSet;
  }
}
