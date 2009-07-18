#pragma once
#include <set>
#include <string>

namespace BWAPI
{
  class Player;
  class Force
  {
    public :
      virtual std::string getName() const = 0;
      virtual std::set<Player*> getPlayers() const = 0;
  };
}
