#include <BWAPI/Bullet.h>

namespace BWAPI {
  Player Bullet::getPlayer() const
  {
    return { getGame(), getBulletData().player };
  }
}
