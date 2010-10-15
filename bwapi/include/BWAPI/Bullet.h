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
      virtual int getID() const = 0;
      virtual Player* getPlayer() const = 0;
      virtual BulletType getType() const = 0;
      virtual Unit* getSource() const = 0;
      virtual Position getPosition() const = 0;
      virtual double getAngle() const = 0;
      virtual double getVelocityX() const = 0;
      virtual double getVelocityY() const = 0;
      virtual Unit* getTarget() const = 0;
      virtual Position getTargetPosition() const = 0;
      virtual int getRemoveTimer() const = 0;
      virtual bool exists() const = 0;
      virtual bool isVisible() const = 0;
      virtual bool isVisible(Player* player) const = 0;
  };
}