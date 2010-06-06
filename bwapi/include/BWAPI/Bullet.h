#pragma once

#include <BWAPI/BulletType.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Player;
  class Unit;
  class Bullet
  {
    public:
      virtual BWAPI::BulletType getType() const = 0;
      virtual BWAPI::Unit* getOwner() const = 0;
      virtual BWAPI::Position getPosition() const = 0;
      virtual double getVelocityX() const = 0;
      virtual double getVelocityY() const = 0;
      virtual int getRemoveTimer() const = 0;
      virtual bool exists() const = 0;
      virtual bool isVisible() const = 0;
      virtual bool isVisible(BWAPI::Player* player) const = 0;
  };
}