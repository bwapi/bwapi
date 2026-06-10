#include <gtest/gtest.h>

#include <BWAPI/UnitType.h>
#include <BWAPI/Race.h>

#include <iostream>
#include <utility>
#include <functional>

using namespace BWAPI;


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

TEST(Terran_Marine, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Barracks, 1} };
    EXPECT_EQ(UnitTypes::Terran_Marine.requiredUnits(), rUnitMap);
}

TEST(Terran_Ghost, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Barracks, 1},  {UnitTypes::Terran_Academy, 1}, {UnitTypes::Terran_Covert_Ops, 1} };
    EXPECT_EQ(UnitTypes::Terran_Ghost.requiredUnits(), rUnitMap);
}

TEST(Terran_Vulture, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1} };
    EXPECT_EQ(UnitTypes::Terran_Vulture.requiredUnits(), rUnitMap);
}

TEST(Terran_Goliath, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1}, {UnitTypes::Terran_Armory, 1} };
    EXPECT_EQ(UnitTypes::Terran_Goliath.requiredUnits(), rUnitMap);
}

TEST(Terran_Siege_Tank_Tank_Mode, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1}, {UnitTypes::Terran_Machine_Shop, 1} };
    EXPECT_EQ(UnitTypes::Terran_Siege_Tank_Tank_Mode.requiredUnits(), rUnitMap);
}

TEST(Terran_SCV, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Command_Center, 1}};
    EXPECT_EQ(UnitTypes::Terran_SCV.requiredUnits(), rUnitMap);
}

TEST(Terran_Wraith, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1} };
    EXPECT_EQ(UnitTypes::Terran_Wraith.requiredUnits(), rUnitMap);
}

TEST(Terran_Science_Vessel, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1}, {UnitTypes::Terran_Control_Tower, 1},{UnitTypes::Terran_Science_Facility, 1} };
    EXPECT_EQ(UnitTypes::Terran_Science_Vessel.requiredUnits(), rUnitMap);
}

TEST(Hero_Gui_Montag, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Gui_Montag.requiredUnits(), rUnitMap);
}

TEST(Terran_Dropship, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1}, {UnitTypes::Terran_Control_Tower, 1} };
    EXPECT_EQ(UnitTypes::Terran_Dropship.requiredUnits(), rUnitMap);
}

TEST(Terran_Battlecruiser, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1}, {UnitTypes::Terran_Control_Tower, 1}, {UnitTypes::Terran_Physics_Lab, 1} };
    EXPECT_EQ(UnitTypes::Terran_Battlecruiser.requiredUnits(), rUnitMap);
}

TEST(Terran_Vulture_Spider_Mine, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Terran_Vulture_Spider_Mine.requiredUnits(), rUnitMap);
}

TEST(Terran_Nuclear_Missile, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Nuclear_Silo, 1}, };
    EXPECT_EQ(UnitTypes::Terran_Nuclear_Missile.requiredUnits(), rUnitMap);
}

TEST(Terran_Civilian, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Terran_Civilian.requiredUnits(), rUnitMap);
}

TEST(Hero_Sarah_Kerrigan, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Terran_Civilian.requiredUnits(), rUnitMap);
}

TEST(Hero_Alan_Schezar, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Terran_Civilian.requiredUnits(), rUnitMap);
}

TEST(Hero_Jim_Raynor_Vulture, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Jim_Raynor_Vulture.requiredUnits(), rUnitMap);
}

TEST(Hero_Jim_Raynor_Marine, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Jim_Raynor_Marine.requiredUnits(), rUnitMap);
}

TEST(Hero_Tom_Kazansky, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Tom_Kazansky.requiredUnits(), rUnitMap);
}

TEST(Hero_Magellan, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Magellan.requiredUnits(), rUnitMap);
}

TEST(Hero_Edmund_Duke_Tank_Mode, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Edmund_Duke_Tank_Mode.requiredUnits(), rUnitMap);
}

TEST(Hero_Edmund_Duke_Siege_Mode, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Edmund_Duke_Siege_Mode.requiredUnits(), rUnitMap);
}

TEST(Hero_Arcturus_Mengsk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Arcturus_Mengsk.requiredUnits(), rUnitMap);
}

