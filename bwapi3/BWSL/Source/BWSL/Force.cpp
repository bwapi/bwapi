#include "Force.h"

#include <Util\Version.h>

#include <BWAPI\all.h>
#include "Game.h"
#include <string>

namespace BWSL
{
  Force::Force(int id)
  {
    this->id=id;
  }
  std::string Force::getName() const
  {
    return BWAPI::getForce(id)->name.c_str();
  }
  std::set<Player*> Force::getPlayers() const
  {
    return players;
  }
}