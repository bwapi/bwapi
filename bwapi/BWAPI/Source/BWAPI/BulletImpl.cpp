#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include "BWAPI/GameImpl.h"
#include "BWAPI/PlayerImpl.h"
#include "BWAPI/UnitImpl.h"

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::Bullet* originalBullet,
                     u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
      , __exists(false)
      , lastExists(false)
  {
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  int BulletImpl::getID() const
  {
    if (!exists()) return -1;
    return id;
  }
  //------------------------------------------------ GET PLAYER ----------------------------------------------
  BWAPI::Player* BulletImpl::getPlayer() const
  {
    if (!exists()) return NULL;
    UnitImpl* source  = _getSource();
    if (source == NULL || !source->_exists)
      return NULL;
    return source->getPlayer();
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::BulletType BulletImpl::getType() const
  {
    if (!exists()) return BWAPI::BulletTypes::None;
    return BWAPI::BulletType(this->bwOriginalBullet->type);
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BWAPI::Unit* BulletImpl::getSource() const
  {
    if (!exists()) return NULL;
    Unit* source = _getSource();;
    if (source == NULL || !source->exists())
      return NULL;
    return source;
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BWAPI::UnitImpl* BulletImpl::_getSource() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->bwOriginalBullet->sourceUnit);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  BWAPI::Position BulletImpl::getPosition() const
  {
    if (!exists()) return BWAPI::Positions::None;
    return BWAPI::Position(this->bwOriginalBullet->position.x, this->bwOriginalBullet->position.y);
  }
  //----------------------------------------------- GET ANGLE ------------------------------------------------
  double BulletImpl::getAngle() const
  {
    if (!exists()) return 0;
    int d = this->bwOriginalBullet->currentDirection;
    d -= 64;
    if (d < 0)
      d += 256;
    double a = (double)d * 3.14159265358979323846 / 128.0;
    return a;
  }
  //--------------------------------------------- GET VELOCITY X ---------------------------------------------
  double BulletImpl::getVelocityX() const
  {
    if (!exists()) return 0;
    return (double)this->bwOriginalBullet->current_speedX / 256.0;
  }
  //--------------------------------------------- GET VELOCITY Y ---------------------------------------------
  double BulletImpl::getVelocityY() const
  {
    if (!exists()) return 0;
    return (double)this->bwOriginalBullet->current_speedY / 256.0;
  }
  //------------------------------------------------ GET TARGET ----------------------------------------------
  BWAPI::Unit* BulletImpl::getTarget() const
  {
    if (!exists())
      return NULL;
    Unit* target = UnitImpl::BWUnitToBWAPIUnit(this->bwOriginalBullet->targetUnit);
    if (target==NULL || !target->exists())
      return NULL;
    return target;
  }
  //-------------------------------------------- GET TARGET POSITION -----------------------------------------
  BWAPI::Position BulletImpl::getTargetPosition() const
  {
    if (!exists()) return BWAPI::Positions::None;
    return BWAPI::Position(this->bwOriginalBullet->targetPosition.x, this->bwOriginalBullet->targetPosition.y);
  }
  //-------------------------------------------- GET REMOVE TIMER --------------------------------------------
  int BulletImpl::getRemoveTimer() const
  {
    if (!exists()) return 0;
    return this->bwOriginalBullet->max_time;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::exists() const
  {
    if (!_exists()) return false; //if it really doesn't exist, return false
    if (isVisible()) return true; //if we can see it, return true

    //if we get here, the bullet exists but the player can't see it, so we either return true of false
    //depending on the CompleteMapInformation cheat flag
    return (BroodwarImpl._isReplay() || BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation));
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::_exists() const
  {
    if (this->bwOriginalBullet == NULL) return false;
    return __exists;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible() const
  {
    if (!_exists()) return false; //if it really doesn't exist, return false

    if (this->bwOriginalBullet->sprite == NULL)
      return false;

    if (BroodwarImpl._isReplay())
      return this->bwOriginalBullet->sprite->visibilityFlags > 0;

    //temporary until we figure out a better way to get visibility of lurker bullets
    if (_getSource()!=NULL && _getSource()->isVisible()) return true;

    return (this->bwOriginalBullet->sprite->visibilityFlags & (1 << BroodwarImpl.BWAPIPlayer->getIndex())) != 0;
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player* player) const
  {
    if (!_exists()) return false; //if it really doesn't exist, return false

    if (this->bwOriginalBullet->sprite == NULL)
      return false;

    if (!BroodwarImpl._isReplay() && !BWAPI::BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return false;

    if (this->getPlayer() == player)
      return true;

    if (player == NULL)
      return false;

    //temporary until we figure out a better way to get visibility of lurker bullets
    if (_getSource()!=NULL && _getSource()->isVisible(player)) return true;

    int playerIndex=((PlayerImpl*)player)->getIndex();
    if (playerIndex<0 || playerIndex>8) //probably the neutral player so just return true if any player can see it
      return this->bwOriginalBullet->sprite->visibilityFlags > 0;

    return (this->bwOriginalBullet->sprite->visibilityFlags & (1 << playerIndex)) != 0;
  }
  //----------------------------------------------- SET EXISTS -----------------------------------------------
  void BulletImpl::setExists(bool exists)
  {
    __exists = exists;
  }
  //---------------------------------------------- SAVE EXISTS -----------------------------------------------
  void BulletImpl::saveExists()
  {
    if (lastExists==false && __exists==true)
    {
      id = BroodwarImpl.bulletCount;
      BroodwarImpl.bulletCount++;
    }
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
    if (bullet == NULL)
      return NULL;

    u16 index = (u16)( ((u32)bullet - (u32)BW::BWDATA_BulletNodeTable) / 112) & 0x7F;
    if (index > BW::BULLET_ARRAY_MAX_LENGTH)
      return NULL;
    return BroodwarImpl.getBulletFromIndex(index);
  }
}