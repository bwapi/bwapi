#pragma once
#include "Race.h"
#include "RaceId.h"
#include "TechTypeId.h"
#include "UpgradeTypeId.h"
#include "UnitSizeTypeId.h"
#include "WeaponTypeId.h"

namespace BWAPI
{
  struct UnitType
  {
    struct BuildRequisite
    {
      UnitTypeId unitType;
      int count;
    };

    int id;
    const char* name;
    const char* subLabel;
    RaceId race;

    BuildRequisite whatBuilds;
    TechTypeId requiredTech;
    UpgradeTypeId armorUpgrade;

    int maxHitPoints;
    int maxShields;
    int maxEnergy;
    int armor;

    int mineralPrice;
    int gasPrice;
    int buildTime;

    int supplyRequired;
    int supplyProvided;
    int spaceRequired;
    int spaceProvided;
    int buildScore;
    int destroyScore;

    UnitSizeTypeId unitSizeType;
    int tileWidth;
    int tileHeight;
    int dimensionLeft;
    int dimensionUp;
    int dimensionRight;
    int dimensionDown;

    int seekRange;
    int sightRange;
    WeaponTypeId groundWeapon;
    int maxGroundHits;
    WeaponTypeId airWeapon;
    int maxAirHits;

    double topSpeed;
    int acceleration;
    int haltDistance;
    int turnRadius;

    bool canProduce;
    bool canAttack;
    bool canMove;
    bool isFlyer;
    bool regeneratesHP;
    bool isSpellcaster;
    bool hasPermanentCloak;
    bool isInvincible;
    bool isOrganic;
    bool isMechanical;
    bool isRobotic;
    bool isDetector;
    bool isResourceContainer;
    bool isResourceDepot;
    bool isWorker;
    bool requiresPsi;
    bool requiresCreep;
    bool isTwoUnitsInOneEgg;
    bool isBurrowable;
    bool isCloakable;
    bool isBuilding;
    bool isAddon;
    bool isFlyingBuilding;
    bool isNeutral;
    bool isRefinery;

    bool valid;
  };
}
