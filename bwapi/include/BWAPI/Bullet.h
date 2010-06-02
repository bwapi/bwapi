#pragma once

#include <BWAPI/BulletType.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Player;
  class Bullet
  {
    public:
      virtual BWAPI::BulletType getType() const = 0;
      virtual BWAPI::Position getPosition() const = 0;
      virtual int getRemoveTimer() const = 0;
  };
}