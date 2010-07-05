#include "ForceImpl.h"
#include "GameImpl.h"
namespace BWAPI
{
  ForceImpl::ForceImpl(std::string name)
  {
    this->name = name;
    this->id = BroodwarImpl.server.getForceID(this); 
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
