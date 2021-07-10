#pragma once
#include <gtest/gtest.h>

#include <cstdint>
#include <array>

namespace BW {

  struct point {
    std::int16_t x, y;
  };

  struct rect {
    std::int16_t left, top, right, bottom;
  };

  struct UpgradeData {
    std::array<std::uint16_t, 61> oreCostBase;
    std::array<std::uint16_t, 61> oreCostFactor;
    std::array<std::uint16_t, 61> gasCostBase;
    std::array<std::uint16_t, 61> gasCostFactor;
    std::array<std::uint16_t, 61> timeCostBase;
    std::array<std::uint16_t, 61> timeCostFactor;
    std::array<std::uint16_t, 61> depIndex;
    std::array<std::uint16_t, 61> icon;
    std::array<std::uint16_t, 61> label;
    std::array<std::uint8_t, 61> race;
    std::array<std::uint8_t, 61> repeats;
    std::array<std::uint8_t, 61> isBroodwar;
  };

  struct TechData {
    std::array<std::uint16_t, 44> oreCost;
    std::array<std::uint16_t, 44> gasCost;
    std::array<std::uint16_t, 44> timeCost;
    std::array<std::uint16_t, 44> energyCost;
    std::array<std::uint16_t, 44> depIndex;
    std::array<std::uint16_t, 44> useDepIndex;
    std::array<std::uint16_t, 44> icon;
    std::array<std::uint16_t, 44> label;
    std::array<std::uint8_t, 44> race;
    std::array<std::uint8_t, 44> defaultResearched;
    std::array<std::uint8_t, 44> isBroodwar;
  };

  struct WeaponData {
    std::array<std::uint16_t, 130> label;
    std::array<std::uint32_t, 130> graphic;
    std::array<std::uint8_t, 130> tech;
    std::array<std::uint16_t, 130> targetFlags;
    std::array<std::uint32_t, 130> minRange;
    std::array<std::uint32_t, 130> maxRange;
    std::array<std::uint8_t, 130> damageUpgrade;
    std::array<std::uint8_t, 130> behavior;
    std::array<std::uint8_t, 130> fireType;
    std::array<std::uint8_t, 130> removeAfter;
    std::array<std::uint8_t, 130> damageType;
    std::array<std::uint16_t, 130> innerSplash;
    std::array<std::uint16_t, 130> middleSplash;
    std::array<std::uint16_t, 130> outerSplash;
    std::array<std::uint16_t, 130> damage;
    std::array<std::uint16_t, 130> damageBonus;
    std::array<std::uint8_t, 130> cooldown;
    std::array<std::uint8_t, 130> damageFactor;
    std::array<std::uint8_t, 130> attackAngle;
    std::array<std::uint8_t, 130> launchSpin;
    std::array<std::int8_t, 130> forwardOffset;
    std::array<std::int8_t, 130> upwardOffset;
    std::array<std::uint16_t, 130> targetErrStr;
    std::array<std::uint16_t, 130> icon;
  };

  struct UnitData {
    std::array<std::uint8_t, 228> flingy;
    std::array<std::uint16_t, 228> subunit1;
    std::array<std::uint16_t, 228> subunit2;
    std::array<std::uint16_t, 96> infestationUnit;
    std::array<std::uint32_t, 228> constructionGfx;
    std::array<std::uint8_t, 228> unitDirection;
    std::array<std::uint8_t, 228> shieldEnable;
    std::array<std::uint16_t, 228> shields;
    std::array<std::uint32_t, 228> hitPoints;
    std::array<std::uint8_t, 228> elevationLevel;
    std::array<std::uint8_t, 228> movementFlags;
    std::array<std::uint8_t, 228> sublabelRank;
    std::array<std::uint8_t, 228> compIdleOrder;
    std::array<std::uint8_t, 228> humanIdleOrder;
    std::array<std::uint8_t, 228> returnToIdleOrder;
    std::array<std::uint8_t, 228> attackUnitOrder;
    std::array<std::uint8_t, 228> attackMoveOrder;
    std::array<std::uint8_t, 228> groundWeapon;
    std::array<std::uint8_t, 228> maxGroundHits;
    std::array<std::uint8_t, 228> airWeapon;
    std::array<std::uint8_t, 228> maxAirHits;
    std::array<std::uint8_t, 228> aiFlags;
    std::array<std::uint32_t, 228> specialAbilityFlags;
    std::array<std::uint8_t, 228> targetAcquisitionRange;
    std::array<std::uint8_t, 228> sightRange;
    std::array<std::uint8_t, 228> armorUpgrade;
    std::array<std::uint8_t, 228> sizeType;
    std::array<std::uint8_t, 228> armorAmount;
    std::array<std::uint8_t, 228> rightClickActionOrder;
    std::array<std::uint16_t, 106> readySfx;
    std::array<std::uint16_t, 228> whatSfxFirst;
    std::array<std::uint16_t, 228> whatSfxLast;
    std::array<std::uint16_t, 106> pissedSfxFirst;
    std::array<std::uint16_t, 106> pissedSfxLast;
    std::array<std::uint16_t, 106> yesSfxFirst;
    std::array<std::uint16_t, 106> yesSfxLast;
    std::array<point, 228> placeboxSize;
    std::array<point, 96> addonPosition;
    std::array<rect, 228> dimensions;
    std::array<std::uint16_t, 228> portrait;
    std::array<std::uint16_t, 228> oreCost;
    std::array<std::uint16_t, 228> gasCost;
    std::array<std::uint16_t, 228> timeCost;
    std::array<std::uint16_t, 228> depIndex;
    std::array<std::uint8_t, 228> groupFlags;
    std::array<std::uint8_t, 228> supplyProvided;
    std::array<std::uint8_t, 228> supplyRequired;
    std::array<std::uint8_t, 228> spaceRequired;
    std::array<std::uint8_t, 228> spaceProvided;
    std::array<std::uint16_t, 228> buildScore;
    std::array<std::uint16_t, 228> destroyScore;
    std::array<std::uint16_t, 228> mapString;
    std::array<std::uint8_t, 228> isBroodwar;
    std::array<std::uint16_t, 228> stareditAvailabilityFlags;
  };

