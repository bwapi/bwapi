#include <gtest/gtest.h>

#include <BWAPI/WeaponType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitSizeType.h>

#include "UnitTypeFixtures.h"
#include "BW/DataEnv.h"

#include <string>
#include <iostream>
#include <utility>
#include <functional>

using namespace BWAPI;

std::string get_unitType_string(const testing::TestParamInfo<UnitType>& value) {
    std::string result = value.param.getName();
    if (result.empty()) {
        result = std::to_string(value.index);
    }
    return result;
}

TEST_P(UnitTypeFixture, maxHitPoints) {
    UnitType u = GetParam();
    ASSERT_EQ(u.maxHitPoints(), BW::units.hitPoints[u.getID()] / 256);
}

TEST_P(UnitTypeFixture, maxShields) {
    UnitType u = GetParam();
    ASSERT_EQ(u.maxShields(), BW::units.shieldEnable[u.getID()] ? BW::units.shields[u.getID()] : 0);
}

TEST_P(UnitTypeFixture, airWeapon) {
    UnitType u = GetParam();
    if (u == UnitTypes::Terran_Goliath) {
        ASSERT_EQ(u.airWeapon().getID(), BWAPI::WeaponTypes::Hellfire_Missile_Pack);
    }
    else if (u == UnitTypes::Hero_Alan_Schezar) {
        ASSERT_EQ(u.airWeapon().getID(), BWAPI::WeaponTypes::Hellfire_Missile_Pack_Alan_Schezar);
    }
    else {
        ASSERT_EQ(u.airWeapon().getID(), BW::units.airWeapon[u.getID()]);
    }
}

TEST_P(UnitTypeFixture, armor) {
    UnitType u = GetParam();
    ASSERT_EQ(u.armor(), BW::units.armorAmount[u.getID()]);
}

TEST_P(UnitTypeFixture, armorUpgrade) {
    UnitType u = GetParam();
    ASSERT_EQ(u.armorUpgrade().getID(), BW::units.armorUpgrade[u.getID()]);
}

TEST_P(UnitTypeFixture, buildScore) {
    UnitType u = GetParam();
    ASSERT_EQ(u.buildScore(), BW::units.buildScore[u.getID()]);
}

TEST_P(UnitTypeFixture, timeCost) {
    UnitType u = GetParam();
    ASSERT_EQ(u.buildTime(), BW::units.timeCost[u.getID()]);
}

TEST_P(UnitTypeFixture, destroyScore) {
    UnitType u = GetParam();
    ASSERT_EQ(u.destroyScore(), BW::units.destroyScore[u.getID()]);
}

TEST_P(UnitTypeFixture, dimensionLeft) {
    UnitType u = GetParam();
    ASSERT_EQ(u.dimensionLeft(), BW::units.dimensions[u.getID()].left);
}

TEST_P(UnitTypeFixture, dimensionUp) {
    UnitType u = GetParam();
    ASSERT_EQ(u.dimensionUp(), BW::units.dimensions[u.getID()].top);
}

TEST_P(UnitTypeFixture, dimensionRight) {
    UnitType u = GetParam();
    ASSERT_EQ(u.dimensionRight(), BW::units.dimensions[u.getID()].right);
}

TEST_P(UnitTypeFixture, dimensionDown) {
    UnitType u = GetParam();
    ASSERT_EQ(u.dimensionDown(), BW::units.dimensions[u.getID()].bottom);
}

TEST_P(UnitTypeFixture, gasPrice) {
    UnitType u = GetParam();
    ASSERT_EQ(u.gasPrice(), BW::units.gasCost[u.getID()]);
}

TEST_P(UnitTypeFixture, groundWeapon) {
    UnitType u = GetParam();
    if (BW::units.subunit1[u.getID()] != UnitTypes::Enum::None) {
        int i = BW::units.subunit1[u.getID()];
        u = UnitType(i);
    }
    ASSERT_EQ(u.groundWeapon().getID(), BW::units.groundWeapon[u.getID()]);
}

