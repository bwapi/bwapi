#pragma once

#include "WeaponTypeId.h"
#include "UnitTypeId.h"
#include "TechTypeId.h"
#include "UpgradeTypeId.h"
#include "DamageTypeId.h"
#include "ExplosionTypeId.h"

namespace BWAPI
{
  struct WeaponType
  {
    int id;
    const char* name;
    TechTypeId techType;
    UnitTypeId whatUses;
    int damageAmount;
    int damageBonus;
    int damageCooldown;
    int damageFactor;
    UpgradeTypeId upgradeType;
    DamageTypeId damageType;
    ExplosionTypeId explosionType;
    int minRange;
    int maxRange;
    int innerSplashRadius;
    int medianSplashRadius;
    int outerSplashRadius;
    bool targetsAir;
    bool targetsGround;
    bool targetsMechanical;
    bool targetsOrganic;
    bool targetsNonBuilding;
    bool targetsNonRobotic;
    bool targetsTerrain;
    bool targetsOrgOrMech;
    bool targetsOwn;
    bool valid;
  };
}
