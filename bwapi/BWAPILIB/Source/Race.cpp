#include <string>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  // NAMES
  template <>
  const std::string Type<Race, Races::Enum::Unknown>::typeNames[Races::Enum::MAX] =
  {
    "Zerg", "Terran", "Protoss",
    "Other", "Unused", "Select",
    "Random", "None", "Unknown"
  };
  
  // (local scope)
  namespace RaceInternal
  {
    using namespace UnitTypes::Enum;
  
    // LOCALIZATION
    std::string raceLocalNames[Races::Enum::MAX];

    // WORKER TYPES
    static const int workerTypes[Races::Enum::MAX] =
    {
      Zerg_Drone, Terran_SCV, Protoss_Probe, 
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // BASE TYPES
    static const int baseTypes[Races::Enum::MAX] =
    {
      Zerg_Hatchery, Terran_Command_Center, Protoss_Nexus,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // REFINERY TYPES
    static const int refineryTypes[Races::Enum::MAX] =
    {
      Zerg_Extractor, Terran_Refinery, Protoss_Assimilator,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // TRANSPORT TYPES
    static const int transportTypes[Races::Enum::MAX] =
    {
      Zerg_Overlord, Terran_Dropship, Protoss_Shuttle,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };

    // SUPPLY TYPES
    static const int supplyTypes[Races::Enum::MAX] =
    {
      Zerg_Overlord, Terran_Supply_Depot, Protoss_Pylon,
      None, None, None, // unused
      Unknown, None, Unknown // random, none, unk
    };
  };// end local scope

  namespace RaceSet
  {
    using namespace Races::Enum;
    const Race::set raceSet = { Zerg, Terran, Protoss, None, Unknown };
  }
  namespace Races
  {
    BWAPI_TYPEDEF(Race,Zerg);
    BWAPI_TYPEDEF(Race,Terran);
    BWAPI_TYPEDEF(Race,Protoss);
    BWAPI_TYPEDEF(Race,Random);
    BWAPI_TYPEDEF(Race,None);
    BWAPI_TYPEDEF(Race,Unknown);
  }
  Race::Race(int id) : Type(id)
  {}
  UnitType Race::getWorker() const
  {
    return RaceInternal::workerTypes[this->getID()];
  }
  UnitType Race::getCenter() const
  {
    return RaceInternal::baseTypes[this->getID()];
  }
  UnitType Race::getRefinery() const
  {
    return RaceInternal::refineryTypes[this->getID()];
  }
  UnitType Race::getTransport() const
  {
    return RaceInternal::transportTypes[this->getID()];
  }
  UnitType Race::getSupplyProvider() const
  {
    return RaceInternal::supplyTypes[this->getID()];
  }
  const Race::set& Races::allRaces()
  {
    return RaceSet::raceSet;
  }
}
