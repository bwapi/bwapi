#pragma once
#include <string>
#include <BWAPI/Playerset.h>

namespace BWAPI
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
      Playerset getPlayers() const;
      Playerset players;
      std::string name;
      void setID(int newID);
    private:
      int id = -1;
  };
};
