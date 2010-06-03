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
  class BulletImpl : public Bullet
  {
    public:
      virtual BWAPI::BulletType getType() const;
      virtual BWAPI::Position getPosition() const;
      virtual int getRemoveTimer() const;

      BulletImpl(BW::Bullet* originalBullet, u16 index);
      ~BulletImpl();
      void setExists(bool exists);
      bool _exists() const;
      BW::Bullet* getRawData() const;
      static BulletImpl* BWBulletToBWAPIBullet(BW::Bullet* bullet);
    private:
      BW::Bullet* bwOriginalBullet; /**< Pointer to broodwar unit data table. */
      u16 index;
      bool __exists;
  };
};