TEST_P(UnitTypeFixture, height) {
    UnitType u = GetParam();
    ASSERT_EQ(u.height(), BW::units.dimensions[u.getID()].top + BW::units.dimensions[u.getID()].bottom + 1);
}

TEST_P(UnitTypeFixture, maxAirHits) {
    UnitType u = GetParam();
    if (BW::units.subunit1[u.getID()] != UnitTypes::Enum::None) {
        int i = BW::units.subunit1[u.getID()];
        u = UnitType(i);
    }
    ASSERT_EQ(u.maxAirHits(), BW::units.maxAirHits[u.getID()]);
}

TEST_P(UnitTypeFixture, maxGroundHits) {
    UnitType u = GetParam();
    if (BW::units.subunit1[u.getID()] != UnitTypes::Enum::None) {
        int i = BW::units.subunit1[u.getID()];
        u = UnitType(i);
    }
    ASSERT_EQ(u.maxGroundHits(), BW::units.maxGroundHits[u.getID()]);

}

TEST_P(UnitTypeFixture, mineralPrice) {
    UnitType u = GetParam();
    ASSERT_EQ(u.mineralPrice(), BW::units.oreCost[u.getID()]);
}

TEST_P(UnitTypeFixture, seekRange) {
    UnitType u = GetParam();
    ASSERT_EQ(u.seekRange(), BW::units.targetAcquisitionRange[u.getID()] * 32);
}

TEST_P(UnitTypeFixture, sightRange) {
    UnitType u = GetParam();
    ASSERT_EQ(u.sightRange(), BW::units.sightRange[u.getID()] * 32);
}

TEST_P(UnitTypeFixture, size) {
    UnitType u = GetParam();
    ASSERT_EQ(u.size().getID(), BW::units.sizeType[u.getID()]);
}

TEST_P(UnitTypeFixture, spaceProvided) {
    UnitType u = GetParam();
    ASSERT_EQ(u.spaceProvided(), BW::units.spaceProvided[u.getID()]);
}

TEST_P(UnitTypeFixture, spaceRequired) {
    UnitType u = GetParam();
    ASSERT_EQ(u.spaceRequired(), BW::units.spaceRequired[u.getID()]);
}

TEST_P(UnitTypeFixture, supplyProvided) {
    UnitType u = GetParam();
    ASSERT_EQ(u.supplyProvided(), BW::units.supplyProvided[u.getID()]);
}

TEST_P(UnitTypeFixture, supplyRequired) {
    UnitType u = GetParam();
    ASSERT_EQ(u.supplyRequired(), BW::units.supplyRequired[u.getID()]);
}

INSTANTIATE_TEST_SUITE_P(
    UnitTypes,
    UnitTypeFixture,
    ::testing::ValuesIn(UnitTypeFixture::allInternalUnitTypes),
    get_unitType_string
);

TEST_P(NoneUnknownUnitTypeFixture, data) {
    ASSERT_EQ(GetParam().maxHitPoints(), 0);
    ASSERT_EQ(GetParam().maxShields(), 0);
    if (GetParam() == UnitTypes::Unknown) {
        ASSERT_EQ(GetParam().airWeapon(), 131);
    }
    else if (GetParam() == UnitTypes::None) {
        ASSERT_EQ(GetParam().airWeapon(), 130);
    }
    else if (GetParam() == UnitTypes::AllUnits) {
        ASSERT_EQ(GetParam().airWeapon(), 130);
    }
    else {
        ASSERT_EQ(GetParam().airWeapon(), 130);
    }
}

INSTANTIATE_TEST_SUITE_P(
    UnitTypes,
    NoneUnknownUnitTypeFixture,
    ::testing::ValuesIn(NoneUnknownUnitTypeFixture::allNoneUnknownTypes),
    get_unitType_string
);

TEST(Terran_Marine, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Marine.whatBuilds(), std::make_pair(UnitTypes::Terran_Barracks, 1));
}

TEST(Terran_Ghost, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Ghost.whatBuilds(), std::make_pair(UnitTypes::Terran_Barracks, 1));
}