TEST(Hero_Hyperion, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Hyperion.requiredUnits(), rUnitMap);
}

TEST(Hero_Norad_II, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Norad_II.requiredUnits(), rUnitMap);
}

TEST(Terran_Siege_Tank_Siege_Mode, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1},  {UnitTypes::Terran_Machine_Shop, 1} };
    EXPECT_EQ(UnitTypes::Terran_Siege_Tank_Siege_Mode.requiredUnits(), rUnitMap);
}

TEST(Terran_Firebat, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Barracks, 1},  {UnitTypes::Terran_Academy, 1} };
    EXPECT_EQ(UnitTypes::Terran_Firebat.requiredUnits(), rUnitMap);
}

TEST(Spell_Scanner_Sweep, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Spell_Scanner_Sweep.requiredUnits(), rUnitMap);
}

TEST(Terran_Medic, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Barracks, 1},  {UnitTypes::Terran_Academy, 1} };
    EXPECT_EQ(UnitTypes::Terran_Medic.requiredUnits(), rUnitMap);
}

TEST(Zerg_Larva, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Hatchery, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Larva.requiredUnits(), rUnitMap);
}

TEST(Zerg_Egg, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}};
    EXPECT_EQ(UnitTypes::Zerg_Egg.requiredUnits(), rUnitMap);
}

TEST(Zerg_Zergling, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Spawning_Pool, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Zergling.requiredUnits(), rUnitMap);
}

TEST(Zerg_Hydralisk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Hydralisk_Den, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Hydralisk.requiredUnits(), rUnitMap);
}

TEST(Zerg_Ultralisk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Ultralisk_Cavern, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Ultralisk.requiredUnits(), rUnitMap);
}

TEST(Zerg_Broodling, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Zerg_Broodling.requiredUnits(), rUnitMap);
}

TEST(Zerg_Drone, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Drone.requiredUnits(), rUnitMap);
}

TEST(Zerg_Overlord, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Overlord.requiredUnits(), rUnitMap);
}

TEST(Zerg_Mutalisk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Spire, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Mutalisk.requiredUnits(), rUnitMap);
}

TEST(Zerg_Guardian, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Mutalisk, 1}, {UnitTypes::Zerg_Greater_Spire, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Guardian.requiredUnits(), rUnitMap);
}

TEST(Zerg_Queen, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Queens_Nest, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Queen.requiredUnits(), rUnitMap);
}

TEST(Zerg_Defiler, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Defiler_Mound, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Defiler.requiredUnits(), rUnitMap);
}

TEST(Zerg_Scourge, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Larva, 1}, {UnitTypes::Zerg_Spire, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Scourge.requiredUnits(), rUnitMap);
}

TEST(Hero_Torrasque, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Torrasque.requiredUnits(), rUnitMap);
}

TEST(Hero_Matriarch, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Matriarch.requiredUnits(), rUnitMap);
}

TEST(Zerg_Infested_Terran, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Infested_Command_Center, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Infested_Terran.requiredUnits(), rUnitMap);
}

TEST(Hero_Infested_Kerrigan, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Infested_Kerrigan.requiredUnits(), rUnitMap);
}

TEST(Hero_Unclean_One, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Unclean_One.requiredUnits(), rUnitMap);
}

TEST(Hero_Hunter_Killer, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Hunter_Killer.requiredUnits(), rUnitMap);
}

TEST(Hero_Devouring_One, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Devouring_One.requiredUnits(), rUnitMap);
}

TEST(Hero_Kukulza_Mutalisk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Kukulza_Mutalisk.requiredUnits(), rUnitMap);
}

TEST(Hero_Kukulza_Guardian, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Kukulza_Guardian.requiredUnits(), rUnitMap);
}

TEST(Hero_Yggdrasill, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Yggdrasill.requiredUnits(), rUnitMap);
}

TEST(Terran_Valkyrie, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1}, {UnitTypes::Terran_Control_Tower, 1}, {UnitTypes::Terran_Armory, 1} };
    EXPECT_EQ(UnitTypes::Terran_Valkyrie.requiredUnits(), rUnitMap);
}

