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
    this->self=&(BWAPI::Client::gameData->forces[id]);
    for(int i=0;i<12;i++)
    {
      if (BWAPI::Client::gameData->players[i].force==id)
      {
        this->players.insert(BWAPI::BroodwarImpl.getPlayer(i));
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