TEST(Terran_Vulture, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Vulture.whatBuilds(), std::make_pair(UnitTypes::Terran_Factory, 1));
}

TEST(Terran_Siege_Tank_Tank_Mode, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Siege_Tank_Tank_Mode.whatBuilds(), std::make_pair(UnitTypes::Terran_Factory, 1));
}

TEST(Terran_SCV, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_SCV.whatBuilds(), std::make_pair(UnitTypes::Terran_Command_Center, 1));
}

TEST(Terran_Wraith, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Wraith.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Terran_Science_Vessel, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Science_Vessel.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Hero_Gui_Montag, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Gui_Montag.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Dropship, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Dropship.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Terran_Battlecruiser, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Battlecruiser.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Terran_Vulture_Spider_Mine, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Vulture_Spider_Mine.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Nuclear_Missile, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Nuclear_Missile.whatBuilds(), std::make_pair(UnitTypes::Terran_Nuclear_Silo, 1));
}

TEST(Terran_Civilian, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Civilian.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Sarah_Kerrigan, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Sarah_Kerrigan.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Alan_Schezar, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Alan_Schezar.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Jim_Raynor_Vulture, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Jim_Raynor_Vulture.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Jim_Raynor_Marine, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Jim_Raynor_Marine.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Tom_Kazansky, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Tom_Kazansky.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Magellan, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Magellan.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Edmund_Duke_Tank_Mode, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Edmund_Duke_Tank_Mode.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Edmund_Duke_Siege_Mode, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Edmund_Duke_Siege_Mode.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Arcturus_Mengsk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Arcturus_Mengsk.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Hyperion, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Hyperion.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Norad_II, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Norad_II.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Siege_Tank_Siege_Mode, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Siege_Tank_Siege_Mode.whatBuilds(), std::make_pair(UnitTypes::Terran_Factory, 1));
}

TEST(Terran_Firebat, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Firebat.whatBuilds(), std::make_pair(UnitTypes::Terran_Barracks, 1));
}

TEST(Spell_Scanner_Sweep, whatBuilds) {
    EXPECT_EQ(UnitTypes::Spell_Scanner_Sweep.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Medic, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Medic.whatBuilds(), std::make_pair(UnitTypes::Terran_Barracks, 1));
}

TEST(Zerg_Larva, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Larva.whatBuilds(), std::make_pair(UnitTypes::Zerg_Hatchery, 1));
}

TEST(Zerg_Egg, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Egg.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Zergling, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Zergling.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Hydralisk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Hydralisk.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Ultralisk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Ultralisk.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Broodling, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Broodling.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Drone, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Drone.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Overlord, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Overlord.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Mutalisk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Mutalisk.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Guardian, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Guardian.whatBuilds(), std::make_pair(UnitTypes::Zerg_Mutalisk, 1));
}

TEST(Zerg_Queen, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Queen.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Defiler, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Defiler.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Zerg_Scourge, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Scourge.whatBuilds(), std::make_pair(UnitTypes::Zerg_Larva, 1));
}

TEST(Hero_Torrasque, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Torrasque.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Matriarch, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Matriarch.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Infested_Terran, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Infested_Terran.whatBuilds(), std::make_pair(UnitTypes::Zerg_Infested_Command_Center, 1));
}

TEST(Hero_Infested_Kerrigan, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Infested_Kerrigan.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Unclean_One, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Unclean_One.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Hunter_Killer, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Hunter_Killer.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Devouring_One, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Devouring_One.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Kukulza_Mutalisk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Kukulza_Mutalisk.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Kukulza_Guardian, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Kukulza_Guardian.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Valkyrie, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Valkyrie.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Zerg_Cocoon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Cocoon.whatBuilds(), std::make_pair(UnitTypes::Zerg_Mutalisk, 1));
}

TEST(Protoss_Corsair, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Corsair.whatBuilds(), std::make_pair(UnitTypes::Protoss_Stargate, 1));
}

TEST(Protoss_Dark_Templar, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Dark_Templar.whatBuilds(), std::make_pair(UnitTypes::Protoss_Gateway, 1));
}

