#include <string>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  template <>
  const std::string Type<UpgradeType, UpgradeTypes::Enum::Unknown>::typeNames[UpgradeTypes::Enum::MAX] =
  {
    "Terran_Infantry_Armor",
    "Terran_Vehicle_Plating",
    "Terran_Ship_Plating",
    "Zerg_Carapace",
    "Zerg_Flyer_Carapace",
    "Protoss_Ground_Armor",
    "Protoss_Air_Armor",
    "Terran_Infantry_Weapons",
    "Terran_Vehicle_Weapons",
    "Terran_Ship_Weapons",
    "Zerg_Melee_Attacks",
    "Zerg_Missile_Attacks",
    "Zerg_Flyer_Attacks",
    "Protoss_Ground_Weapons",
    "Protoss_Air_Weapons",
    "Protoss_Plasma_Shields",
    "U_238_Shells",
    "Ion_Thrusters",
    "",
    "Titan_Reactor",
    "Ocular_Implants",
    "Moebius_Reactor",
    "Apollo_Reactor",
    "Colossus_Reactor",
    "Ventral_Sacs",
    "Antennae",
    "Pneumatized_Carapace",
    "Metabolic_Boost",
    "Adrenal_Glands",
    "Muscular_Augments",
    "Grooved_Spines",
    "Gamete_Meiosis",
    "Metasynaptic_Node",
    "Singularity_Charge",
    "Leg_Enhancements",
    "Scarab_Damage",
    "Reaver_Capacity",
    "Gravitic_Drive",
    "Sensor_Array",
    "Gravitic_Boosters",
    "Khaydarin_Amulet",
    "Apial_Sensors",
    "Gravitic_Thrusters",
    "Carrier_Capacity",
    "Khaydarin_Core",
    "",
    "",
    "Argus_Jewel",
    "",
    "Argus_Talisman",
    "",
    "Caduceus_Reactor",
    "Chitinous_Plating",
    "Anabolic_Synthesis",
    "Charon_Boosters",
    "",
    "",
    "",
    "",
    "",
    "Upgrade_60",
    "None",
    "Unknown"
  };

  // LOCALIZATION
  std::string upgradeLocalNames[UpgradeTypes::Enum::MAX];

  // DEFAULTS
  static const int defaultOreCostBase[UpgradeTypes::Enum::MAX] =    // same as default gas cost base
  { 
    100, 100, 150, 150, 150, 100, 150, 100, 100, 100, 100, 100, 100, 100, 100, 200, 150, 100, 200, 150, 100, 150, 200, 150, 200, 150, 150, 100, 200,
    150, 150, 150, 150, 150, 150, 200, 200, 200, 150, 150, 150, 100, 200, 100, 150, 0, 0, 100, 100, 150, 150, 150, 150, 200, 100, 0, 0, 0, 0, 0, 0, 0, 0
  };
  static const int defaultOreCostFactor[UpgradeTypes::Enum::MAX] =    // same as default gas cost factor
  { 
    75, 75, 75, 75, 75, 75, 75, 75, 75, 50, 50, 50, 75, 50, 75, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  static const int defaultTimeCostBase[UpgradeTypes::Enum::MAX] =
  { 
    4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 1500, 1500, 0, 2500,
    2500, 2500, 2500, 2500, 2400, 2000, 2000, 1500, 1500, 1500, 1500, 2500, 2500, 2500, 2000, 2500, 2500, 2500, 2000,
    2000, 2500, 2500, 2500, 1500, 2500, 0, 0, 2500, 2500, 2500, 2500, 2500, 2000, 2000, 2000, 0, 0, 0, 0, 0, 0, 0, 0
  };
  static const int defaultTimeCostFactor[UpgradeTypes::Enum::MAX] =
  { 
    480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  static const int defaultMaxRepeats[UpgradeTypes::Enum::MAX] = 
  {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
  };
  // ACTUAL
  int oreCostBase[UpgradeTypes::Enum::MAX], gasCostBase[UpgradeTypes::Enum::MAX], timeCostBase[UpgradeTypes::Enum::MAX];
  int oreCostFactor[UpgradeTypes::Enum::MAX], gasCostFactor[UpgradeTypes::Enum::MAX], timeCostFactor[UpgradeTypes::Enum::MAX];
  int maxRepeats[UpgradeTypes::Enum::MAX];

  namespace upgradeInternalWhat
  {
    using namespace UnitTypes;
    static const UnitType whatUpgrades[UpgradeTypes::Enum::MAX] =
    {
      Terran_Engineering_Bay, Terran_Armory, Terran_Armory, Zerg_Evolution_Chamber, Zerg_Spire, Protoss_Forge, Protoss_Cybernetics_Core, Terran_Engineering_Bay,
      Terran_Armory, Terran_Armory, Zerg_Evolution_Chamber, Zerg_Evolution_Chamber, Zerg_Spire, Protoss_Forge, Protoss_Cybernetics_Core, Protoss_Forge, Terran_Academy,
      Terran_Machine_Shop, None, Terran_Science_Facility, Terran_Covert_Ops, Terran_Covert_Ops, Terran_Control_Tower, Terran_Physics_Lab, Zerg_Lair, Zerg_Lair, Zerg_Lair,
      Zerg_Spawning_Pool, Zerg_Spawning_Pool, Zerg_Hydralisk_Den, Zerg_Hydralisk_Den, Zerg_Queens_Nest, Zerg_Defiler_Mound, Protoss_Cybernetics_Core, Protoss_Citadel_of_Adun,
      Protoss_Robotics_Support_Bay, Protoss_Robotics_Support_Bay, Protoss_Robotics_Support_Bay, Protoss_Observatory, Protoss_Observatory, Protoss_Templar_Archives, 
      Protoss_Fleet_Beacon, Protoss_Fleet_Beacon, Protoss_Fleet_Beacon, Protoss_Arbiter_Tribunal, None, None, Protoss_Fleet_Beacon, None, Protoss_Templar_Archives,
      None, Terran_Academy, Zerg_Ultralisk_Cavern, Zerg_Ultralisk_Cavern, Terran_Machine_Shop, None, None, None, None, None, None, None, None
    };
  }
  namespace upgradeInternalReqs
  {
    using namespace UnitTypes;
    static const UnitType requirements[3][UpgradeTypes::Enum::MAX] =
    {
      // Level 1
      { None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, 
        None, None, None, None, None, Zerg_Hive, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, Terran_Armory, None, None, None, None, None, None, None, None },
      // Level 2
      { Terran_Science_Facility, Terran_Science_Facility, Terran_Science_Facility, Zerg_Lair, Zerg_Lair, Protoss_Templar_Archives, Protoss_Fleet_Beacon,
        Terran_Science_Facility, Terran_Science_Facility, Terran_Science_Facility, Zerg_Lair, Zerg_Lair, Zerg_Lair, Protoss_Templar_Archives,
        Protoss_Fleet_Beacon, Protoss_Cybernetics_Core, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, None },
      // Level 3
      { Terran_Science_Facility, Terran_Science_Facility, Terran_Science_Facility, Zerg_Hive, Zerg_Hive, Protoss_Templar_Archives, Protoss_Fleet_Beacon,
        Terran_Science_Facility, Terran_Science_Facility, Terran_Science_Facility, Zerg_Hive, Zerg_Hive, Zerg_Hive, Protoss_Templar_Archives,
        Protoss_Fleet_Beacon, Protoss_Cybernetics_Core, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None,
        None, None, None, None, None, None, None, None, None },
    };
  }
  namespace upgradeInternalUsage
  {
    using namespace UnitTypes;

    static const UnitType Infantry_Armor[] = { Terran_Marine, Terran_Ghost, Terran_SCV, Hero_Gui_Montag, Terran_Civilian, Hero_Sarah_Kerrigan,
                                          Hero_Jim_Raynor_Marine, Terran_Firebat, Terran_Medic, Hero_Samir_Duran, Hero_Alexei_Stukov };
    static const UnitType Vehicle_Plating[] = { Terran_Vulture, Terran_Goliath, Terran_Siege_Tank_Tank_Mode, Hero_Alan_Schezar, Hero_Jim_Raynor_Vulture,
                                            Hero_Edmund_Duke_Tank_Mode, Hero_Edmund_Duke_Siege_Mode, Terran_Siege_Tank_Siege_Mode };
    static const UnitType Ship_Plating[] = { Terran_Wraith, Terran_Science_Vessel, Terran_Dropship, Terran_Battlecruiser, Hero_Tom_Kazansky, Hero_Magellan,
                                        Hero_Arcturus_Mengsk, Hero_Hyperion, Hero_Norad_II, Terran_Valkyrie, Hero_Gerard_DuGalle };
    static const UnitType Carapace[] = { Zerg_Larva, Zerg_Egg, Zerg_Zergling, Zerg_Hydralisk, Zerg_Ultralisk, Zerg_Broodling, Zerg_Drone, Zerg_Defiler,
                                    Hero_Torrasque, Zerg_Infested_Terran, Hero_Infested_Kerrigan, Hero_Unclean_One, Hero_Hunter_Killer, Hero_Devouring_One,
                                    Zerg_Cocoon, Zerg_Lurker_Egg, Zerg_Lurker, Hero_Infested_Duran };
    static const UnitType Flyer_Carapace[] = { Zerg_Overlord, Zerg_Mutalisk, Zerg_Guardian, Zerg_Queen, Zerg_Scourge, Hero_Matriarch, Hero_Kukulza_Mutalisk,
                                          Hero_Kukulza_Guardian, Hero_Yggdrasill, Zerg_Devourer };
    static const UnitType Protoss_Armor[] = { Protoss_Dark_Templar, Protoss_Dark_Archon, Protoss_Probe, Protoss_Zealot, Protoss_Dragoon, Protoss_High_Templar,
                                          Protoss_Archon, Hero_Dark_Templar, Hero_Zeratul, Hero_Tassadar_Zeratul_Archon, Hero_Fenix_Zealot, Hero_Fenix_Dragoon,
                                          Hero_Tassadar, Hero_Warbringer, Protoss_Reaver, Hero_Aldaris };
    static const UnitType Protoss_Plating[] = { Protoss_Corsair, Protoss_Shuttle, Protoss_Scout, Protoss_Arbiter, Protoss_Carrier, Protoss_Interceptor, Hero_Mojo,
                                            Hero_Gantrithor, Protoss_Observer, Hero_Danimoth, Hero_Artanis, Hero_Raszagal };
    static const UnitType Infantry_Weapons[] = { Terran_Marine, Hero_Jim_Raynor_Marine, Terran_Ghost, Hero_Sarah_Kerrigan, Terran_Firebat, Hero_Gui_Montag,
                                            Special_Wall_Flame_Trap, Special_Right_Wall_Flame_Trap, Hero_Samir_Duran, Hero_Alexei_Stukov, Hero_Infested_Duran };
    static const UnitType Vehicle_Weapons[] = { Terran_Vulture, Hero_Jim_Raynor_Vulture, Terran_Goliath, Hero_Alan_Schezar, Terran_Siege_Tank_Tank_Mode,
                                            Terran_Siege_Tank_Siege_Mode, Hero_Edmund_Duke_Tank_Mode, Hero_Edmund_Duke_Siege_Mode, Special_Floor_Missile_Trap,
                                            Special_Floor_Gun_Trap, Special_Wall_Missile_Trap, Special_Right_Wall_Missile_Trap };
    static const UnitType Ship_Weapons[] = { Terran_Wraith, Hero_Tom_Kazansky, Terran_Battlecruiser, Hero_Hyperion, Hero_Norad_II, Hero_Arcturus_Mengsk,
                                        Hero_Gerard_DuGalle, Terran_Valkyrie };
    static const UnitType Zerg_MeleeAtk[] = { Zerg_Zergling, Hero_Devouring_One, Hero_Infested_Kerrigan, Zerg_Ultralisk, Hero_Torrasque, Zerg_Broodling };
    static const UnitType Zerg_RangeAtk[] = { Zerg_Hydralisk, Hero_Hunter_Killer, Zerg_Lurker };
    static const UnitType Zerg_FlyerAtk[] = { Zerg_Mutalisk, Hero_Kukulza_Mutalisk, Hero_Kukulza_Guardian, Zerg_Guardian, Zerg_Devourer };
    static const UnitType Protoss_GrndWpn[] = { Protoss_Zealot, Hero_Fenix_Zealot, Protoss_Dragoon, Hero_Fenix_Dragoon, Hero_Tassadar, Hero_Aldaris, Protoss_Archon,
                                            Hero_Tassadar_Zeratul_Archon, Hero_Dark_Templar, Hero_Zeratul, Protoss_Dark_Templar };
    static const UnitType Protoss_AirWpn[] = { Protoss_Scout, Hero_Mojo, Protoss_Arbiter, Hero_Danimoth, Protoss_Interceptor, Protoss_Carrier, Protoss_Corsair, Hero_Artanis };
    static const UnitType Shields[] = { Protoss_Corsair, Protoss_Dark_Templar, Protoss_Dark_Archon, Protoss_Probe, Protoss_Zealot, Protoss_Dragoon, Protoss_High_Templar,
                                    Protoss_Archon, Protoss_Shuttle, Protoss_Scout, Protoss_Arbiter, Protoss_Carrier, Protoss_Interceptor, Hero_Dark_Templar,
                                    Hero_Zeratul, Hero_Tassadar_Zeratul_Archon, Hero_Fenix_Zealot, Hero_Fenix_Dragoon, Hero_Tassadar, Hero_Mojo, Hero_Warbringer,
                                    Hero_Gantrithor, Protoss_Reaver, Protoss_Observer, Hero_Danimoth, Hero_Aldaris, Hero_Artanis, Hero_Raszagal };
    static const UnitType Shells[] = { Terran_Marine };
    static const UnitType Ion_Thrusters[] = { Terran_Vulture };
    static const UnitType Titan_Reactor[] = { Terran_Science_Vessel };
    static const UnitType Ghost_Upgrades[] = { Terran_Ghost };
    static const UnitType Apollo_Reactor[] = { Terran_Wraith };
    static const UnitType Colossus_Reactor[] = { Terran_Battlecruiser };
    static const UnitType Overlord_Upgrades[] = { Zerg_Overlord };
    static const UnitType Zergling_Upgrades[] = { Zerg_Zergling };
    static const UnitType Hydralisk_Upgrades[] = { Zerg_Hydralisk };
    static const UnitType Gamete_Meiosis[] = { Zerg_Queen };
    static const UnitType Metasynaptic_Node[] = { Zerg_Defiler };
    static const UnitType Singularity_Charge[] = { Protoss_Dragoon };
    static const UnitType Leg_Enhancements[] = { Protoss_Zealot };
    static const UnitType Reaver_Upgrades[] = { Protoss_Reaver };
    static const UnitType Gravitic_Drive[] = { Protoss_Shuttle };
    static const UnitType Observer_Upgrades[] = { Protoss_Observer };
    static const UnitType Khaydarin_Amulet[] = { Protoss_High_Templar };
    static const UnitType Scout_Upgrades[] = { Protoss_Scout };
    static const UnitType Carrier_Capacity[] = { Protoss_Carrier };
    static const UnitType Khaydarin_Core[] = { Protoss_Arbiter };
    static const UnitType Argus_Jewel[] = { Protoss_Corsair };
    static const UnitType Argus_Talisman[] = { Protoss_Dark_Archon };
    static const UnitType Caduceus_Reactor[] = { Terran_Medic };
    static const UnitType Ultralisk_Upgrades[] = { Zerg_Ultralisk };
    static const UnitType Charon_Boosters[] = { Terran_Goliath };

    static const UnitType Upgrade60[] = { Terran_Vulture_Spider_Mine, Critter_Ursadon, Critter_Scantid, Critter_Rhynadon, Critter_Ragnasaur, Critter_Kakaru, Critter_Bengalaas,
      Special_Cargo_Ship, Special_Mercenary_Gunship, Terran_SCV, Protoss_Probe, Zerg_Drone, Zerg_Infested_Terran, Zerg_Scourge };

#define TSET(x) {std::begin(x), std::end(x)}
#define TSETEMPTY {}
    static const UnitType::set upgradeWhatUses[UpgradeTypes::Enum::MAX] =
    {
      TSET(Infantry_Armor), TSET(Vehicle_Plating), TSET(Ship_Plating), TSET(Carapace), TSET(Flyer_Carapace), TSET(Protoss_Armor), TSET(Protoss_Plating),
      TSET(Infantry_Weapons), TSET(Vehicle_Weapons), TSET(Ship_Weapons), TSET(Zerg_MeleeAtk), TSET(Zerg_RangeAtk), TSET(Zerg_FlyerAtk), TSET(Protoss_GrndWpn),
      TSET(Protoss_AirWpn), TSET(Shields), TSET(Shells), TSET(Ion_Thrusters), TSETEMPTY, TSET(Titan_Reactor), TSET(Ghost_Upgrades), TSET(Ghost_Upgrades),
      TSET(Apollo_Reactor), TSET(Colossus_Reactor), TSET(Overlord_Upgrades), TSET(Overlord_Upgrades), TSET(Overlord_Upgrades), TSET(Zergling_Upgrades),
      TSET(Zergling_Upgrades), TSET(Hydralisk_Upgrades), TSET(Hydralisk_Upgrades), TSET(Gamete_Meiosis), TSET(Metasynaptic_Node), TSET(Singularity_Charge),
      TSET(Leg_Enhancements), TSET(Reaver_Upgrades), TSET(Reaver_Upgrades), TSET(Gravitic_Drive), TSET(Observer_Upgrades), TSET(Observer_Upgrades),
      TSET(Khaydarin_Amulet), TSET(Scout_Upgrades), TSET(Scout_Upgrades), TSET(Carrier_Capacity), TSET(Khaydarin_Core), TSETEMPTY, TSETEMPTY,
      TSET(Argus_Jewel), TSETEMPTY, TSET(Argus_Talisman), TSETEMPTY, TSET(Caduceus_Reactor), TSET(Ultralisk_Upgrades), TSET(Ultralisk_Upgrades),
      TSET(Charon_Boosters), TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSETEMPTY, TSET(Upgrade60), TSETEMPTY, TSETEMPTY
    };
#undef TSETEMPTY
#undef TSET
  }

  namespace upgradeInternalRace
  {
    using namespace Races;
    static const Race upgradeRaces[UpgradeTypes::Enum::MAX] =
    {
      Terran, Terran, Terran, Zerg, Zerg, Protoss, Protoss, Terran, Terran, Terran, Zerg, Zerg, Zerg, Protoss, Protoss, Protoss, Terran, Terran, Terran, Terran, Terran, 
      Terran, Terran, Terran, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Zerg, Protoss, Protoss, Protoss, Protoss, Protoss, Protoss, Protoss, Protoss, Protoss, 
      Protoss, Protoss, Protoss, None, None, Protoss, None, Protoss, None, Terran, Zerg, Zerg, Terran, None, None, None, None, None, None, None, Unknown
    };
  }

  namespace UpgradeTypeSet
  {
    using namespace UpgradeTypes;
    const UpgradeType::set upgradeTypeSet = { Terran_Infantry_Armor, Terran_Vehicle_Plating, Terran_Ship_Plating,
      Zerg_Carapace, Zerg_Flyer_Carapace, Protoss_Ground_Armor, Protoss_Air_Armor,
      Terran_Infantry_Weapons, Terran_Vehicle_Weapons, Terran_Ship_Weapons,
      Zerg_Melee_Attacks, Zerg_Missile_Attacks, Zerg_Flyer_Attacks,
      Protoss_Ground_Weapons, Protoss_Air_Weapons, Protoss_Plasma_Shields,
      U_238_Shells, Ion_Thrusters, Titan_Reactor, Ocular_Implants,
      Moebius_Reactor, Apollo_Reactor, Colossus_Reactor, Ventral_Sacs,
      Antennae, Pneumatized_Carapace, Metabolic_Boost, Adrenal_Glands,
      Muscular_Augments, Grooved_Spines, Gamete_Meiosis, Metasynaptic_Node,
      Singularity_Charge, Leg_Enhancements, Scarab_Damage, Reaver_Capacity,
      Gravitic_Drive, Sensor_Array, Gravitic_Boosters, Khaydarin_Amulet,
      Apial_Sensors, Gravitic_Thrusters, Carrier_Capacity, Khaydarin_Core,
      Argus_Jewel, Argus_Talisman, Caduceus_Reactor, Chitinous_Plating,
      Anabolic_Synthesis, Charon_Boosters, None, Unknown };
  }
  Race UpgradeType::getRace() const
  {
    return upgradeInternalRace::upgradeRaces[this->getID()];
  }
  int UpgradeType::mineralPrice(int level) const
  {
    return defaultOreCostBase[this->getID()] + 
           std::max(0, level-1) * this->mineralPriceFactor();
  }
  int UpgradeType::mineralPriceFactor() const
  {
    return defaultOreCostFactor[this->getID()];
  }
  int UpgradeType::gasPrice(int level) const
  {
    // Gas price is exactly the same as mineral price
    return mineralPrice(level);
  }
  int UpgradeType::gasPriceFactor() const
  {
    // Gas price factor is the same as mineral price factor
    return mineralPriceFactor();
  }
  int UpgradeType::upgradeTime(int level) const
  {
    return defaultTimeCostBase[this->getID()] + 
            std::max(0, level-1) * this->upgradeTimeFactor();
  }
  int UpgradeType::upgradeTimeFactor() const
  {
    return defaultTimeCostFactor[this->getID()];
  }
  UnitType UpgradeType::whatUpgrades() const
  {
    return upgradeInternalWhat::whatUpgrades[this->getID()];
  }
  const UnitType::set& UpgradeType::whatUses() const
  {
    return upgradeInternalUsage::upgradeWhatUses[this->getID()];
  }
  int UpgradeType::maxRepeats() const
  {
    return defaultMaxRepeats[this->getID()];
  }
  UnitType UpgradeType::whatsRequired(int level) const
  {
    if ( level >= 1 && level <= 3)
      return upgradeInternalReqs::requirements[level-1][this->getID()];
    return UnitTypes::None;
  }
  const UpgradeType::set& UpgradeTypes::allUpgradeTypes()
  {
    return UpgradeTypeSet::upgradeTypeSet;
  }
}
