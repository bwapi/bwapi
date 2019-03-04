#include <string>
#include <BWAPI/GameType.h>

namespace BWAPI
{
  template <>
  const std::string Type<GameType, GameTypes::Enum::Unknown>::typeNames[GameTypes::Enum::MAX] =
  {
    "None",
    "Custom",
    "Melee",
    "Free_For_All",
    "One_on_One",
    "Capture_The_Flag",
    "Greed",
    "Slaughter",
    "Sudden_Death",
    "Ladder",
    "Use_Map_Settings",
    "Team_Melee",
    "Team_Free_For_All",
    "Team_Capture_The_Flag",
    "",
    "Top_vs_Bottom",
    "Iron_Man_Ladder",  // 16
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
    "Unknown"
  };
  namespace GameTypeSet
  {
    using namespace GameTypes;
    const GameType::set gameTypeSet = { Melee, Free_For_All, One_on_One, Capture_The_Flag, Greed, Slaughter, Sudden_Death,
      Ladder, Use_Map_Settings, Team_Melee, Team_Free_For_All, Team_Capture_The_Flag,
      Top_vs_Bottom, None, Unknown };
  }
  const GameType::set& GameTypes::allGameTypes()
  {
    return GameTypeSet::gameTypeSet;
  }
}
