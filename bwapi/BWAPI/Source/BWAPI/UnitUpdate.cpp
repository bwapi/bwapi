#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Logger.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>
#include "Server.h"
namespace BWAPI
{
  void UnitImpl::updateUnitData()
  {
    id = (int)this;
    int selfPlayerID = BroodwarImpl.server.getPlayerID(Broodwar->self());
    if (_exists)
    {
      for(int i=0;i<9;i++)
      {
        Player* player = BroodwarImpl.server.getPlayer(i);
        if (i==selfPlayerID) continue;
        if (getRawDataLocal->sprite == NULL)
          self->isVisible[i]=false;
             //this function is only available when Broodwar is in a replay or the complete map information flag is enabled.
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i]=false;
        else if (_getPlayer() == player)
          self->isVisible[i]=true;
        else if (player == NULL)
          self->isVisible[i]=false;
        else if (player->isNeutral())
          self->isVisible[i]=getRawDataLocal->sprite->visibilityFlags > 0;
        else
          self->isVisible[i]=(getRawDataLocal->sprite->visibilityFlags & (1 << i)) != 0;
      }

      if (getRawDataLocal->sprite == NULL)
        self->isVisible[selfPlayerID] = false;
      else if (BroodwarImpl._isReplay())
        self->isVisible[selfPlayerID] = getRawDataLocal->sprite->visibilityFlags > 0;
      else if (_getPlayer() == BWAPI::BroodwarImpl.self())
        self->isVisible[selfPlayerID] = true;
      else if (makeVisible)
        self->isVisible[selfPlayerID] = true;
      else
        self->isVisible[selfPlayerID] = (getRawDataLocal->sprite->visibilityFlags & (1 << Broodwar->self()->getID())) != 0;
    }
    else
    {
      for(int i=0;i<9;i++)
        self->isVisible[i] = false;
    }
  }
}