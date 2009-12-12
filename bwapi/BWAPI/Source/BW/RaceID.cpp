#include "RaceID.h"
namespace BW
{
  namespace RaceIDs
  {
    u8 fromString(const std::string& name)
    {
      if (name == "Zerg")
        return Zerg;
      if (name == "Terran")
        return Terran;
      if (name == "Protoss")
        return Protoss;
      return None;
    };
    std::string getName(u8 race)
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
