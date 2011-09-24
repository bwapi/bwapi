#include <BWAPI.h>
#include "Client.h"
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
namespace BWAPI
{
  ForceImpl::ForceImpl(int _id)
    : self( &(BWAPI::BWAPIClient.data->forces[id]) )
    , id(_id)
  {
  }
  std::string ForceImpl::getName() const
  {
    return std::string(this->self->name);
  }
  std::set<Player*> ForceImpl::getPlayers() const
  {
    std::set<Player*> players;
    for( int i = 0; i < 12; ++i )
      if ( BWAPI::BWAPIClient.data->players[i].force == id )
        players.insert( ((GameImpl*)Broodwar)->getPlayer(i) );
    return players;
  }
}