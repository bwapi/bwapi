#pragma once
#include <BWAPI.h>
#include "BulletData.h"
#include <string>

namespace BWAPI
{
  class PlayerInterface;
  typedef PlayerInterface *Player;

  class BulletImpl : public BulletInterface
  {
    private:
      const BulletData* self;
      int index;
    public:

      BulletImpl(int index);
      virtual int getID() const override;
      virtual bool exists() const override;
      virtual Player getPlayer() const override;
      virtual BulletType getType() const override;
      virtual Unit getSource() const override;
      virtual Position getPosition() const override;
      virtual double getAngle() const override;
      virtual double getVelocityX() const override;
      virtual double getVelocityY() const override;
      virtual Unit getTarget() const override;
      virtual Position getTargetPosition() const override;
      virtual int getRemoveTimer() const override;
      virtual bool isVisible(Player player = nullptr) const override;
  };
}
