#include "ForceImpl.h"
#include "GameImpl.h"

namespace BWAPI4
{
  ForceImpl::ForceImpl(std::string const & _name)
    : name(_name)
  {
  }
  void ForceImpl::setID(int newID)
  {
    id = newID;
  }
  std::string ForceImpl::getName() const
  {
    return this->name;
  }
  std::set<Player> ForceImpl::getPlayers() const
  {
    return this->players;
  }
  int ForceImpl::getID() const
  {
    return id;
  }

};
