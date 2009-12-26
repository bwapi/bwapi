#pragma once
/**
 *  the race data table
 */

#include <BWAPITypes\Race.h>
#include <BWAPITypes\RaceId.h>

#include <string>

namespace BWAPI
{
  namespace Races
  {
    extern RaceId &getIdByName(std::string& name);
    extern Race &getRace(RaceId raceId);
    extern void init();
  }
}
