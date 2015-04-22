#include <BWAPI.h>
#include <BWAPI/Client/Client.h>
#include <BWAPI/Client/GameImpl.h>
#include <BWAPI/Client/ForceImpl.h>
#include <BWAPI/Client/PlayerImpl.h>
#include <BWAPI/Client/UnitImpl.h>
namespace BWAPI
{
  ForceImpl::ForceImpl(int _id)
    : self( &(BWAPI::BWAPIClient.data->forces[_id]) )
    , id(_id)
  {
  }
  std::string ForceImpl::getName() const
  {
    return std::string(this->self->name);
  }
  Playerset ForceImpl::getPlayers() const
  {
    Playerset players;
    for (int i = 0; i < std::extent<decltype(BWAPI::BWAPIClient.data->players)>::value; ++i)
      if ( BWAPI::BWAPIClient.data->players[i].force == id )
        players.insert( Broodwar->getPlayer(i) );
    return players;
  }
}
