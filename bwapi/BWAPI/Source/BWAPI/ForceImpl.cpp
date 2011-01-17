#include "ForceImpl.h"
#include "GameImpl.h"

#include "../../Debug.h"

namespace BWAPI
{
  ForceImpl::ForceImpl(std::string name)
  {
    this->name = name;
    this->id = -1;
  }
  void ForceImpl::setID(int newID)
  {
    id = newID;
  }
  int ForceImpl::getID() const
  {
    return id;
  }
  std::string ForceImpl::getName() const
  {
    return this->name;
  }
  std::set<Player*> ForceImpl::getPlayers() const
  {
    return this->players;
  }
};
