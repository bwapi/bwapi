#include "helpers.h"

#include <set>
#include <string>

void writePlayerTypeInfo()
{
  std::ofstream of("playertypes.dox");
  for (auto t : PlayerTypes::allPlayerTypes())
  {
    if (t == PlayerTypes::Unknown || t == PlayerTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    std::set<std::string> locations;
    if (t.isGameType()) locations.insert("Game");
    if (t.isLobbyType()) locations.insert("Lobby");

    if (!locations.empty())
    {
      of << "<table>";
      of << row("Game Locations", makelist(locations));
      of << "</table>\n";
    }

    of << makekeywords(t);

    of << docEnd();
  }
}
