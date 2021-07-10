#include "DataEnv.h"

#include <array>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <cassert>

namespace BW {
  UpgradeData upgrades;
  TechData techs;
  WeaponData weapons;
  UnitData units;
  OrderData orders;
  SpriteData sprites;
  ImageData images;
  FlingyData flingies;
  
  void DataEnv::SetUp() {
    initUpgrades();
    initTechs();
    initWeapons();
    initUnits();
    initOrders();
    initSprites();
    initImages();
    initFlingies();
  }

  void DataEnv::TearDown() {}

  template<class T>
  void readArr(std::istream& is, T& arr) {
    if (!is.read(reinterpret_cast<char*>(arr.data()), arr.size() * sizeof(typename T::value_type))) {
      std::ostringstream ss;
      ss << "Failed to read arr data.\n"
        << "Tried reading " << arr.size() << " entries of size " << sizeof(typename T::value_type) << "\n";
      throw std::runtime_error(ss.str());
    }
  }

  std::ifstream loadFile(const std::string& filename) {
    std::filesystem::path path = std::filesystem::absolute(filename);
    std::cerr << "Loading arr: " << path << "\n";
    assert(std::filesystem::exists(path));
    return std::ifstream(path, std::ios::binary);
  }

  void DataEnv::initUpgrades() {
    std::ifstream f = loadFile("arr/upgrades.dat");

    readArr(f, upgrades.oreCostBase);
    readArr(f, upgrades.oreCostFactor);
    readArr(f, upgrades.gasCostBase);
    readArr(f, upgrades.gasCostFactor);
    readArr(f, upgrades.timeCostBase);
    readArr(f, upgrades.timeCostFactor);
    readArr(f, upgrades.depIndex);
    readArr(f, upgrades.icon);
    readArr(f, upgrades.label);
    readArr(f, upgrades.race);
    readArr(f, upgrades.repeats);
    readArr(f, upgrades.isBroodwar);
  }

  void DataEnv::initTechs() {
    std::ifstream f = loadFile("arr/techdata.dat");

    readArr(f, techs.oreCost);
    readArr(f, techs.gasCost);
    readArr(f, techs.timeCost);
    readArr(f, techs.energyCost);
    readArr(f, techs.depIndex);
    readArr(f, techs.useDepIndex);
    readArr(f, techs.icon);
    readArr(f, techs.label);
    readArr(f, techs.race);
    readArr(f, techs.defaultResearched);
    readArr(f, techs.isBroodwar);
  }

  void DataEnv::initWeapons() {
    std::ifstream f = loadFile("arr/weapons.dat");

    readArr(f, weapons.label);
    readArr(f, weapons.graphic);
    readArr(f, weapons.tech);
    readArr(f, weapons.targetFlags);
    readArr(f, weapons.minRange);
    readArr(f, weapons.maxRange);
    readArr(f, weapons.damageUpgrade);
    readArr(f, weapons.behavior);
    readArr(f, weapons.fireType);
    readArr(f, weapons.removeAfter);
    readArr(f, weapons.damageType);
    readArr(f, weapons.innerSplash);
    readArr(f, weapons.middleSplash);
    readArr(f, weapons.outerSplash);
    readArr(f, weapons.damage);
    readArr(f, weapons.damageBonus);
    readArr(f, weapons.cooldown);
    readArr(f, weapons.damageFactor);
    readArr(f, weapons.attackAngle);
    readArr(f, weapons.launchSpin);
    readArr(f, weapons.forwardOffset);
    readArr(f, weapons.upwardOffset);
    readArr(f, weapons.targetErrStr);
    readArr(f, weapons.icon);
  }

