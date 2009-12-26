#pragma once
/**
 *  the race data table
 */

#include <BWAPITypes\Race.h>
#include <BWAPITypes\RaceId.h>

#include <string>
#include <map>
#include <set>

namespace BWAPI
{
  namespace Races
  {
    extern Race raceData[RaceIds::count];
    extern std::map<std::string, RaceId> raceMap;
    extern std::set< RaceId > raceSet;

    extern RaceId &getIdByName(std::string& name);
    extern void init();
  }
}
