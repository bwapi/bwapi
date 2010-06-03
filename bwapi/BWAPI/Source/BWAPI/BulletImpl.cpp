#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>
#include "BWAPI/GameImpl.h"

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
    if (!_exists()) return BWAPI::BulletTypes::None;
    return BWAPI::BulletType(this->bwOriginalBullet->type);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  BWAPI::Position BulletImpl::getPosition() const
  {
    if (!_exists()) return BWAPI::Positions::None;
    return BWAPI::Position(this->bwOriginalBullet->pos_x, this->bwOriginalBullet->pos_y);
  }
  //-------------------------------------------- GET REMOVE TIMER --------------------------------------------
  int BulletImpl::getRemoveTimer() const
  {
    if (!_exists()) return 0;
    return this->bwOriginalBullet->time_left;
  }
  //----------------------------------------------- SET EXISTS -----------------------------------------------
  void BulletImpl::setExists(bool exists)
  {
    __exists = exists;
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::_exists() const
  {
    if (this->bwOriginalBullet == NULL) return false;
    return __exists;
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