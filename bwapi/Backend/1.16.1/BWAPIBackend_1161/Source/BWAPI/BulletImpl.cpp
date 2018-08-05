#include "BulletImpl.h"
#include <BW/CBullet.h>
#include <BW/CSprite.h>
#include <BW/Constants.h>
#include <BWAPI/Client/BulletData.h>
#include <Util/Convenience.h>

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"

#include "../../../Debug.h"

namespace BWAPI
{
  int BulletImpl::nextId = 0;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::CBullet* originalBullet, u16 _index)
      : bwOriginalBullet(originalBullet)
      , index(_index)
  {
  }
  //----------------------------------------------- SET EXISTS -----------------------------------------------
  void BulletImpl::setExists(bool exists)
  {
    __exists = exists;
  }
  //---------------------------------------------- SAVE EXISTS -----------------------------------------------
  void BulletImpl::saveExists()
  {
    if ( !lastExists && __exists)
      id = nextId++;
    lastExists = __exists;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::CBullet* BulletImpl::getRawData() const
  {
    return this->bwOriginalBullet;
  }
  //---------------------------------------- BW BULLET TO BWAPI BULLET ---------------------------------------
  BulletImpl* BulletImpl::BWBulletToBWAPIBullet(BW::CBullet* bullet)
  {
    if ( !bullet )
      return nullptr;

    int index = bullet - BW::BWDATA::BulletNodeTable.data();
    if ( index > BW::BULLET_ARRAY_MAX_LENGTH )
      return nullptr;
    return BroodwarImpl.getBulletFromIndex(index);
  }
  void BulletImpl::updateData()
  {
    bool _exists = __exists && bwOriginalBullet;
    if ( _exists )
    {
      for(int i = 0; i < 9; ++i)
      {
        PlayerImpl* player = static_cast<PlayerImpl*>(BroodwarImpl.getPlayer(i));

        if ( !bwOriginalBullet->sprite || !player )
          data.isVisible[i] = false;
        else if ( BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
          data.isVisible[i] = true;
        else
          data.isVisible[i] = static_cast<Game *>(&BroodwarImpl)->isVisible( TilePosition(bwOriginalBullet->sprite->position) );
      }
    }
    else
    {
      MemZero(data.isVisible);
    }

    if ( _exists && 
         (BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) ||
          BroodwarImpl.isReplay() || 
          isVisible()) )
    {
      UnitImpl *_getSource = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->sourceUnit);
      UnitImpl *_getTarget = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->attackTarget.pUnit);
      Player   _getPlayer = _getSource ? _getSource->_getPlayer : nullptr;

      // id, player, type, source
      data.id     = id;
      data.player = (_getSource && _getSource->isAlive && _getPlayer) ? _getPlayer->getID() : -1;
      data.type   = bwOriginalBullet->type;
      data.source = (_getSource && _getSource->exists()) ? _getSource->getID() : -1;

      // position
      data.positionX = bwOriginalBullet->position.x;
      data.positionY = bwOriginalBullet->position.y;

      // angle, velocity
      int d = (int)bwOriginalBullet->currentDirection - 64;
      if ( d < 0 )
        d += 256;
      data.angle     = (double)d * 3.14159265358979323846 / 128.0;
      data.velocityX = (double)(bwOriginalBullet->current_speed.x / 256.0);
      data.velocityY = (double)(bwOriginalBullet->current_speed.y / 256.0);

      // target, targetPosition
      data.target          = (_getTarget && _getTarget->exists()) ? _getTarget->getID() : -1;
      data.targetPositionX = bwOriginalBullet->targetPosition.x;
      data.targetPositionY = bwOriginalBullet->targetPosition.y;

      // removeTimer
      data.removeTimer = bwOriginalBullet->time_remaining;

      // exists
      data.exists  = true;
    }
    else
    {
      data.id              = -1;
      data.player          = -1;
      data.type            = BulletTypes::Unknown;
      data.source          = -1;
      data.positionX       = Positions::Unknown.x;
      data.positionY       = Positions::Unknown.y;
      data.angle           = 0;
      data.velocityX       = 0;
      data.velocityY       = 0;
      data.target          = -1;
      data.targetPositionX = Positions::Unknown.x;
      data.targetPositionY = Positions::Unknown.y;
      data.removeTimer     = 0;
      data.exists          = false;
    }

  }
}