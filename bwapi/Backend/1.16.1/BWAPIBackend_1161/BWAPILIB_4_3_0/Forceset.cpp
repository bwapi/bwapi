#include <BWAPI/Forceset.h>
#include <BWAPI/Force.h>
#include <BWAPI/Playerset.h>

#include <utility>

namespace BWAPI
{
  Playerset Forceset::getPlayers() const
  {
    Playerset rset;
    for (auto &f : *this)
    {
      auto players = f->getPlayers();
      rset.insert(players.begin(), players.end());
    }
    return rset;
  }
}

