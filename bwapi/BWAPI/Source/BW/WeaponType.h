#pragma once

#include <string>
#include <map>
#include <Util/Bitmask.h>
#include <Util/Types.h>

#include "WeaponTargetFlags.h"
#include "WeaponID.h"
#include "DamageID.h"
#include "ExplosionID.h"
#include "Offsets.h"
#include "UpgradeType.h"

namespace BW
{
  /**
  * Definition of weapon prototype, it's possible types are listed in #BW#WeaponID.
  * The only data this class holds is the enumeration of type u16, so it can be used directly
  * in the mapping of #BW#Weapon
  *
  */
  class WeaponType
  {
    public :
      WeaponType();
      WeaponType(const u8& id);
      bool operator == (const u8& id) const;
      bool operator != (const u8& id) const;
      bool operator == (const WeaponType& type) const;
      bool operator  < (const WeaponType& type) const;
      const char*            getName() const;
      u8                     getID() const;
      u16                    damageAmount() const;
      u16                    damageBonus() const;
      u8                     damageCooldown() const;
      u8                     damageFactor() const;
      BW::UpgradeType        upgradeType() const;
      u8                     damageType() const;
      u8                     explosionType() const;
      u32                    minRange() const;
      u32                    maxRange() const;
      u16                    innerSplashRadius() const;
      u16                    medianSplashRadius() const;
      u16                    outerSplashRadius() const;
      bool                   targetsAir() const;
      bool                   targetsGround() const;
      bool                   targetsMechanical() const;
      bool                   targetsOrganic() const;
      bool                   targetsNonBuilding() const;
      bool                   targetsNonRobotic() const;
      bool                   targetsTerrain() const;
      bool                   targetsOrgOrMech() const;
      bool                   targetsOwn() const;
      u8                     id;
    private :
  };
};

