#include <BWAPI/Playerset.h>

#include <BWAPI/Unitset.h>
#include <BWAPI/Player.h>

#include <BWAPI/Filters.h>
#include <BWAPI/Game.h>

#include <utility>

namespace BWAPI
{
  Unitset Playerset::getUnits() const
  {
    Unitset retSet;
    for (auto &p : *this)
    {
      auto units = p->getUnits();
      retSet.insert(units.begin(), units.end());
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
      Broodwar->setAlliance(p, allies, alliedVictory);
    }
  }
}

