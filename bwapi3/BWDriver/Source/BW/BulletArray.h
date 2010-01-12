#pragma once
/**
 *  Direct mapping of the bullet node table in bw memory.
 */

#include "Bullet.h"

namespace BW
{
  struct BulletArray
  {
    static const int count = 100;
    Bullet bullets[count];

    int getIndexByBullet(Bullet* bullet);
  };
}