  struct OrderData {
    std::array<std::uint16_t, 189> label;
    std::array<std::uint8_t, 189> useWeaponTargetting;
    std::array<std::uint8_t, 189> orderBackground;
    std::array<std::uint8_t, 189> unused3;
    std::array<std::uint8_t, 189> unknown4;
    std::array<std::uint8_t, 189> unused5;
    std::array<std::uint8_t, 189> canBeInterrupted;
    std::array<std::uint8_t, 189> unknown7;
    std::array<std::uint8_t, 189> canBeQueued;
    std::array<std::uint8_t, 189> unknown9;
    std::array<std::uint8_t, 189> canBeObstructed;
    std::array<std::uint8_t, 189> unknown11;
    std::array<std::uint8_t, 189> unused12;
    std::array<std::uint8_t, 189> weapon;
    std::array<std::uint8_t, 189> tech;
    std::array<std::uint8_t, 189> animSequence;
    std::array<std::uint16_t, 189> highlight;
    std::array<std::uint16_t, 189> depIndex;
    std::array<std::uint8_t, 189> obscuredOrder;
  };

  struct SpriteData {
    std::array<std::uint16_t, 517> image;
    std::array<std::uint8_t, 387> healthbarSize;
    std::array<std::uint8_t, 517> unknown;
    std::array<std::uint8_t, 517> isVisible;
    std::array<std::uint8_t, 387> selectionCircleType;
    std::array<std::uint8_t, 387> verticalOffset;
  };

  struct ImageData {
    std::array<std::uint32_t, 999> grpFileIndex;
    std::array<std::uint8_t, 999> hasTurns;
    std::array<std::uint8_t, 999> isClickable;
    std::array<std::uint8_t, 999> usesFullIscript;
    std::array<std::uint8_t, 999> drawIfCloaked;
    std::array<std::uint8_t, 999> drawFunction;
    std::array<std::uint8_t, 999> colorMappingFunction;
    std::array<std::uint32_t, 999> iscriptId;
    std::array<std::uint32_t, 999> shieldOverlay;
    std::array<std::uint32_t, 999> attackOverlay;
    std::array<std::uint32_t, 999> damageOverlay;
    std::array<std::uint32_t, 999> specialOverlay;
    std::array<std::uint32_t, 999> landingOverlay;
    std::array<std::uint32_t, 999> liftoffOverlay;
  };

  struct FlingyData {
    std::array<std::uint16_t, 209> sprite;
    std::array<std::uint32_t, 209> topSpeed;
    std::array<std::uint16_t, 209> acceleration;
    std::array<std::uint32_t, 209> haltDistance;
    std::array<std::uint8_t, 209> turnRadius;
    std::array<std::uint8_t, 209> unused;
    std::array<std::uint8_t, 209> moveControl;
  };

  extern UpgradeData upgrades;
  extern TechData techs;
  extern WeaponData weapons;
  extern UnitData units;
  extern OrderData orders;
  extern SpriteData sprites;
  extern ImageData images;
  extern FlingyData flingies;

  class DataEnv : public ::testing::Environment {
  public:
    void SetUp() override;
    void TearDown() override;

  private:
    void initUpgrades();
    void initTechs();
    void initWeapons();
    void initUnits();
    void initOrders();
    void initSprites();
    void initImages();
    void initFlingies();
  };

}
