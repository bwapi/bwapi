#include <BWAPI.h>
#include <BWAPI/Client/GameImpl.h>
#include <BWAPI/Client/BulletImpl.h>

namespace BWAPI
{
  BulletImpl::BulletImpl(int _index)
    : self( &(BWAPI::BWAPIClient.data->bullets[_index]) )
    , index(_index)
  {
  }
}