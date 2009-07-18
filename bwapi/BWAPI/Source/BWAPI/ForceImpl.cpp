#include "ForceImpl.h"
namespace BWAPI
{
  ForceImpl::ForceImpl(std::string name)
  {
    this->name = name;
  }
  std::string ForceImpl::getName() const
  {
    return this->name;
  }
  std::set<Player*> ForceImpl::getPlayers() const
  {
    return this->players;
  }
}