#include "UpgradeTypes.h"

#include <BWAPITypes\UpgradeType.h>
#include <BWAPITypes\UpgradeTypeId.h>

namespace BWAPI
{
  UpgradeTypeInternal upgradeTypeData[UpgradeTypeIds::count];
  std::map<std::string, UpgradeTypeId> upgradeTypeMap;
  std::set< UpgradeTypeId > upgradeTypeSet;

  void fillUpgradeType(UpgradeTypeId id, const char* name, int mineralPriceBase, int mineralPriceFactor, int gasPriceBase, int gasPriceFactor, int upgradeTimeBase, int upgradeTimeFactor, BWAPI::UnitTypeId whatUpgrades, RaceId race, BWAPI::UnitTypeId whatUses, int maxRepeats)
  {
    UpgradeTypeInternal &target = upgradeTypeData[id];
    target.name = name;
    target.mineralPriceBase = mineralPriceBase;
    target.mineralPriceFactor = mineralPriceFactor;
    target.gasPriceBase = gasPriceBase;
    target.gasPriceFactor = gasPriceFactor;
    target.upgradeTimeBase = upgradeTimeBase;
    target.upgradeTimeFactor = upgradeTimeFactor;
    target.whatUpgrades = whatUpgrades;
    target.race = race;
    if (whatUses!=NULL && whatUses!= UnitTypeIds::None)
    {
      target.whatUses.insert(whatUses);
    }
    target.maxRepeats = maxRepeats;
    target.valid = true;
  }
  namespace UpgradeTypes
  {
    void init()
    {
      fillUpgradeType(UpgradeTypeIds::Terran_Infantry_Armor, "Terran Infantry Armor", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Terran_Engineering_Bay, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Terran_Vehicle_Plating, "Terran Vehicle Plating", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Terran_Armory, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Terran_Ship_Plating, "Terran Ship Plating", 150, 75, 150, 75, 4000, 480, UnitTypeIds::Terran_Armory, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Zerg_Carapace, "Zerg Carapace", 150, 75, 150, 75, 4000, 480, UnitTypeIds::Zerg_Evolution_Chamber, RaceIds::Zerg, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Zerg_Flyer_Carapace, "Zerg Flyer Carapace", 150, 75, 150, 75, 4000, 480, UnitTypeIds::Zerg_Spire, RaceIds::Zerg, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Protoss_Armor, "Protoss Armor", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Protoss_Forge, RaceIds::Protoss, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Protoss_Plating, "Protoss Plating", 150, 75, 150, 75, 4000, 480, UnitTypeIds::Protoss_Cybernetics_Core, RaceIds::Protoss, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Terran_Infantry_Weapons, "Terran Infantry Weapons", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Terran_Engineering_Bay, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Terran_Vehicle_Weapons, "Terran Vehicle Weapons", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Terran_Armory, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Terran_Ship_Weapons, "Terran Ship Weapons", 100, 50, 100, 50, 4000, 480, UnitTypeIds::Terran_Armory, RaceIds::Terran, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Zerg_Melee_Attacks, "Zerg Melee Attacks", 100, 50, 100, 50, 4000, 480, UnitTypeIds::Zerg_Evolution_Chamber, RaceIds::Zerg, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Zerg_Missile_Attacks, "Zerg Missile Attacks", 100, 50, 100, 50, 4000, 480, UnitTypeIds::Zerg_Evolution_Chamber, RaceIds::Zerg, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Zerg_Flyer_Attacks, "Zerg Flyer Attacks", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Zerg_Spire, RaceIds::Zerg, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Protoss_Ground_Weapons, "Protoss Ground Weapons", 100, 50, 100, 50, 4000, 480, UnitTypeIds::Protoss_Forge, RaceIds::Protoss, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Protoss_Air_Weapons, "Protoss Air Weapons", 100, 75, 100, 75, 4000, 480, UnitTypeIds::Protoss_Cybernetics_Core, RaceIds::Protoss, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::Protoss_Plasma_Shields, "Protoss Plasma Shields", 200, 100, 200, 100, 4000, 480, UnitTypeIds::Protoss_Forge, RaceIds::Protoss, UnitTypeIds::None, 3);
      fillUpgradeType(UpgradeTypeIds::U_238_Shells, "U-238 Shells", 150, 0, 150, 0, 1500, 0, UnitTypeIds::Terran_Academy, RaceIds::Terran, UnitTypeIds::Terran_Marine, 1);
      fillUpgradeType(UpgradeTypeIds::Ion_Thrusters, "Ion Thrusters", 100, 0, 100, 0, 1500, 0, UnitTypeIds::Terran_Machine_Shop, RaceIds::Terran, UnitTypeIds::Terran_Vulture, 1);
      fillUpgradeType(UpgradeTypeIds::Titan_Reactor, "Titan Reactor", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Terran_Science_Facility, RaceIds::Terran, UnitTypeIds::Terran_Science_Vessel, 1);
      fillUpgradeType(UpgradeTypeIds::Ocular_Implants, "Ocular Implants", 100, 0, 100, 0, 2500, 0, UnitTypeIds::Terran_Covert_Ops, RaceIds::Terran, UnitTypeIds::Terran_Ghost, 1);
      fillUpgradeType(UpgradeTypeIds::Moebius_Reactor, "Moebius Reactor", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Terran_Covert_Ops, RaceIds::Terran, UnitTypeIds::Terran_Ghost, 1);
      fillUpgradeType(UpgradeTypeIds::Apollo_Reactor, "Apollo Reactor", 200, 0, 200, 0, 2500, 0, UnitTypeIds::Terran_Control_Tower, RaceIds::Terran, UnitTypeIds::Terran_Wraith, 1);
      fillUpgradeType(UpgradeTypeIds::Colossus_Reactor, "Colossus Reactor", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Terran_Physics_Lab, RaceIds::Terran, UnitTypeIds::Terran_Battlecruiser, 1);
      fillUpgradeType(UpgradeTypeIds::Ventral_Sacs, "Ventral Sacs", 200, 0, 200, 0, 2400, 0, UnitTypeIds::Zerg_Lair, RaceIds::Zerg, UnitTypeIds::Zerg_Overlord, 1);
      fillUpgradeType(UpgradeTypeIds::Antennae, "Antennae", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Zerg_Lair, RaceIds::Zerg, UnitTypeIds::Zerg_Overlord, 1);
      fillUpgradeType(UpgradeTypeIds::Pneumatized_Carapace, "Pneumatized Carapace", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Zerg_Lair, RaceIds::Zerg, UnitTypeIds::Zerg_Overlord, 1);
      fillUpgradeType(UpgradeTypeIds::Metabolic_Boost, "Metabolic Boost", 100, 0, 100, 0, 1500, 0, UnitTypeIds::Zerg_Spawning_Pool, RaceIds::Zerg, UnitTypeIds::Zerg_Zergling, 1);
      fillUpgradeType(UpgradeTypeIds::Adrenal_Glands, "Adrenal Glands", 200, 0, 200, 0, 1500, 0, UnitTypeIds::Zerg_Spawning_Pool, RaceIds::Zerg, UnitTypeIds::Zerg_Zergling, 1);
      fillUpgradeType(UpgradeTypeIds::Muscular_Augments, "Muscular Augments", 150, 0, 150, 0, 1500, 0, UnitTypeIds::Zerg_Hydralisk_Den, RaceIds::Zerg, UnitTypeIds::Zerg_Hydralisk, 1);
      fillUpgradeType(UpgradeTypeIds::Grooved_Spines, "Grooved Spines", 150, 0, 150, 0, 1500, 0, UnitTypeIds::Zerg_Hydralisk_Den, RaceIds::Zerg, UnitTypeIds::Zerg_Hydralisk, 1);
      fillUpgradeType(UpgradeTypeIds::Gamete_Meiosis, "Gamete Meiosis", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Zerg_Queens_Nest, RaceIds::Zerg, UnitTypeIds::Zerg_Queen, 1);
      fillUpgradeType(UpgradeTypeIds::Metasynaptic_Node, "Metasynaptic Node", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Zerg_Defiler_Mound, RaceIds::Zerg, UnitTypeIds::Zerg_Defiler, 1);
      fillUpgradeType(UpgradeTypeIds::Singularity_Charge, "Singularity Charge", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Protoss_Cybernetics_Core, RaceIds::Protoss, UnitTypeIds::Protoss_Dragoon, 1);
      fillUpgradeType(UpgradeTypeIds::Leg_Enhancements, "Leg Enhancements", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Protoss_Citadel_of_Adun, RaceIds::Protoss, UnitTypeIds::Protoss_Zealot, 1);
      fillUpgradeType(UpgradeTypeIds::Scarab_Damage, "Scarab Damage", 200, 0, 200, 0, 2500, 0, UnitTypeIds::Protoss_Robotics_Support_Bay, RaceIds::Protoss, UnitTypeIds::Protoss_Scarab, 1);
      fillUpgradeType(UpgradeTypeIds::Reaver_Capacity, "Reaver Capacity", 200, 0, 200, 0, 2500, 0, UnitTypeIds::Protoss_Robotics_Support_Bay, RaceIds::Protoss, UnitTypeIds::Protoss_Reaver, 1);
      fillUpgradeType(UpgradeTypeIds::Gravitic_Drive, "Gravitic Drive", 200, 0, 200, 0, 2500, 0, UnitTypeIds::Protoss_Robotics_Support_Bay, RaceIds::Protoss, UnitTypeIds::Protoss_Shuttle, 1);
      fillUpgradeType(UpgradeTypeIds::Sensor_Array, "Sensor Array", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Protoss_Observatory, RaceIds::Protoss, UnitTypeIds::Protoss_Observer, 1);
      fillUpgradeType(UpgradeTypeIds::Gravitic_Boosters, "Gravitic Boosters", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Protoss_Observatory, RaceIds::Protoss, UnitTypeIds::Protoss_Observer, 1);
      fillUpgradeType(UpgradeTypeIds::Khaydarin_Amulet, "Khaydarin Amulet", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Protoss_Templar_Archives, RaceIds::Protoss, UnitTypeIds::Protoss_High_Templar, 1);
      fillUpgradeType(UpgradeTypeIds::Apial_Sensors, "Apial Sensors", 100, 0, 100, 0, 2500, 0, UnitTypeIds::Protoss_Templar_Archives, RaceIds::Protoss, UnitTypeIds::Protoss_Scout, 1);
      fillUpgradeType(UpgradeTypeIds::Gravitic_Thrusters, "Gravitic Thrusters", 200, 0, 200, 0, 2500, 0, UnitTypeIds::Protoss_Fleet_Beacon, RaceIds::Protoss, UnitTypeIds::Protoss_Scout, 1);
      fillUpgradeType(UpgradeTypeIds::Carrier_Capacity, "Carrier Capacity", 100, 0, 100, 0, 1500, 0, UnitTypeIds::Protoss_Fleet_Beacon, RaceIds::Protoss, UnitTypeIds::Protoss_Carrier, 1);
      fillUpgradeType(UpgradeTypeIds::Khaydarin_Core, "Khaydarin Core", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Protoss_Arbiter_Tribunal, RaceIds::Protoss, UnitTypeIds::Protoss_Arbiter, 1);
      fillUpgradeType(UpgradeTypeIds::Argus_Jewel, "Argus Jewel", 100, 0, 100, 0, 2500, 0, UnitTypeIds::Protoss_Fleet_Beacon, RaceIds::Protoss, UnitTypeIds::Protoss_Corsair, 1);
      fillUpgradeType(UpgradeTypeIds::Argus_Talisman, "Argus Talisman", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Protoss_Templar_Archives, RaceIds::Protoss, UnitTypeIds::Protoss_Dark_Archon, 1);
      fillUpgradeType(UpgradeTypeIds::Caduceus_Reactor, "Caduceus Reactor", 150, 0, 150, 0, 2500, 0, UnitTypeIds::Terran_Academy, RaceIds::Terran, UnitTypeIds::Terran_Medic, 1);
      fillUpgradeType(UpgradeTypeIds::Chitinous_Plating, "Chitinous Plating", 150, 0, 150, 0, 2000, 0, UnitTypeIds::Zerg_Ultralisk_Cavern, RaceIds::Zerg, UnitTypeIds::Zerg_Ultralisk, 1);
      fillUpgradeType(UpgradeTypeIds::Anabolic_Synthesis, "Anabolic Synthesis", 200, 0, 200, 0, 2000, 0, UnitTypeIds::Zerg_Ultralisk_Cavern, RaceIds::Zerg, UnitTypeIds::Zerg_Ultralisk, 1);
      fillUpgradeType(UpgradeTypeIds::Charon_Booster, "Charon Booster", 100, 0, 100, 0, 2000, 0, UnitTypeIds::Terran_Machine_Shop, RaceIds::Terran, UnitTypeIds::Terran_Goliath, 1);
      fillUpgradeType(UpgradeTypeIds::None, "None", 0, 0, 0, 0, 0, 0, UnitTypeIds::None, RaceIds::None, UnitTypeIds::None, 0);

      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Armor].whatUses.insert(UnitTypeIds::Terran_Firebat);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Armor].whatUses.insert(UnitTypeIds::Terran_Ghost);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Armor].whatUses.insert(UnitTypeIds::Terran_Marine);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Armor].whatUses.insert(UnitTypeIds::Terran_Medic);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Armor].whatUses.insert(UnitTypeIds::Terran_SCV);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Plating].whatUses.insert(UnitTypeIds::Terran_Goliath);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Plating].whatUses.insert(UnitTypeIds::Terran_Siege_Tank_Siege_Mode);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Plating].whatUses.insert(UnitTypeIds::Terran_Siege_Tank_Tank_Mode);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Plating].whatUses.insert(UnitTypeIds::Terran_Vulture);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Plating].whatUses.insert(UnitTypeIds::Terran_Battlecruiser);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Plating].whatUses.insert(UnitTypeIds::Terran_Dropship);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Plating].whatUses.insert(UnitTypeIds::Terran_Science_Vessel);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Plating].whatUses.insert(UnitTypeIds::Terran_Valkyrie);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Plating].whatUses.insert(UnitTypeIds::Terran_Wraith);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Broodling);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Defiler);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Drone);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Hydralisk);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Infested_Terran);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Larva);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Lurker);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Ultralisk);
      upgradeTypeData[UpgradeTypeIds::Zerg_Carapace].whatUses.insert(UnitTypeIds::Zerg_Zergling);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Devourer);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Guardian);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Mutalisk);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Overlord);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Queen);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Carapace].whatUses.insert(UnitTypeIds::Zerg_Scourge);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Archon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Dark_Archon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Dark_Templar);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Dragoon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_High_Templar);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Probe);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Reaver);
      upgradeTypeData[UpgradeTypeIds::Protoss_Armor].whatUses.insert(UnitTypeIds::Protoss_Zealot);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Arbiter);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Carrier);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Corsair);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Interceptor);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Observer);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Scout);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plating].whatUses.insert(UnitTypeIds::Protoss_Shuttle);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Weapons].whatUses.insert(UnitTypeIds::Terran_Firebat);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Weapons].whatUses.insert(UnitTypeIds::Terran_Ghost);
      upgradeTypeData[UpgradeTypeIds::Terran_Infantry_Weapons].whatUses.insert(UnitTypeIds::Terran_Marine);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Weapons].whatUses.insert(UnitTypeIds::Terran_Goliath);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Weapons].whatUses.insert(UnitTypeIds::Terran_Siege_Tank_Siege_Mode);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Weapons].whatUses.insert(UnitTypeIds::Terran_Siege_Tank_Tank_Mode);
      upgradeTypeData[UpgradeTypeIds::Terran_Vehicle_Weapons].whatUses.insert(UnitTypeIds::Terran_Vulture);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Weapons].whatUses.insert(UnitTypeIds::Terran_Battlecruiser);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Weapons].whatUses.insert(UnitTypeIds::Terran_Valkyrie);
      upgradeTypeData[UpgradeTypeIds::Terran_Ship_Weapons].whatUses.insert(UnitTypeIds::Terran_Wraith);
      upgradeTypeData[UpgradeTypeIds::Zerg_Melee_Attacks].whatUses.insert(UnitTypeIds::Zerg_Broodling);
      upgradeTypeData[UpgradeTypeIds::Zerg_Melee_Attacks].whatUses.insert(UnitTypeIds::Zerg_Ultralisk);
      upgradeTypeData[UpgradeTypeIds::Zerg_Melee_Attacks].whatUses.insert(UnitTypeIds::Zerg_Zergling);
      upgradeTypeData[UpgradeTypeIds::Zerg_Missile_Attacks].whatUses.insert(UnitTypeIds::Zerg_Hydralisk);
      upgradeTypeData[UpgradeTypeIds::Zerg_Missile_Attacks].whatUses.insert(UnitTypeIds::Zerg_Lurker);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Attacks].whatUses.insert(UnitTypeIds::Zerg_Devourer);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Attacks].whatUses.insert(UnitTypeIds::Zerg_Guardian);
      upgradeTypeData[UpgradeTypeIds::Zerg_Flyer_Attacks].whatUses.insert(UnitTypeIds::Zerg_Mutalisk);
      upgradeTypeData[UpgradeTypeIds::Protoss_Ground_Weapons].whatUses.insert(UnitTypeIds::Protoss_Dark_Templar);
      upgradeTypeData[UpgradeTypeIds::Protoss_Ground_Weapons].whatUses.insert(UnitTypeIds::Protoss_Dragoon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Ground_Weapons].whatUses.insert(UnitTypeIds::Protoss_Zealot);
      upgradeTypeData[UpgradeTypeIds::Protoss_Air_Weapons].whatUses.insert(UnitTypeIds::Protoss_Arbiter);
      upgradeTypeData[UpgradeTypeIds::Protoss_Air_Weapons].whatUses.insert(UnitTypeIds::Protoss_Corsair);
      upgradeTypeData[UpgradeTypeIds::Protoss_Air_Weapons].whatUses.insert(UnitTypeIds::Protoss_Interceptor);
      upgradeTypeData[UpgradeTypeIds::Protoss_Air_Weapons].whatUses.insert(UnitTypeIds::Protoss_Scout);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Arbiter);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Arbiter_Tribunal);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Archon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Assimilator);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Carrier);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Citadel_of_Adun);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Corsair);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Cybernetics_Core);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Dark_Archon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Dark_Templar);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Dragoon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Fleet_Beacon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Forge);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Gateway);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_High_Templar);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Interceptor);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Nexus);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Observatory);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Observer);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Photon_Cannon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Probe);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Pylon);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Reaver);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Robotics_Facility);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Robotics_Support_Bay);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Scarab);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Scout);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Shield_Battery);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Shuttle);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Stargate);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Templar_Archives);
      upgradeTypeData[UpgradeTypeIds::Protoss_Plasma_Shields].whatUses.insert(UnitTypeIds::Protoss_Zealot);

      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Infantry_Armor);
      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Vehicle_Plating);
      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Ship_Plating);
      upgradeTypeSet.insert(UpgradeTypeIds::Zerg_Carapace);
      upgradeTypeSet.insert(UpgradeTypeIds::Zerg_Flyer_Carapace);
      upgradeTypeSet.insert(UpgradeTypeIds::Protoss_Armor);
      upgradeTypeSet.insert(UpgradeTypeIds::Protoss_Plating);
      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Infantry_Weapons);
      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Vehicle_Weapons);
      upgradeTypeSet.insert(UpgradeTypeIds::Terran_Ship_Weapons);
      upgradeTypeSet.insert(UpgradeTypeIds::Zerg_Melee_Attacks);
      upgradeTypeSet.insert(UpgradeTypeIds::Zerg_Missile_Attacks);
      upgradeTypeSet.insert(UpgradeTypeIds::Zerg_Flyer_Attacks);
      upgradeTypeSet.insert(UpgradeTypeIds::Protoss_Ground_Weapons);
      upgradeTypeSet.insert(UpgradeTypeIds::Protoss_Air_Weapons);
      upgradeTypeSet.insert(UpgradeTypeIds::Protoss_Plasma_Shields);
      upgradeTypeSet.insert(UpgradeTypeIds::U_238_Shells);
      upgradeTypeSet.insert(UpgradeTypeIds::Ion_Thrusters);
      upgradeTypeSet.insert(UpgradeTypeIds::Titan_Reactor);
      upgradeTypeSet.insert(UpgradeTypeIds::Ocular_Implants);
      upgradeTypeSet.insert(UpgradeTypeIds::Moebius_Reactor);
      upgradeTypeSet.insert(UpgradeTypeIds::Apollo_Reactor);
      upgradeTypeSet.insert(UpgradeTypeIds::Colossus_Reactor);
      upgradeTypeSet.insert(UpgradeTypeIds::Ventral_Sacs);
      upgradeTypeSet.insert(UpgradeTypeIds::Antennae);
      upgradeTypeSet.insert(UpgradeTypeIds::Pneumatized_Carapace);
      upgradeTypeSet.insert(UpgradeTypeIds::Metabolic_Boost);
      upgradeTypeSet.insert(UpgradeTypeIds::Adrenal_Glands);
      upgradeTypeSet.insert(UpgradeTypeIds::Muscular_Augments);
      upgradeTypeSet.insert(UpgradeTypeIds::Grooved_Spines);
      upgradeTypeSet.insert(UpgradeTypeIds::Gamete_Meiosis);
      upgradeTypeSet.insert(UpgradeTypeIds::Metasynaptic_Node);
      upgradeTypeSet.insert(UpgradeTypeIds::Singularity_Charge);
      upgradeTypeSet.insert(UpgradeTypeIds::Leg_Enhancements);
      upgradeTypeSet.insert(UpgradeTypeIds::Scarab_Damage);
      upgradeTypeSet.insert(UpgradeTypeIds::Reaver_Capacity);
      upgradeTypeSet.insert(UpgradeTypeIds::Gravitic_Drive);
      upgradeTypeSet.insert(UpgradeTypeIds::Sensor_Array);
      upgradeTypeSet.insert(UpgradeTypeIds::Gravitic_Boosters);
      upgradeTypeSet.insert(UpgradeTypeIds::Khaydarin_Amulet);
      upgradeTypeSet.insert(UpgradeTypeIds::Apial_Sensors);
      upgradeTypeSet.insert(UpgradeTypeIds::Gravitic_Thrusters);
      upgradeTypeSet.insert(UpgradeTypeIds::Carrier_Capacity);
      upgradeTypeSet.insert(UpgradeTypeIds::Khaydarin_Core);
      upgradeTypeSet.insert(UpgradeTypeIds::Argus_Jewel);
      upgradeTypeSet.insert(UpgradeTypeIds::Argus_Talisman);
      upgradeTypeSet.insert(UpgradeTypeIds::Caduceus_Reactor);
      upgradeTypeSet.insert(UpgradeTypeIds::Chitinous_Plating);
      upgradeTypeSet.insert(UpgradeTypeIds::Anabolic_Synthesis);
      upgradeTypeSet.insert(UpgradeTypeIds::Charon_Booster);
      upgradeTypeSet.insert(UpgradeTypeIds::None);

      for(std::set<UpgradeTypeId>::iterator i = upgradeTypeSet.begin(); i != upgradeTypeSet.end(); i++)
      {
        upgradeTypeMap.insert(std::make_pair(upgradeTypeData[*i].name, *i));
      }
    }
  }
  namespace UpgradeTypes
  {
    UpgradeTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, UpgradeTypeId>::iterator i = upgradeTypeMap.find(name);
      if (i == upgradeTypeMap.end()) return UpgradeTypeIds::None;
      return (*i).second;
    }
  }
}
