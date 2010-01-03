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
  private:
    int id;
  public :
    Force(int id);
    BWAPI2_EXPORT std::string getName() const;
    BWAPI2_EXPORT std::set<Player*> getPlayers() const;
  };
}
