#include "main.h"

#include <BWAPI.h>
#include <fstream>
#include <iostream>
#include <set>
#include "helpers.h"

std::string tab(size_t level)
{
  return std::string( level*2, ' ' );
}

void genUnitTypeTest()
{
	std::ofstream of("testUnitTypes.txt");
  of.setf(std::ios::showpoint);
  of.setf(std::ios::boolalpha);
  std::set<BWAPI::UnitType> types(BWAPI::UnitTypes::allUnitTypes().begin(), BWAPI::UnitTypes::allUnitTypes().end());
  for (auto t : types)
  {
    of << tab(2) << "TEST_METHOD(Test" << t << ")\n";
    of << tab(2) << "{\n";
    of << tab(3) << "auto t = BWAPI::UnitTypes::" << t << ";\n\n";

#define ASS_EQ(n) of << tab(3) << "Assert_" #n "(" << t.n() << ");\n"
#define ASS_EQ_NS(n) of << tab(3) << "Assert_" #n "(BWAPI::" << namespaceOf<decltype(t.n())>::value << "::" << t.n() << ");\n"

    ASS_EQ_NS(getRace);
    ASS_EQ_NS(requiredTech);
    ASS_EQ_NS(cloakingTech);
    ASS_EQ_NS(armorUpgrade);
    ASS_EQ(maxHitPoints);
    ASS_EQ(maxShields);
    ASS_EQ(maxEnergy);
    ASS_EQ(armor);
    ASS_EQ(mineralPrice);
    ASS_EQ(gasPrice);
    ASS_EQ(buildTime);
    ASS_EQ(supplyRequired);
    ASS_EQ(supplyProvided);
    ASS_EQ(spaceRequired);
    ASS_EQ(spaceProvided);
    ASS_EQ(buildScore);
    ASS_EQ(destroyScore);
    ASS_EQ_NS(size);
    ASS_EQ(tileWidth);
    ASS_EQ(tileHeight);
    of << tab(3) << "Assert_tileSize(BWAPI::TilePosition" << t.tileSize() << ");\n";
    ASS_EQ(dimensionLeft);
    ASS_EQ(dimensionUp);
    ASS_EQ(dimensionRight);
    ASS_EQ(dimensionDown);
    ASS_EQ(width);
    ASS_EQ(height);
    ASS_EQ(seekRange);
    ASS_EQ(sightRange);
    ASS_EQ_NS(groundWeapon);
    ASS_EQ(maxGroundHits);
    ASS_EQ_NS(airWeapon);
    ASS_EQ(maxAirHits);
    ASS_EQ(topSpeed);
    ASS_EQ(acceleration);
    ASS_EQ(haltDistance);
    ASS_EQ(turnRadius);
    ASS_EQ(canProduce);
    ASS_EQ(canAttack);
    ASS_EQ(canMove);
    ASS_EQ(isFlyer);
    ASS_EQ(regeneratesHP);
    ASS_EQ(isSpellcaster);
    ASS_EQ(hasPermanentCloak);
    ASS_EQ(isInvincible);
    ASS_EQ(isOrganic);
    ASS_EQ(isMechanical);
    ASS_EQ(isRobotic);
    ASS_EQ(isDetector);
    ASS_EQ(isResourceContainer);
    ASS_EQ(isResourceDepot);
    ASS_EQ(isRefinery);
    ASS_EQ(isWorker);
    ASS_EQ(requiresPsi);
    ASS_EQ(requiresCreep);
    ASS_EQ(isTwoUnitsInOneEgg);
    ASS_EQ(isBurrowable);
    ASS_EQ(isCloakable);
    ASS_EQ(isBuilding);
    ASS_EQ(isAddon);
    ASS_EQ(isFlyingBuilding);
    ASS_EQ(isNeutral);
    ASS_EQ(isHero);
    ASS_EQ(isPowerup);
    ASS_EQ(isBeacon);
    ASS_EQ(isFlagBeacon);
    ASS_EQ(isSpecialBuilding);
    ASS_EQ(isSpell);
    ASS_EQ(producesLarva);
    ASS_EQ(isMineralField);
    ASS_EQ(isCritter);
    ASS_EQ(canBuildAddon);

    of << tab(2) << "}\n";
  }
}
int main()
{
  genUnitTypeTest();

  writeUnitInfo();
  writeColorInfo();
  writeWeaponInfo();
  writeUpgradeInfo();
  writeUnitSizeInfo();
  writeUnitCommandInfo();
  writeTechInfo();
  writeRaceInfo();
  writePlayerTypeInfo();
  writeOrderInfo();
  writeGameTypeInfo();
  writeExplosionTypeInfo();
  writeDamageTypeInfo();
  writeBulletTypeInfo();
  writeErrorInfo();
}
