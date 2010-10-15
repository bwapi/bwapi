#pragma once
#include <BWAPI.h>
#include "BulletData.h"
#include <set>
#include <string>

namespace BWAPI
{
  class Player;
  class Unit;
  class BulletImpl : public Bullet
  {
    private:
      const BulletData* self;
      int index;
    public:

      BulletImpl(int index);
      virtual int getID() const;
      virtual Player* getPlayer() const;
      virtual BulletType getType() const;
      virtual Unit* getSource() const;
      virtual Position getPosition() const;
      virtual double getAngle() const;
      virtual double getVelocityX() const;
      virtual double getVelocityY() const;
      virtual Unit* getTarget() const;
      virtual Position getTargetPosition() const;
      virtual int getRemoveTimer() const;
      virtual bool exists() const;
      virtual bool isVisible() const;
      virtual bool isVisible(Player* player) const;
  };
}
