#include "Races.h"

#include <BWAPITypes\UnitTypeId.h>

namespace BWAPI
{
  namespace Races
  {
    void fillRace(Race &race, const char* name, UnitTypeId worker, UnitTypeId center, UnitTypeId refinery, UnitTypeId transport, UnitTypeId supplyProvider)
    {
      race.name = name;
      race.worker = worker;
      race.center = center;
      race.refinery = refinery;
      race.transport = transport;
      race.supplyProvider = supplyProvider;
    }

    Race raceData[RaceIds::count];
    std::map<std::string, RaceId> raceMap;
    std::set< RaceId > raceSet;

    void init()
    {
      fillRace(raceData[RaceIds::Zerg], "Zerg", UnitTypeIds::Zerg_Drone, UnitTypeIds::Zerg_Hatchery, UnitTypeIds::Zerg_Extractor, UnitTypeIds::Zerg_Overlord, UnitTypeIds::Zerg_Overlord);
      fillRace(raceData[RaceIds::Terran], "Terran", UnitTypeIds::Terran_SCV, UnitTypeIds::Terran_Command_Center, UnitTypeIds::Terran_Refinery, UnitTypeIds::Terran_Dropship, UnitTypeIds::Terran_Supply_Depot);
      fillRace(raceData[RaceIds::Protoss], "Protoss", UnitTypeIds::Protoss_Probe, UnitTypeIds::Protoss_Nexus, UnitTypeIds::Protoss_Assimilator, UnitTypeIds::Protoss_Shuttle, UnitTypeIds::Protoss_Pylon);

      raceSet.clear();
      raceSet.insert(RaceIds::Zerg);
      raceSet.insert(RaceIds::Terran);
      raceSet.insert(RaceIds::Protoss);

      raceMap.clear();
      for(std::set<RaceId>::iterator i = raceSet.begin(); i != raceSet.end(); i++)
      {
        raceMap.insert(std::make_pair(std::string(raceData[*i].name), *i));
      }
    }
  }
}
