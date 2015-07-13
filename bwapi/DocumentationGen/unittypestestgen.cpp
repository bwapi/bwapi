#include "helpers.h"

void genUnitTypeTest()
{
  std::ofstream of("testUnitTypes.txt");
  of.setf(std::ios::showpoint);
  of.setf(std::ios::boolalpha);
  std::set<BWAPI::UnitType> types(BWAPI::UnitTypes::allUnitTypes().begin(), BWAPI::UnitTypes::allUnitTypes().end());
  for (auto t : types)
  {
    of << tab(2) << "TEST_METHOD(UnitType" << t << ")\n";
    of << tab(2) << "{\n";
    of << tab(3) << "auto t = BWAPI::UnitTypes::" << t << ";\n";
    of << tab(3) << "Assert_enum(" << t << ");\n\n";

#define ASS_EQ(n) of << tab(3) << "Assert_" #n "(" << t.n() << ");\n"

    ASS_EQ(getName);
    of << tab(3) << "Assert::AreEqual(t.c_str(), t.getName().c_str());\n";
    ASS_EQ(getRace);
    of << tab(3) << "Assert_whatBuilds(" << t.whatBuilds().first << ", " << t.whatBuilds().second << ");\n";
    of << tab(3) << "AssertSetEquals(" << mapToString(t.requiredUnits(), "BWAPI::UnitTypes::") << ", t.requiredUnits());\n";
    ASS_EQ(requiredTech);
    ASS_EQ(cloakingTech);
    of << tab(3) << "AssertSetEquals(" << setToString(t.abilities(), "BWAPI::TechTypes::") << ", t.abilities());\n";
    of << tab(3) << "AssertSetEquals(" << setToString(t.upgrades(), "BWAPI::UpgradeTypes::") << ", t.upgrades());\n";
    ASS_EQ(armorUpgrade);
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
    ASS_EQ(size);
    ASS_EQ(tileWidth);
    ASS_EQ(tileHeight);
    of << tab(3) << "Assert_tileSize(BWAPI::TilePosition" << t.tileSize() << ");\n";
    of << tab(3) << "Assert::AreEqual(BWAPI::TilePosition(t.tileWidth(), t.tileHeight()), t.tileSize());\n";
    ASS_EQ(dimensionLeft);
    ASS_EQ(dimensionUp);
    ASS_EQ(dimensionRight);
    ASS_EQ(dimensionDown);
    ASS_EQ(width);
    of << tab(3) << "Assert::AreEqual(t.dimensionLeft() + 1 + t.dimensionRight(), t.width());\n";
    ASS_EQ(height);
    of << tab(3) << "Assert::AreEqual(t.dimensionUp() + 1 + t.dimensionDown(), t.height());\n";
    ASS_EQ(seekRange);
    ASS_EQ(sightRange);
    ASS_EQ(groundWeapon);
    ASS_EQ(maxGroundHits);
    ASS_EQ(airWeapon);
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
    ASS_EQ(producesCreep);
    ASS_EQ(producesLarva);
    ASS_EQ(isMineralField);
    ASS_EQ(isCritter);
    ASS_EQ(canBuildAddon);
    of << tab(3) << "AssertSetEquals(" << setToString(t.buildsWhat(), "BWAPI::UnitTypes::") << ", t.buildsWhat());\n";
    of << tab(3) << "AssertSetEquals(" << setToString(t.researchesWhat(), "BWAPI::TechTypes::") << ", t.researchesWhat());\n";
    of << tab(3) << "AssertSetEquals(" << setToString(t.upgradesWhat(), "BWAPI::UpgradeTypes::") << ", t.upgradesWhat());\n";

    of << tab(2) << "}\n";
  }
}