TEST(Zerg_Cocoon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Greater_Spire, 1}, {UnitTypes::Zerg_Mutalisk, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Cocoon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Corsair, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Stargate, 1}};
    EXPECT_EQ(UnitTypes::Protoss_Corsair.requiredUnits(), rUnitMap);
}

TEST(Protoss_Dark_Templar, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Gateway, 1} ,{UnitTypes::Protoss_Templar_Archives, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Dark_Templar.requiredUnits(), rUnitMap);
}

TEST(Zerg_Devourer, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Mutalisk, 1} ,{UnitTypes::Zerg_Greater_Spire, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Devourer.requiredUnits(), rUnitMap);
}

TEST(Protoss_Dark_Archon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Dark_Templar, 2}};
    EXPECT_EQ(UnitTypes::Protoss_Dark_Archon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Probe, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Nexus, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Probe.requiredUnits(), rUnitMap);
}

TEST(Protoss_Zealot, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Gateway, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Zealot.requiredUnits(), rUnitMap);
}

TEST(Protoss_Dragoon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Gateway, 1}, {UnitTypes::Protoss_Cybernetics_Core, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Dragoon.requiredUnits(), rUnitMap);
}

TEST(Protoss_High_Templar, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Gateway, 1}, {UnitTypes::Protoss_Templar_Archives, 1} };
    EXPECT_EQ(UnitTypes::Protoss_High_Templar.requiredUnits(), rUnitMap);
}

TEST(Protoss_Archon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_High_Templar, 2}};
    EXPECT_EQ(UnitTypes::Protoss_Archon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Shuttle, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Robotics_Facility, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Shuttle.requiredUnits(), rUnitMap);
}

TEST(Protoss_Scout, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Stargate, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Scout.requiredUnits(), rUnitMap);
}

TEST(Protoss_Arbiter, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Stargate, 1},  {UnitTypes::Protoss_Arbiter_Tribunal, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Arbiter.requiredUnits(), rUnitMap);
}

TEST(Protoss_Carrier, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Stargate, 1},  {UnitTypes::Protoss_Fleet_Beacon, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Carrier.requiredUnits(), rUnitMap);
}

TEST(Protoss_Interceptor, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Carrier, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Interceptor.requiredUnits(), rUnitMap);
}

TEST(Hero_Dark_Templar, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Dark_Templar.requiredUnits(), rUnitMap);
}

TEST(Hero_Zeratul, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Zeratul.requiredUnits(), rUnitMap);
}

TEST(Hero_Tassadar_Zeratul_Archon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Tassadar_Zeratul_Archon.requiredUnits(), rUnitMap);
}

TEST(Hero_Fenix_Zealot, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Fenix_Zealot.requiredUnits(), rUnitMap);
}

TEST(Hero_Fenix_Dragoon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Fenix_Dragoon.requiredUnits(), rUnitMap);
}

TEST(Hero_Tassadar, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Tassadar.requiredUnits(), rUnitMap);
}

TEST(Hero_Mojo, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Mojo.requiredUnits(), rUnitMap);
}

TEST(Hero_Warbringer, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Warbringer.requiredUnits(), rUnitMap);
}

TEST(Hero_Gantrithor, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Hero_Gantrithor.requiredUnits(), rUnitMap);
}

TEST(Protoss_Reaver, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Robotics_Facility, 1}, {UnitTypes::Protoss_Robotics_Support_Bay, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Reaver.requiredUnits(), rUnitMap);
}

TEST(Protoss_Observer, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Robotics_Facility, 1}, {UnitTypes::Protoss_Observatory, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Observer.requiredUnits(), rUnitMap);
}

TEST(Protoss_Scarab, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Reaver, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Scarab.requiredUnits(), rUnitMap);
}

TEST(Hero_Danimoth, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Danimoth.requiredUnits(), rUnitMap);
}

TEST(Hero_Aldaris, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Danimoth.requiredUnits(), rUnitMap);
}

TEST(Hero_Artanis, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Danimoth.requiredUnits(), rUnitMap);
}

TEST(Critter_Rhynadon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Rhynadon.requiredUnits(), rUnitMap);
}

TEST(Critter_Bengalaas, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Bengalaas.requiredUnits(), rUnitMap);
}

TEST(Special_Cargo_Ship, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Cargo_Ship.requiredUnits(), rUnitMap);
}

