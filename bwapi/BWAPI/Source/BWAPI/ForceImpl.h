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
      virtual int getID() const;
      virtual std::string getName() const;
      virtual std::set<Player*> getPlayers() const;
      std::set<Player*> players;
      std::string name;
      void setID(int newID);
    private:
      int id;
  };
};