TEST(Zerg_Devourer, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Devourer.whatBuilds(), std::make_pair(UnitTypes::Zerg_Mutalisk, 1));
}

TEST(Protoss_Dark_Archon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Dark_Archon.whatBuilds(), std::make_pair(UnitTypes::Protoss_Dark_Templar, 2));
}

TEST(Protoss_Probe, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Probe.whatBuilds(), std::make_pair(UnitTypes::Protoss_Nexus, 1));
}

TEST(Protoss_Zealot, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Zealot.whatBuilds(), std::make_pair(UnitTypes::Protoss_Gateway, 1));
}

TEST(Protoss_Dragoon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Dragoon.whatBuilds(), std::make_pair(UnitTypes::Protoss_Gateway, 1));
}

TEST(Protoss_High_Templar, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_High_Templar.whatBuilds(), std::make_pair(UnitTypes::Protoss_Gateway, 1));
}

TEST(Protoss_Archon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Archon.whatBuilds(), std::make_pair(UnitTypes::Protoss_High_Templar, 2));
}

TEST(Protoss_Shuttle, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Shuttle.whatBuilds(), std::make_pair(UnitTypes::Protoss_Robotics_Facility, 1));
}

TEST(Protoss_Scout, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Scout.whatBuilds(), std::make_pair(UnitTypes::Protoss_Stargate, 1));
}

TEST(Protoss_Arbiter, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Arbiter.whatBuilds(), std::make_pair(UnitTypes::Protoss_Stargate, 1));
}

TEST(Protoss_Carrier, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Carrier.whatBuilds(), std::make_pair(UnitTypes::Protoss_Stargate, 1));
}

TEST(Protoss_Interceptor, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Interceptor.whatBuilds(), std::make_pair(UnitTypes::Protoss_Carrier, 1));
}

TEST(Hero_Dark_Templar, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Dark_Templar.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Zeratul, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Zeratul.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Tassadar_Zeratul_Archon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Tassadar_Zeratul_Archon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Fenix_Zealot, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Fenix_Zealot.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Fenix_Dragoon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Fenix_Dragoon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Tassadar, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Tassadar.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Mojo, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Mojo.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Warbringer, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Warbringer.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Gantrithor, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Gantrithor.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Protoss_Reaver, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Reaver.whatBuilds(), std::make_pair(UnitTypes::Protoss_Robotics_Facility, 1));
}

TEST(Protoss_Observer, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Observer.whatBuilds(), std::make_pair(UnitTypes::Protoss_Robotics_Facility, 1));
}

TEST(Protoss_Scarab, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Scarab.whatBuilds(), std::make_pair(UnitTypes::Protoss_Reaver, 1));
}

TEST(Hero_Danimoth, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Danimoth.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Aldaris, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Aldaris.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Artanis, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Artanis.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Rhynadon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Rhynadon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Bengalaas, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Bengalaas.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Cargo_Ship, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Cargo_Ship.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Mercenary_Gunship, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Mercenary_Gunship.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Scantid, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Scantid.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Kakaru, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Kakaru.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Ragnasaur, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Ragnasaur.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Critter_Ursadon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Critter_Ursadon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Lurker_Egg, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Lurker_Egg.whatBuilds(), std::make_pair(UnitTypes::Zerg_Hydralisk, 1));
}

TEST(Hero_Raszagal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Raszagal.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Samir_Duran, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Samir_Duran.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Alexei_Stukov, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Alexei_Stukov.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Map_Revealer, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Map_Revealer.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Hero_Gerard_DuGalle, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Gerard_DuGalle.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Lurker, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Lurker.whatBuilds(), std::make_pair(UnitTypes::Zerg_Hydralisk, 1));
}

