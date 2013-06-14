#pragma once

#include "BWAPI/Bullet.h"

#include <Util/Types.h>
#include <BWAPI/Position.h>
#include <BWAPI/Client/BulletData.h>
namespace BW    { class CBullet; };

namespace BWAPI
{
  // forwards
  class UnitImpl;
  class BulletType;

  /**
   * Interface for broodwar bullets, can be used to obtain any information
   * about bullets and spells
   */
  class BulletImpl : public BulletInterface
  {
    public:
      virtual int        getID() const override;
      virtual bool       exists() const override;
      virtual Player     getPlayer() const override;
      virtual BulletType getType() const override;
      virtual Unit       getSource() const override;
      virtual Position   getPosition() const override;
      virtual double     getAngle() const override;
      virtual double     getVelocityX() const override;
      virtual double     getVelocityY() const override;
      virtual Unit       getTarget() const override;
      virtual Position   getTargetPosition() const override;
      virtual int        getRemoveTimer() const override;
      virtual bool       isVisible(Player player = nullptr) const override;

      BulletImpl(BW::CBullet* originalBullet, u16 index);
      ~BulletImpl();

      void        setExists(bool exists);
      BW::CBullet* getRawData() const;
      void        saveExists();

      BulletData* self;
      BulletData  data;
      void        updateData();

      static BulletImpl* BWBulletToBWAPIBullet(BW::CBullet* bullet);
    private:
      BW::CBullet* bwOriginalBullet; /**< Pointer to broodwar unit data table. */
      u16 index;
      int id;
      bool __exists;
      bool lastExists;
  };
};