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
    return std::string("");//Todo: implement
  }
  std::set<Player*> Force::getPlayers() const
  {
    std::set<Player*> empty;
    return empty;//Todo: implement
  }
}