TEST(Hero_Infested_Duran, whatBuilds) {
    EXPECT_EQ(UnitTypes::Hero_Infested_Duran.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Spell_Disruption_Web, whatBuilds) {
    EXPECT_EQ(UnitTypes::Spell_Disruption_Web.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Terran_Command_Center, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Command_Center.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Comsat_Station, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Comsat_Station.whatBuilds(), std::make_pair(UnitTypes::Terran_Command_Center, 1));
}

TEST(Terran_Nuclear_Silo, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Nuclear_Silo.whatBuilds(), std::make_pair(UnitTypes::Terran_Command_Center, 1));
}

TEST(Terran_Supply_Depot, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Supply_Depot.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Refinery, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Refinery.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Barracks, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Barracks.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Academy, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Academy.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Factory, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Factory.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Starport, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Starport.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Control_Tower, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Control_Tower.whatBuilds(), std::make_pair(UnitTypes::Terran_Starport, 1));
}

TEST(Terran_Science_Facility, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Science_Facility.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Covert_Ops, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Covert_Ops.whatBuilds(), std::make_pair(UnitTypes::Terran_Science_Facility, 1));
}

TEST(Terran_Physics_Lab, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Physics_Lab.whatBuilds(), std::make_pair(UnitTypes::Terran_Science_Facility, 1));
}

TEST(Terran_Machine_Shop, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Machine_Shop.whatBuilds(), std::make_pair(UnitTypes::Terran_Factory, 1));
}

TEST(Terran_Engineering_Bay, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Engineering_Bay.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Armory, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Armory.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Missile_Turret, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Missile_Turret.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Terran_Bunker, whatBuilds) {
    EXPECT_EQ(UnitTypes::Terran_Bunker.whatBuilds(), std::make_pair(UnitTypes::Terran_SCV, 1));
}

TEST(Special_Crashed_Norad_II, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Crashed_Norad_II.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Ion_Cannon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Ion_Cannon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Uraj_Crystal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Uraj_Crystal.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Khalis_Crystal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Khalis_Crystal.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Infested_Command_Center, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Infested_Command_Center.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Hatchery, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Hatchery.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Lair, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Lair.whatBuilds(), std::make_pair(UnitTypes::Zerg_Hatchery, 1));
}

TEST(Zerg_Hive, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Hive.whatBuilds(), std::make_pair(UnitTypes::Zerg_Lair, 1));
}

TEST(Zerg_Nydus_Canal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Nydus_Canal.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Hydralisk_Den, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Hydralisk_Den.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Defiler_Mound, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Defiler_Mound.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Queens_Nest, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Queens_Nest.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Evolution_Chamber, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Evolution_Chamber.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Ultralisk_Cavern, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Ultralisk_Cavern.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Spire, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Spire.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Spawning_Pool, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Spawning_Pool.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Creep_Colony, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Creep_Colony.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Zerg_Spore_Colony, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Spore_Colony.whatBuilds(), std::make_pair(UnitTypes::Zerg_Creep_Colony, 1));
}

TEST(Zerg_Sunken_Colony, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Sunken_Colony.whatBuilds(), std::make_pair(UnitTypes::Zerg_Creep_Colony, 1));
}

TEST(Special_Overmind_With_Shell, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Overmind_With_Shell.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Overmind, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Overmind.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Zerg_Extractor, whatBuilds) {
    EXPECT_EQ(UnitTypes::Zerg_Extractor.whatBuilds(), std::make_pair(UnitTypes::Zerg_Drone, 1));
}

TEST(Special_Mature_Chrysalis, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Mature_Chrysalis.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Cerebrate, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Cerebrate.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Cerebrate_Daggoth, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Cerebrate_Daggoth.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Protoss_Nexus, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Nexus.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Robotics_Facility, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Robotics_Facility.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Pylon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Pylon.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Assimilator, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Assimilator.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Observatory, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Observatory.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Gateway, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Gateway.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Photon_Cannon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Photon_Cannon.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Citadel_of_Adun, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Citadel_of_Adun.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Cybernetics_Core, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Cybernetics_Core.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Templar_Archives, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Templar_Archives.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Forge, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Forge.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Stargate, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Stargate.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Special_Stasis_Cell_Prison, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Stasis_Cell_Prison.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Protoss_Fleet_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Fleet_Beacon.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Arbiter_Tribunal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Arbiter_Tribunal.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Robotics_Support_Bay, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Robotics_Support_Bay.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Protoss_Shield_Battery, whatBuilds) {
    EXPECT_EQ(UnitTypes::Protoss_Shield_Battery.whatBuilds(), std::make_pair(UnitTypes::Protoss_Probe, 1));
}

