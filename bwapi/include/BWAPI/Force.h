#pragma once
#include <set>
#include <string>

namespace BWAPI
{
  class Player;
  class Force
  {
    public :
      virtual std::string __cdecl getName() const = 0;
      virtual std::set<Player*> __cdecl getPlayers() const = 0;
  };
}
