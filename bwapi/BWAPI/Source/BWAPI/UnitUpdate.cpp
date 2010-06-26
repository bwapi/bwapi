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
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = (Player*)BroodwarImpl.players[getRawDataLocal->playerID];


      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
      {
        Player* player = BroodwarImpl.server.getPlayer(i);
        if (i==selfPlayerID) continue;
        if (getRawDataLocal->sprite == NULL)
          self->isVisible[i]=false;
             //this function is only available when Broodwar is in a replay or the complete map information flag is enabled.
        else if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
          self->isVisible[i]=false;
        else if (_getPlayer == player)
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
      else if (_getPlayer == BWAPI::BroodwarImpl.self())
        self->isVisible[selfPlayerID] = true;
      else if (makeVisible)
        self->isVisible[selfPlayerID] = true;
      else
        self->isVisible[selfPlayerID] = (getRawDataLocal->sprite->visibilityFlags & (1 << Broodwar->self()->getID())) != 0;

      //------------------------------------------------------------------------------------------------------
      //_getType
      if ( getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch1 ||
         getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch2 ||
         getRawDataLocal->unitID.id == BW::UnitID::Resource_MineralPatch3)
      {
        _getType = UnitTypes::Resource_Mineral_Field;
      }
      else
      {
        _getType = UnitType(getRawDataLocal->unitID.id);
      }

      //------------------------------------------------------------------------------------------------------
      //_getTransport
      if (_getType == UnitTypes::Protoss_Interceptor ||
          _getType == UnitTypes::Protoss_Scarab ||
          _getType == UnitTypes::Terran_Vulture_Spider_Mine)
      {
        if (getRawDataLocal->childUnitUnion3.inHanger==0 ||
            getRawDataLocal->status.getBit(BW::StatusFlags::InTransport) ||
            getRawDataLocal->status.getBit(BW::StatusFlags::InBuilding))
          _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->childInfoUnion.parentUnit));
      }
      else if (getRawDataLocal->status.getBit(BW::StatusFlags::InTransport) ||
               getRawDataLocal->status.getBit(BW::StatusFlags::InBuilding))
        _getTransport = (Unit*)(UnitImpl::BWUnitToBWAPIUnit(getRawDataLocal->connectedUnit));
      else
        _getTransport = NULL;

      //------------------------------------------------------------------------------------------------------
      //_getPosition
      if (_getTransport!=NULL)
        _getPosition = Position(((UnitImpl*)_getTransport)->getRawDataLocal->position.x,((UnitImpl*)_getTransport)->getRawDataLocal->position.y);
      else
        _getPosition = Position(getRawDataLocal->position.x, getRawDataLocal->position.y);

    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //_getPlayer
      _getPlayer = savedPlayer;
      //------------------------------------------------------------------------------------------------------
      //isVisible
      for(int i=0;i<9;i++)
        self->isVisible[i] = false;
      //------------------------------------------------------------------------------------------------------
      //_getType
      _getType = savedUnitType;
      //------------------------------------------------------------------------------------------------------
      //_getTransport
      _getTransport = NULL;
      //------------------------------------------------------------------------------------------------------
      //_getPosition
      _getPosition = Positions::Unknown;
    }
    if (canAccess())
    {
      //------------------------------------------------------------------------------------------------------
      //getPosition
      self->positionX = _getPosition.x();
      self->positionY = _getPosition.y();
    }
    else
    {
      //------------------------------------------------------------------------------------------------------
      //getPosition
      self->positionX = BWAPI::Positions::Unknown.x();
      self->positionX = BWAPI::Positions::Unknown.y();
    }

    if (canAccessSpecial())
    {
      self->exists = _exists;
      self->player = BroodwarImpl.server.getPlayerID(_getPlayer);
      self->type   = _getType.getID();
    }
    else
    {
      self->exists = false;
      self->player = BroodwarImpl.server.getPlayerID((Player*)BroodwarImpl.players[11]);
      self->type   = UnitTypes::Unknown.getID();
    }
  }
}