TEST(Special_Mercenary_Gunship, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Mercenary_Gunship.requiredUnits(), rUnitMap);
}

TEST(Critter_Scantid, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Scantid.requiredUnits(), rUnitMap);
}

TEST(Critter_Kakaru, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Kakaru.requiredUnits(), rUnitMap);
}

TEST(Critter_Ragnasaur, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Ragnasaur.requiredUnits(), rUnitMap);
}

TEST(Critter_Ursadon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Critter_Ursadon.requiredUnits(), rUnitMap);
}

TEST(Zerg_Lurker_Egg, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Hydralisk, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Lurker_Egg.requiredUnits(), rUnitMap);
}

TEST(Hero_Raszagal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Raszagal.requiredUnits(), rUnitMap);
}

TEST(Hero_Samir_Duran, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Samir_Duran.requiredUnits(), rUnitMap);
}

TEST(Hero_Alexei_Stukov, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Alexei_Stukov.requiredUnits(), rUnitMap);
}

TEST(Special_Map_Revealer, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Map_Revealer.requiredUnits(), rUnitMap);
}

TEST(Hero_Gerard_DuGalle, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Gerard_DuGalle.requiredUnits(), rUnitMap);
}

TEST(Zerg_Lurker, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Hydralisk,1} };
    EXPECT_EQ(UnitTypes::Zerg_Lurker.requiredUnits(), rUnitMap);
}

TEST(Hero_Infested_Duran, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Hero_Infested_Duran.requiredUnits(), rUnitMap);
}

TEST(Spell_Disruption_Web, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Spell_Disruption_Web.requiredUnits(), rUnitMap);
}

TEST(Terran_Command_Center, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Command_Center.requiredUnits(), rUnitMap);
}

TEST(Terran_Comsat_Station, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Command_Center, 1}, {UnitTypes::Terran_Academy, 1} };
    EXPECT_EQ(UnitTypes::Terran_Comsat_Station.requiredUnits(), rUnitMap);
}

TEST(Terran_Nuclear_Silo, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Command_Center, 1}, {UnitTypes::Terran_Covert_Ops, 1}, {UnitTypes::Terran_Science_Facility, 1} };
    EXPECT_EQ(UnitTypes::Terran_Nuclear_Silo.requiredUnits(), rUnitMap);
}

TEST(Terran_Supply_Depot, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1}  };
    EXPECT_EQ(UnitTypes::Terran_Supply_Depot.requiredUnits(), rUnitMap);
}

TEST(Terran_Refinery, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Refinery.requiredUnits(), rUnitMap);
}

TEST(Terran_Barracks, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1},  {UnitTypes::Terran_Command_Center, 1} };
    EXPECT_EQ(UnitTypes::Terran_Barracks.requiredUnits(), rUnitMap);
}

TEST(Terran_Academy, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1}, {UnitTypes::Terran_Barracks, 1} };
    EXPECT_EQ(UnitTypes::Terran_Academy.requiredUnits(), rUnitMap);
}

TEST(Terran_Factory, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1}, {UnitTypes::Terran_Barracks, 1} };
    EXPECT_EQ(UnitTypes::Terran_Factory.requiredUnits(), rUnitMap);
}

TEST(Terran_Starport, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1}, {UnitTypes::Terran_Factory, 1} };
    EXPECT_EQ(UnitTypes::Terran_Starport.requiredUnits(), rUnitMap);
}

TEST(Terran_Control_Tower, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Starport, 1}};
    EXPECT_EQ(UnitTypes::Terran_Control_Tower.requiredUnits(), rUnitMap);
}

TEST(Terran_Science_Facility, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_SCV, 1}, {UnitTypes::Terran_Starport, 1} };
    EXPECT_EQ(UnitTypes::Terran_Science_Facility.requiredUnits(), rUnitMap);
}

TEST(Terran_Covert_Ops, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Science_Facility, 1}};
    EXPECT_EQ(UnitTypes::Terran_Covert_Ops.requiredUnits(), rUnitMap);
}

TEST(Terran_Physics_Lab, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Science_Facility, 1} };
    EXPECT_EQ(UnitTypes::Terran_Physics_Lab.requiredUnits(), rUnitMap);
}

