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
  BulletImpl::BulletImpl(BW::Bullet* originalBullet,
                     u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
      , __exists(false)
      , lastExists(false)
      , self(&data)
  {
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
    UnitImpl* _getSource = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->sourceUnit);
    Player*   _getPlayer = NULL;
    if ( _getSource )
      _getPlayer = _getSource->_getPlayer;

    bool _exists = __exists;
    if ( !bwOriginalBullet )
      _exists = false;

    if (_exists)
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
      self->exists = true;
      self->id = id;
      UnitImpl* source  = _getSource;
      if ( !source || !source->isAlive)
        self->player = -1;
      else
        self->player = source->_getPlayer->getID();
      self->type = bwOriginalBullet->type;

      if ( !source || !source->exists())
        self->source = -1;
      else
        self->source = source->getID();

      self->positionX = bwOriginalBullet->position.x;
      self->positionY = bwOriginalBullet->position.y;
      int d = bwOriginalBullet->currentDirection;
      d -= 64;
      if (d < 0)
        d += 256;
      self->angle     = (double)d * 3.14159265358979323846 / 128.0;
      self->velocityX = (double)(bwOriginalBullet->current_speedX / 256.0);
      self->velocityY = (double)(bwOriginalBullet->current_speedY / 256.0);

      Unit* target = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->targetUnit);
      if ( !target || !target->exists())
        self->target = -1;
      else
        self->target = target->getID();
      self->targetPositionX = bwOriginalBullet->targetPosition.x;
      self->targetPositionY = bwOriginalBullet->targetPosition.y;
      self->removeTimer     = bwOriginalBullet->time_remaining;
    }
    else
    {
      self->exists          = false;
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
    }

  }
}