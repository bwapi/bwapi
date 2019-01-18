#include <BWAPI.h>

namespace BWAPI
{
  Playerset Force::getPlayers() const
  {
    if (getData().players.empty()) return { };
    Playerset ret;
    for (auto &id : getData().players)
      ret.emplace(*getGame().getPlayerData(id));
    return ret;
  }
}

