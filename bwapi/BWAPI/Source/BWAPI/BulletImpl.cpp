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
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::BulletType BulletImpl::getType() const
  {
    if (!exists()) return BWAPI::BulletTypes::None;
    return BWAPI::BulletType(this->bwOriginalBullet->type);
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BWAPI::Unit* BulletImpl::getOwner() const
  {
    if (!exists()) return NULL;
    return (BWAPI::Unit*)_getOwner();
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BWAPI::UnitImpl* BulletImpl::_getOwner() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(this->bwOriginalBullet->owner);
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
    Unit* unit = _getOwner();
    if (unit == NULL) return false;
    //bullet is visible only if the owner is visible (at least until a better method is found)
    return unit->isVisible();
  }
  //----------------------------------------------- IS VISIBLE -----------------------------------------------
  bool BulletImpl::isVisible(BWAPI::Player* player) const
  {
    if (!_exists()) return false; //if it really doesn't exist, return false
    Unit* unit = _getOwner();
    if (unit == NULL) return false;
    //bullet is visible only if the owner is visible to the given player (at least until a better method is found)
    return unit->isVisible(player);
  }
  //----------------------------------------------- SET EXISTS -----------------------------------------------
  void BulletImpl::setExists(bool exists)
  {
    __exists = exists;
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