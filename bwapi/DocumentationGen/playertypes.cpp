#include "helpers.h"

void writePlayerTypeInfo()
{
  std::ofstream of("playertypes.dox");
  for (auto t : PlayerTypes::allPlayerTypes())
  {
    if (t == Races::Unknown || t == Races::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << "<table border='0'>";

    std::set<std::string> locations;
    if (t.isGameType()) locations.insert("Game");
    if (t.isLobbyType()) locations.insert("Lobby");
    of << row("Game Locations", makelist(locations));

    of << "</table>\n";

    of << docEnd();
  }
}
