#pragma once

#include "../Types.h"
#include <BWAPI/Position.h>
#include <BWAPI4/Client/BulletData.h>
namespace BW    { class CBullet; };

namespace BWAPI4
{
  // forwards
  class UnitImpl;
  class BulletType;

  class PlayerImpl;
  typedef PlayerImpl* Player;

  /**
   * Interface for broodwar bullets, can be used to obtain any information
   * about bullets and spells
   */
  class BulletImpl
  {
    public:
      int        getID() const;
      bool       exists() const;
      bool       isVisible(Player player = nullptr) const;

      BulletImpl(BW::CBullet* originalBullet, u16 index);

      void        setExists(bool exists);
      BW::CBullet* getRawData() const;
      void        saveExists();

      BulletData  data = BulletData();
      BulletData* self = &data;

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