#pragma once
#include <BWAPI/Force.h>
#include <set>
#include <string>
namespace BWAPI
{
  class Player;
  class ForceImpl : public Force
  {
    public:
      ForceImpl(std::string name);
      virtual std::string __cdecl getName() const;
      virtual std::set<Player*> __cdecl getPlayers() const;
      std::set<Player*> players;
      std::string name;
  };
}