#pragma once

#include "BWAPI/Bullet.h"

#include <Util/Types.h>
#include <BWAPI/Position.h>
#include <BWAPI/BulletType.h>
namespace BW    { struct Bullet; };

namespace BWAPI
{
  /**
   * Interface for broodwar bullets, can be used to obtain any information
   * about bullets and spells
   */
  class UnitImpl;
  class BulletImpl : public Bullet
  {
    public:
      virtual BWAPI::BulletType getType() const;
      virtual BWAPI::Unit*      getOwner() const;
      virtual BWAPI::Position   getPosition() const;
      virtual double            getVelocityX() const;
      virtual double            getVelocityY() const;
      virtual int               getRemoveTimer() const;
      virtual bool              exists() const;
      virtual bool              isVisible() const;
      virtual bool              isVisible(Player* player) const;

      BulletImpl(BW::Bullet* originalBullet, u16 index);
      ~BulletImpl();

      BWAPI::UnitImpl*  _getOwner() const;
      bool              _exists() const;

      void        setExists(bool exists);
      BW::Bullet* getRawData() const;

      static BulletImpl* BWBulletToBWAPIBullet(BW::Bullet* bullet);
    private:
      BW::Bullet* bwOriginalBullet; /**< Pointer to broodwar unit data table. */
      u16 index;
      bool __exists;
  };
};