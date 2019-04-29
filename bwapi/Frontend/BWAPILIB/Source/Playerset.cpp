#include <BWAPI/Playerset.h>

#include <BWAPI/Unitset.h>
#include <BWAPI/Player.h>
#include <BWAPI/Game.h>

namespace BWAPI
{
  Unitset Playerset::getUnits() const
  {
    Unitset retSet;
    for (auto &p : *this)
    {
      for (auto &u : p->getUnits())
      {
        retSet.emplace(u);
      }
    }
    return retSet;
  }

  Race::set Playerset::getRaces() const
  {
    Race::set retSet;
    for (auto &p : *this)
    {
      retSet.insert(p->getRace());
    }
    return retSet;
  }

  void Playerset::setAlliance(bool allies, bool alliedVictory)
  {
    for (auto &p : *this)
    {
      p.getGame().setAlliance(p, allies, alliedVictory);
    }
  }
}

