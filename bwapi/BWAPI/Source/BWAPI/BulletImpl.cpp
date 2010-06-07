#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include "BWAPI/GameImpl.h"
#include "BWAPI/UnitImpl.h"

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::Bullet* originalBullet,
                     u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
      , __exists(false)
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
    if (source == NULL || !source->_exists())
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
    return this->bwOriginalBullet->time_left >> 8;
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
    Unit* unit = _getSource();
    if (unit == NULL) return false;
    //bullet is visible only if the owner is visible (at least until a better method is found)
    return unit->isVisible();
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player* player) const
  {
    if (!_exists()) return false; //if it really doesn't exist, return false
    Unit* unit = _getSource();
    if (unit == NULL) return false;
    //bullet is visible only if the owner is visible to the given player (at least until a better method is found)
    return unit->isVisible(player);
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
    return BroodwarImpl.getBullet(index);
  }
}