TEST(Terran_Machine_Shop, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1} };
    EXPECT_EQ(UnitTypes::Terran_Machine_Shop.requiredUnits(), rUnitMap);
}


TEST(Terran_Engineering_Bay, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Command_Center, 1} ,{UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Engineering_Bay.requiredUnits(), rUnitMap);
}

TEST(Terran_Armory, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Factory, 1} ,{UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Armory.requiredUnits(), rUnitMap);
}

TEST(Terran_Missile_Turret, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Engineering_Bay, 1} ,{UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Missile_Turret.requiredUnits(), rUnitMap);
}

TEST(Terran_Bunker, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Terran_Barracks, 1} ,{UnitTypes::Terran_SCV, 1} };
    EXPECT_EQ(UnitTypes::Terran_Bunker.requiredUnits(), rUnitMap);
}

TEST(Special_Crashed_Norad_II, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Special_Crashed_Norad_II.requiredUnits(), rUnitMap);
}

TEST(Special_Ion_Cannon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Special_Ion_Cannon.requiredUnits(), rUnitMap);
}

TEST(Powerup_Uraj_Crystal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Powerup_Uraj_Crystal.requiredUnits(), rUnitMap);
}

TEST(Powerup_Khalis_Crystal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Powerup_Khalis_Crystal.requiredUnits(), rUnitMap);
}

TEST(Zerg_Infested_Command_Center, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Zerg_Infested_Command_Center.requiredUnits(), rUnitMap);
}

TEST(Zerg_Hatchery, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}};
    EXPECT_EQ(UnitTypes::Zerg_Hatchery.requiredUnits(), rUnitMap);
}

TEST(Zerg_Lair, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Hatchery, 1}, {UnitTypes::Zerg_Spawning_Pool, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Lair.requiredUnits(), rUnitMap);
}

TEST(Zerg_Hive, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Lair, 1}, {UnitTypes::Zerg_Queens_Nest, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Hive.requiredUnits(), rUnitMap);
}

TEST(Zerg_Nydus_Canal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Hive, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Nydus_Canal.requiredUnits(), rUnitMap);
}

TEST(Zerg_Hydralisk_Den, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Spawning_Pool, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Hydralisk_Den.requiredUnits(), rUnitMap);
}

TEST(Zerg_Defiler_Mound, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Hive, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Defiler_Mound.requiredUnits(), rUnitMap);
}

TEST(Zerg_Greater_Spire, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Spire, 1}, {UnitTypes::Zerg_Hive, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Greater_Spire.requiredUnits(), rUnitMap);
}

TEST(Zerg_Queens_Nest, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Lair, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Queens_Nest.requiredUnits(), rUnitMap);
}

TEST(Zerg_Evolution_Chamber, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Hatchery, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Evolution_Chamber.requiredUnits(), rUnitMap);
}

TEST(Zerg_Ultralisk_Cavern, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Hive, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Ultralisk_Cavern.requiredUnits(), rUnitMap);
}

TEST(Zerg_Spire, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Lair, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Spire.requiredUnits(), rUnitMap);
}

TEST(Zerg_Spawning_Pool, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}, {UnitTypes::Zerg_Hatchery, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Spawning_Pool.requiredUnits(), rUnitMap);
}

TEST(Zerg_Creep_Colony, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1}};
    EXPECT_EQ(UnitTypes::Zerg_Creep_Colony.requiredUnits(), rUnitMap);
}

TEST(Zerg_Spore_Colony, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Creep_Colony, 1}, {UnitTypes::Zerg_Evolution_Chamber, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Spore_Colony.requiredUnits(), rUnitMap);
}

TEST(Zerg_Sunken_Colony, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Creep_Colony, 1}, {UnitTypes::Zerg_Spawning_Pool, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Sunken_Colony.requiredUnits(), rUnitMap);
}

TEST(Special_Overmind_With_Shell, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Overmind_With_Shell.requiredUnits(), rUnitMap);
}

TEST(Special_Overmind, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Overmind.requiredUnits(), rUnitMap);
}

TEST(Zerg_Extractor, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Zerg_Drone, 1} };
    EXPECT_EQ(UnitTypes::Zerg_Extractor.requiredUnits(), rUnitMap);
}

