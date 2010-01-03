#pragma once
#include "BWAPI2Interface.h"

IP_STRIP

#include <set>
#include <string>

namespace BWAPI2
{
  class Player;
  class Force
  {
  public :
    BWAPI2_EXPORT std::string getName() const;
    BWAPI2_EXPORT std::set<Player*> getPlayers() const;
  };
}
