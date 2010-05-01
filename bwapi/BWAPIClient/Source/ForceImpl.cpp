#include <BWAPI.h>
#include "BWAPIClient.h"
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
namespace BWAPI
{
  ForceImpl::ForceImpl(int id)
  {
    this->self=&(BWAPI::BWAPIClient.data->forces[id]);
    for(int i=0;i<12;i++)
    {
      if (BWAPI::BWAPIClient.data->players[i].force==id)
      {
        this->players.insert(((GameImpl*)Broodwar)->getPlayer(i));
      }
    }
  }
  std::string ForceImpl::getName() const
  {
    return std::string(this->self->name);
  }
  std::set<Player*> ForceImpl::getPlayers() const
  {
    return this->players;
  }
}