TEST(Special_Mature_Chrysalis, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Mature_Chrysalis.requiredUnits(), rUnitMap);
}

TEST(Special_Cerebrate, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Cerebrate.requiredUnits(), rUnitMap);
}

TEST(Special_Cerebrate_Daggoth, requiredUnits) {
    std::map<UnitType, int> rUnitMap{};
    EXPECT_EQ(UnitTypes::Special_Cerebrate_Daggoth.requiredUnits(), rUnitMap);
}

TEST(Protoss_Nexus, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Nexus.requiredUnits(), rUnitMap);
}

TEST(Protoss_Robotics_Facility, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1}, {UnitTypes::Protoss_Cybernetics_Core, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Robotics_Facility.requiredUnits(), rUnitMap);
}

TEST(Protoss_Pylon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Pylon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Assimilator, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Assimilator.requiredUnits(), rUnitMap);
}

TEST(Protoss_Observatory, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Robotics_Facility, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Observatory.requiredUnits(), rUnitMap);
}

TEST(Protoss_Gateway, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1}, {UnitTypes::Protoss_Nexus, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Gateway.requiredUnits(), rUnitMap);
}

TEST(Protoss_Photon_Cannon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Forge, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Photon_Cannon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Citadel_of_Adun, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Cybernetics_Core, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Citadel_of_Adun.requiredUnits(), rUnitMap);
}

TEST(Protoss_Cybernetics_Core, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Gateway, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Cybernetics_Core.requiredUnits(), rUnitMap);
}

TEST(Protoss_Templar_Archives, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Citadel_of_Adun, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Templar_Archives.requiredUnits(), rUnitMap);
}

TEST(Protoss_Forge, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Nexus, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Forge.requiredUnits(), rUnitMap);
}

TEST(Protoss_Stargate, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Cybernetics_Core, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Stargate.requiredUnits(), rUnitMap);
}

TEST(Special_Stasis_Cell_Prison, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ };
    EXPECT_EQ(UnitTypes::Special_Stasis_Cell_Prison.requiredUnits(), rUnitMap);
}

TEST(Protoss_Fleet_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Stargate, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Fleet_Beacon.requiredUnits(), rUnitMap);
}

TEST(Protoss_Arbiter_Tribunal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Stargate, 1},  {UnitTypes::Protoss_Templar_Archives, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Arbiter_Tribunal.requiredUnits(), rUnitMap);
}

TEST(Protoss_Robotics_Support_Bay, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Robotics_Facility, 1}};
    EXPECT_EQ(UnitTypes::Protoss_Robotics_Support_Bay.requiredUnits(), rUnitMap);
}

TEST(Protoss_Shield_Battery, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Protoss_Probe, 1},  {UnitTypes::Protoss_Gateway, 1} };
    EXPECT_EQ(UnitTypes::Protoss_Shield_Battery.requiredUnits(), rUnitMap);
}

TEST(Special_Khaydarin_Crystal_Form, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Khaydarin_Crystal_Form.requiredUnits(), rUnitMap);
}

TEST(Special_Protoss_Temple, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Protoss_Temple.requiredUnits(), rUnitMap);
}

TEST(Special_XelNaga_Temple, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_XelNaga_Temple.requiredUnits(), rUnitMap);
}

TEST(Resource_Mineral_Field, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field.requiredUnits(), rUnitMap);
}

TEST(Resource_Mineral_Field_Type_2, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field_Type_2.requiredUnits(), rUnitMap);
}

TEST(Resource_Mineral_Field_Type_3, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Resource_Mineral_Field_Type_3.requiredUnits(), rUnitMap);
}

TEST(Special_Independant_Starport, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Independant_Starport.requiredUnits(), rUnitMap);
}

TEST(Resource_Vespene_Geyser, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Resource_Vespene_Geyser.requiredUnits(), rUnitMap);
}

TEST(Special_Warp_Gate, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Warp_Gate.requiredUnits(), rUnitMap);
}

TEST(Special_Psi_Disrupter, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Psi_Disrupter.requiredUnits(), rUnitMap);
}

