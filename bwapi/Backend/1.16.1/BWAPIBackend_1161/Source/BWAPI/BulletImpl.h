#pragma once

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
  class BulletImpl
  {
    public:
      int        getID() const;
      bool       exists() const;
      Player     getPlayer() const;
      BulletType getType() const;
      Unit       getSource() const;
      Position   getPosition() const;
      double     getAngle() const;
      double     getVelocityX() const;
      double     getVelocityY() const;
      Unit       getTarget() const;
      Position   getTargetPosition() const;
      int        getRemoveTimer() const;
      bool       isVisible(Player player = nullptr) const;

      BulletImpl(BW::CBullet* originalBullet, u16 index);

      void        setExists(bool exists);
      BW::CBullet* getRawData() const;
      void        saveExists();

      BulletData  data = BulletData();

      void        updateData();

      static BulletImpl* BWBulletToBWAPIBullet(BW::CBullet* bullet);
      static int nextId;
    private:
      BW::CBullet* bwOriginalBullet; /**< Pointer to broodwar unit data table. */
      u16 index;
      int id = -1;
      bool __exists = false;
      bool lastExists = false;
  };
};