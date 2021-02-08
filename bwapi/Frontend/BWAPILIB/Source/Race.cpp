#include <string>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>

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
  } // end local scope

  namespace RaceSet
  {
    using namespace Races;
    const Race::set raceSet = { Zerg, Terran, Protoss, None, Unknown };
  }
  UnitType Race::getWorker() const
  {
    return UnitType{ RaceInternal::workerTypes[this->getID()] };
  }
  UnitType Race::getResourceDepot() const
  {
    return UnitType{ RaceInternal::baseTypes[this->getID()] };
  }
  UnitType Race::getCenter() const
  {
    return getResourceDepot();
  }
  UnitType Race::getRefinery() const
  {
    return UnitType{ RaceInternal::refineryTypes[this->getID()] };
  }
  UnitType Race::getTransport() const
  {
    return UnitType{ RaceInternal::transportTypes[this->getID()] };
  }
  UnitType Race::getSupplyProvider() const
  {
    return UnitType{ RaceInternal::supplyTypes[this->getID()] };
  }
  const Race::set& Races::allRaces()
  {
    return RaceSet::raceSet;
  }
}
