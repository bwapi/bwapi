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
    virtual std::string getName() const;
    virtual std::set<Player*> getPlayers() const;
    std::set<Player*> players;
    std::string name;
  };
}