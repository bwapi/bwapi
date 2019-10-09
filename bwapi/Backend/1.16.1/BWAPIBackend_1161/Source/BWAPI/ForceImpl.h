#pragma once
#include <string>
#include <set>

namespace BWAPI4
{
  // Forwards
  class PlayerImpl;
  typedef PlayerImpl *Player;

  class ForceImpl
  {
    public:
      ForceImpl(std::string const & name);
      int getID() const;
      std::string getName() const;
      std::set<Player> getPlayers() const;
      std::set<Player> players;
      std::string name;
      void setID(int newID);
    private:
      int id = -1;
  };
};
