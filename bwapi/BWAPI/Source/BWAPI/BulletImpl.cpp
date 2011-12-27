#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BWAPI/UnitImpl.h"

#include "../../Debug.h"

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::Bullet* originalBullet, u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
      , __exists(false)
      , lastExists(false)
      , self(&data)
      , id(-1)
  {
    MemZero(data);
  }
  BulletImpl::~BulletImpl()
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
      id = BroodwarImpl.bulletCount++;
    lastExists = __exists;
  }
  //---------------------------------------------- GET RAW DATA ----------------------------------------------
  BW::Bullet* BulletImpl::getRawData() const
  {
    return this->bwOriginalBullet;
  }
  //---------------------------------------- BW BULLET TO BWAPI BULLET ---------------------------------------
  BulletImpl* BulletImpl::BWBulletToBWAPIBullet(BW::Bullet* bullet)
  {
    if ( !bullet )
      return NULL;

    u16 index = (u16)( ((u32)bullet - (u32)BW::BWDATA_BulletNodeTable) / 112) & 0x7F;
    if ( index > BULLET_ARRAY_MAX_LENGTH )
      return NULL;
    return BroodwarImpl.getBulletFromIndex(index);
  }
  void BulletImpl::updateData()
  {
    bool _exists = __exists && bwOriginalBullet;
    if ( _exists )
    {
      for(int i = 0; i < 9; ++i)
      {
        PlayerImpl* player = (PlayerImpl*)Broodwar->getPlayer(i);

        if ( !bwOriginalBullet->sprite || !player )
          self->isVisible[i] = false;
        else if ( BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
          self->isVisible[i] = true;
        else
          self->isVisible[i] = BroodwarImpl.isVisible(bwOriginalBullet->sprite->position.x/32, bwOriginalBullet->sprite->position.y/32);
      }
    }
    else
    {
      MemZero(self->isVisible);
    }

    if ( _exists && 
         (BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) ||
          BroodwarImpl._isReplay() || 
          isVisible()) )
    {
      UnitImpl *_getSource = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->sourceUnit);
      UnitImpl *_getTarget = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->targetUnit);
      Player   *_getPlayer = _getSource ? _getSource->_getPlayer : NULL;

      // id, player, type, source
      self->id      = id;
      self->player  = (_getSource && _getSource->isAlive && _getPlayer) ? _getPlayer->getID() : -1;
      self->type    = bwOriginalBullet->type;
      self->source  = (_getSource && _getSource->exists()) ? _getSource->getID() : -1;

      // position
      self->positionX = bwOriginalBullet->position.x;
      self->positionY = bwOriginalBullet->position.y;

      // angle, velocity
      int d = (int)bwOriginalBullet->currentDirection - 64;
      if ( d < 0 )
        d += 256;
      self->angle     = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)(bwOriginalBullet->current_speed.x / 256.0);
      self->velocityY = (double)(bwOriginalBullet->current_speed.y / 256.0);

      // target, targetPosition
      self->target          = (_getTarget && _getTarget->exists()) ? _getTarget->getID() : -1;
      self->targetPositionX = bwOriginalBullet->targetPosition.x;
      self->targetPositionY = bwOriginalBullet->targetPosition.y;

      // removeTimer
      self->removeTimer = bwOriginalBullet->time_remaining;

      // exists
      self->exists  = true;
    }
    else
    {
      self->id              = -1;
      self->player          = -1;
      self->type            = BulletTypes::None;
      self->source          = -1;
      self->positionX       = Positions::None.x();
      self->positionY       = Positions::None.y();
      self->angle           = 0;
      self->velocityX       = 0;
      self->velocityY       = 0;
      self->target          = -1;
      self->targetPositionX = 0;
      self->targetPositionY = 0;
      self->removeTimer     = 0;
      self->exists          = false;
    }

  }
}