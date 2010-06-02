#include "BulletImpl.h"
#include <BW/Bullet.h>
#include <BW/Offsets.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  BulletImpl::BulletImpl(BW::Bullet* originalBullet,
                     u16 index)
      : bwOriginalBullet(originalBullet)
      , index(index)
  {
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::AttackType BulletImpl::getType() const
  {
    if (!_exists()) return BWAPI::AttackTypes::None;
    return BWAPI::AttackType(this->bwOriginalBullet->type);
  }
  //---------------------------------------------- GET POSITION ----------------------------------------------
  BWAPI::Position BulletImpl::getPosition() const
  {
    if (!_exists()) return BWAPI::Positions::None;
    return BWAPI::Position(this->bwOriginalBullet->pos_x, this->bwOriginalBullet->pos_y);
  }
  //------------------------------------------------- EXISTS -------------------------------------------------
  bool BulletImpl::_exists() const
  {
    if (this->bwOriginalBullet == NULL) return false;
    return (this->bwOriginalBullet->time_left > 0);
  }
}