TEST(Special_Khaydarin_Crystal_Form, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Khaydarin_Crystal_Form.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Protoss_Temple, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Protoss_Temple.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_XelNaga_Temple, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_XelNaga_Temple.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Resource_Mineral_Field, whatBuilds) {
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Resource_Mineral_Field_Type_2, whatBuilds) {
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field_Type_2.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Resource_Mineral_Field_Type_3, whatBuilds) {
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field_Type_3.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Independant_Starport, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Independant_Starport.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Resource_Vespene_Geyser, whatBuilds) {
    EXPECT_EQ(UnitTypes::Resource_Vespene_Geyser.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Psi_Disrupter, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Psi_Disrupter.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Zerg_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Zerg_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Terran_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Terran_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Protoss_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Protoss_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Zerg_Flag_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Zerg_Flag_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Terran_Flag_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Terran_Flag_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Protoss_Flag_Beacon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Protoss_Flag_Beacon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Power_Generator, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Power_Generator.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Overmind_Cocoon, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Overmind_Cocoon.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Spell_Dark_Swarm, whatBuilds) {
    EXPECT_EQ(UnitTypes::Spell_Dark_Swarm.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Floor_Missile_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Floor_Missile_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Floor_Hatch, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Floor_Hatch.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Upper_Level_Door, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Upper_Level_Door.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Right_Upper_Level_Door, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Right_Upper_Level_Door.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Pit_Door, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Pit_Door.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Right_Pit_Door, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Right_Pit_Door.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Floor_Gun_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Floor_Gun_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Wall_Missile_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Wall_Missile_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Wall_Flame_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Wall_Flame_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Right_Wall_Missile_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Right_Wall_Missile_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Right_Wall_Flame_Trap, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Right_Wall_Flame_Trap.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Special_Start_Location, whatBuilds) {
    EXPECT_EQ(UnitTypes::Special_Start_Location.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Flag, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Flag.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Young_Chrysalis, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Young_Chrysalis.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Psi_Emitter, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Psi_Emitter.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Data_Disk, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Data_Disk.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Khaydarin_Crystal, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Khaydarin_Crystal.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Mineral_Cluster_Type_1, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Mineral_Cluster_Type_1.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Mineral_Cluster_Type_2, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Mineral_Cluster_Type_2.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Protoss_Gas_Orb_Type_1, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Protoss_Gas_Orb_Type_1.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Protoss_Gas_Orb_Type_2, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Protoss_Gas_Orb_Type_2.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Zerg_Gas_Sac_Type_1, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Zerg_Gas_Sac_Type_1.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Zerg_Gas_Sac_Type_2, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Zerg_Gas_Sac_Type_2.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Terran_Gas_Tank_Type_1, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Terran_Gas_Tank_Type_1.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Powerup_Terran_Gas_Tank_Type_2, whatBuilds) {
    EXPECT_EQ(UnitTypes::Powerup_Terran_Gas_Tank_Type_2.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(None, whatBuilds) {
    EXPECT_EQ(UnitTypes::None.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(AllUnits, whatBuilds) {
    EXPECT_EQ(UnitTypes::AllUnits.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Men, whatBuilds) {
    EXPECT_EQ(UnitTypes::Men.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Buildings, whatBuilds) {
    EXPECT_EQ(UnitTypes::Buildings.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Factories, whatBuilds) {
    EXPECT_EQ(UnitTypes::Factories.whatBuilds(), std::make_pair(UnitTypes::None, 0));
}

TEST(Unknown, whatBuilds) {
    EXPECT_EQ(UnitTypes::Unknown.whatBuilds(), std::make_pair(UnitTypes::Unknown, 1));
}
