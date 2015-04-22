#include "ForceImpl.h"
#include "GameImpl.h"

#include "../../../Debug.h"

namespace BWAPI
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
  Playerset ForceImpl::getPlayers() const
  {
    return this->players;
  }
};
