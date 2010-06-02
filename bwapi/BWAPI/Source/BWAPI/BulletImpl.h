
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
  class BulletImpl
  {
    public:
      BWAPI::BulletType getType() const;
      BWAPI::Position getPosition() const;
      int getRemoveTimer() const;

      BulletImpl(BW::Bullet* originalBullet, u16 index);
      ~BulletImpl();
      bool _exists() const;
      //static BulletImpl* BWBulletToBWAPIBullet(BW::Bullet* bullet);
    private:
      BW::Bullet* bwOriginalBullet; /**< Pointer to broodwar unit data table. */
      u16 index;
  };
};