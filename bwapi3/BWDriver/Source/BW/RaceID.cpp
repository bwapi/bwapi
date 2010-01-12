#include "RaceID.h"
namespace BW
{
  namespace RaceIDs
  {
    RaceID fromString(const std::string& name)
    {
      if (name == "Zerg")
        return Zerg;
      if (name == "Terran")
        return Terran;
      if (name == "Protoss")
        return Protoss;
      return None;
    };
    std::string getName(RaceID race)
    {
      switch (race)
      {
        case RaceIDs::Zerg    : return "Zerg";
        case RaceIDs::Terran  : return "Terran";
        case RaceIDs::Protoss : return "Protoss";
        case RaceIDs::Other   : return "Other";
        case RaceIDs::Select  : return "Select";
        case RaceIDs::Random  : return "Random";
        case RaceIDs::None    : return "None";
        default               : return "Unknown";
      }
    };
  }
};