TEST(Special_Zerg_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Zerg_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Terran_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Terran_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Protoss_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Protoss_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Zerg_Flag_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Zerg_Flag_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Terran_Flag_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Terran_Flag_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Protoss_Flag_Beacon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Protoss_Flag_Beacon.requiredUnits(), rUnitMap);
}

TEST(Special_Power_Generator, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Power_Generator.requiredUnits(), rUnitMap);
}

TEST(Special_Overmind_Cocoon, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Overmind_Cocoon.requiredUnits(), rUnitMap);
}

TEST(Spell_Dark_Swarm, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Spell_Dark_Swarm.requiredUnits(), rUnitMap);
}

TEST(Special_Floor_Missile_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Floor_Missile_Trap.requiredUnits(), rUnitMap);
}

TEST(Special_Floor_Hatch, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Floor_Hatch.requiredUnits(), rUnitMap);
}

TEST(Special_Upper_Level_Door, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Upper_Level_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Right_Upper_Level_Door, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Right_Upper_Level_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Pit_Door, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Pit_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Right_Pit_Door, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Right_Pit_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Floor_Gun_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Upper_Level_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Wall_Missile_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Upper_Level_Door.requiredUnits(), rUnitMap);
}

TEST(Special_Wall_Flame_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Wall_Flame_Trap.requiredUnits(), rUnitMap);
}

TEST(Special_Right_Wall_Missile_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Right_Wall_Missile_Trap.requiredUnits(), rUnitMap);
}

TEST(Special_Right_Wall_Flame_Trap, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Right_Wall_Flame_Trap.requiredUnits(), rUnitMap);
}

TEST(Special_Start_Location, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Special_Start_Location.requiredUnits(), rUnitMap);
}

TEST(Powerup_Flag, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Flag.requiredUnits(), rUnitMap);
}

TEST(Powerup_Young_Chrysalis, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Young_Chrysalis.requiredUnits(), rUnitMap);
}

TEST(Powerup_Psi_Emitter, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Psi_Emitter.requiredUnits(), rUnitMap);
}

TEST(Powerup_Data_Disk, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Data_Disk.requiredUnits(), rUnitMap);
}

TEST(Powerup_Khaydarin_Crystal, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Khaydarin_Crystal.requiredUnits(), rUnitMap);
}

TEST(Powerup_Mineral_Cluster_Type_1, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Mineral_Cluster_Type_1.requiredUnits(), rUnitMap);
}

TEST(Powerup_Mineral_Cluster_Type_2, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Mineral_Cluster_Type_2.requiredUnits(), rUnitMap);
}

TEST(Powerup_Protoss_Gas_Orb_Type_1, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Protoss_Gas_Orb_Type_1.requiredUnits(), rUnitMap);
}

TEST(Powerup_Protoss_Gas_Orb_Type_2, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Protoss_Gas_Orb_Type_2.requiredUnits(), rUnitMap);
}

TEST(Powerup_Zerg_Gas_Sac_Type_1, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Zerg_Gas_Sac_Type_1.requiredUnits(), rUnitMap);
}

TEST(Powerup_Zerg_Gas_Sac_Type_2, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Zerg_Gas_Sac_Type_2.requiredUnits(), rUnitMap);
}

TEST(Powerup_Terran_Gas_Tank_Type_1, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Terran_Gas_Tank_Type_1.requiredUnits(), rUnitMap);
}

TEST(Powerup_Terran_Gas_Tank_Type_2, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Powerup_Terran_Gas_Tank_Type_2.requiredUnits(), rUnitMap);
}

TEST(None, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::None.requiredUnits(), rUnitMap);
}

TEST(AllUnits, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::AllUnits.requiredUnits(), rUnitMap);
}

TEST(Men, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Men.requiredUnits(), rUnitMap);
}

TEST(Buildings, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Buildings.requiredUnits(), rUnitMap);
}

TEST(Factories, requiredUnits) {
    std::map<UnitType, int> rUnitMap{  };
    EXPECT_EQ(UnitTypes::Factories.requiredUnits(), rUnitMap);
}

TEST(Unknown, requiredUnits) {
    std::map<UnitType, int> rUnitMap{ {UnitTypes::Unknown, 1} };
    EXPECT_EQ(UnitTypes::Unknown.requiredUnits(), rUnitMap);
}