  void DataEnv::initUnits() {
    std::ifstream f = loadFile("arr/units.dat");

    readArr(f, units.flingy);
    readArr(f, units.subunit1);
    readArr(f, units.subunit2);
    readArr(f, units.infestationUnit);
    readArr(f, units.constructionGfx);
    readArr(f, units.unitDirection);
    readArr(f, units.shieldEnable);
    readArr(f, units.shields);
    readArr(f, units.hitPoints);
    readArr(f, units.elevationLevel);
    readArr(f, units.movementFlags);
    readArr(f, units.sublabelRank);
    readArr(f, units.compIdleOrder);
    readArr(f, units.humanIdleOrder);
    readArr(f, units.returnToIdleOrder);
    readArr(f, units.attackUnitOrder);
    readArr(f, units.attackMoveOrder);
    readArr(f, units.groundWeapon);
    readArr(f, units.maxGroundHits);
    readArr(f, units.airWeapon);
    readArr(f, units.maxAirHits);
    readArr(f, units.aiFlags);
    readArr(f, units.specialAbilityFlags);
    readArr(f, units.targetAcquisitionRange);
    readArr(f, units.sightRange);
    readArr(f, units.armorUpgrade);
    readArr(f, units.sizeType);
    readArr(f, units.armorAmount);
    readArr(f, units.rightClickActionOrder);
    readArr(f, units.readySfx);
    readArr(f, units.whatSfxFirst);
    readArr(f, units.whatSfxLast);
    readArr(f, units.pissedSfxFirst);
    readArr(f, units.pissedSfxLast);
    readArr(f, units.yesSfxFirst);
    readArr(f, units.yesSfxLast);
    readArr(f, units.placeboxSize);
    readArr(f, units.addonPosition);
    readArr(f, units.dimensions);
    readArr(f, units.portrait);
    readArr(f, units.oreCost);
    readArr(f, units.gasCost);
    readArr(f, units.timeCost);
    readArr(f, units.depIndex);
    readArr(f, units.groupFlags);
    readArr(f, units.supplyProvided);
    readArr(f, units.supplyRequired);
    readArr(f, units.spaceRequired);
    readArr(f, units.spaceProvided);
    readArr(f, units.buildScore);
    readArr(f, units.destroyScore);
    readArr(f, units.mapString);
    readArr(f, units.isBroodwar);
    readArr(f, units.stareditAvailabilityFlags);
  }

  void DataEnv::initOrders() {
    std::ifstream f = loadFile("arr/orders.dat");

    readArr(f, orders.label);
    readArr(f, orders.useWeaponTargetting);
    readArr(f, orders.orderBackground);
    readArr(f, orders.unused3);
    readArr(f, orders.unknown4);
    readArr(f, orders.unused5);
    readArr(f, orders.canBeInterrupted);
    readArr(f, orders.unknown7);
    readArr(f, orders.canBeQueued);
    readArr(f, orders.unknown9);
    readArr(f, orders.canBeObstructed);
    readArr(f, orders.unknown11);
    readArr(f, orders.unused12);
    readArr(f, orders.weapon);
    readArr(f, orders.tech);
    readArr(f, orders.animSequence);
    readArr(f, orders.highlight);
    readArr(f, orders.depIndex);
    readArr(f, orders.obscuredOrder);
  }

  void DataEnv::initSprites() {
    std::ifstream f = loadFile("arr/sprites.dat");

    readArr(f, sprites.image);
    readArr(f, sprites.healthbarSize);
    readArr(f, sprites.unknown);
    readArr(f, sprites.isVisible);
    readArr(f, sprites.selectionCircleType);
    readArr(f, sprites.verticalOffset);
  }

  void DataEnv::initImages() {
    std::ifstream f = loadFile("arr/images.dat");

    readArr(f, images.grpFileIndex);
    readArr(f, images.hasTurns);
    readArr(f, images.isClickable);
    readArr(f, images.usesFullIscript);
    readArr(f, images.drawIfCloaked);
    readArr(f, images.drawFunction);
    readArr(f, images.colorMappingFunction);
    readArr(f, images.iscriptId);
    readArr(f, images.shieldOverlay);
    readArr(f, images.attackOverlay);
    readArr(f, images.damageOverlay);
    readArr(f, images.specialOverlay);
    readArr(f, images.landingOverlay);
    readArr(f, images.liftoffOverlay);
  }

  void DataEnv::initFlingies() {
    std::ifstream f = loadFile("arr/flingy.dat");

    readArr(f, flingies.sprite);
    readArr(f, flingies.topSpeed);
    readArr(f, flingies.acceleration);
    readArr(f, flingies.haltDistance);
    readArr(f, flingies.turnRadius);
    readArr(f, flingies.unused);
    readArr(f, flingies.moveControl);
  }

}
