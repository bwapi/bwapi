#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"

#include <Util\Foreach.h>

#include <string>
namespace BWAPI
{
  BulletImpl::BulletImpl(int _index)
    : self( &(BWAPI::BWAPIClient.data->bullets[index]) )
    , index(_index)
  {
  }
}