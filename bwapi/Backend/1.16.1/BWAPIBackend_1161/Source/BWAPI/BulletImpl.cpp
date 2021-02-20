#include "BulletImpl.h"
#include "../BW/CBullet.h"
#include "../BW/CSprite.h"
#include "../BW/Constants.h"
#include <BWAPI4/Client/BulletData.h>
#include <BWAPI/BulletType.h>
#include "../Convenience.h"

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"

namespace BWAPI4
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
        PlayerImpl* player = BroodwarImpl.getPlayer(i);

        if ( !bwOriginalBullet->sprite || !player )
          self->isVisible[i] = false;
        else if ( BWAPI4::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
          self->isVisible[i] = true;
        else
          self->isVisible[i] = BroodwarImpl.isVisible( TilePosition(bwOriginalBullet->sprite->position) );
      }
    }
    else
    {
      MemZero(self->isVisible);
    }

    if ( _exists && 
         (BWAPI4::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) ||
          BroodwarImpl.isReplay() || 
          isVisible()) )
    {
      UnitImpl *_getSource = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->sourceUnit);
      UnitImpl *_getTarget = UnitImpl::BWUnitToBWAPIUnit(bwOriginalBullet->attackTarget.pUnit);
      Player   _getPlayer = _getSource ? _getSource->_getPlayer : nullptr;

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
      self->type            = BulletTypes::Unknown;
      self->source          = -1;
      self->positionX       = Positions::Unknown.x;
      self->positionY       = Positions::Unknown.y;
      self->angle           = 0;
      self->velocityX       = 0;
      self->velocityY       = 0;
      self->target          = -1;
      self->targetPositionX = Positions::Unknown.x;
      self->targetPositionY = Positions::Unknown.y;
      self->removeTimer     = 0;
      self->exists          = false;
    }

  }
  //------------------------------------------------- GET ID -------------------------------------------------
  int BulletImpl::getID() const
  {
    return self->id;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::exists() const
  {
    return self->exists;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI4::Player player) const
  {
    if (player == nullptr)  // Default to self
      player = BroodwarImpl.self();

    if (player != nullptr) // Only if player is valid
    {
      int id = player->getID();
      if (id >= 0 && id < 9) // Check if ID is valid
        return self->isVisible[id]; // return visibility
    }
    else  // fallback for self also being null
    {
      for (int i = 0; i < 9; ++i) // check if the bullet is visible by ANY player
      {
        if (self->isVisible[i])
          return true;
      }
    }
    return false;
  }
}