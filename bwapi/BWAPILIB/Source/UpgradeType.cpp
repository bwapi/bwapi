#include <string>
#include <map>
#include <set>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>
#include <BW/UnitType.h> // Need to include this until a corresponding BWAPI::UnitType class is added to BWAPILIB

namespace BWAPI
{
  struct UpgradeTypeStruct
  {
    std::string name;
    int mineralPriceBase;
    int mineralPriceFactor;
    int gasPriceBase;
    int gasPriceFactor;
    int upgradeTimeBase;
    int upgradeTimeFactor;
    BW::UnitType* whatUpgrades;
    Race race;
  };
  UpgradeTypeStruct upgradeTypeData[63];
  std::map<std::string, UpgradeType> upgradeTypeMap;
  std::set< UpgradeType > upgradeTypeSet;
  namespace UpgradeTypes
  {
    const UpgradeType Terran_Infantry_Armor(0);
    const UpgradeType Terran_Vehicle_Plating(1);
    const UpgradeType Terran_Ship_Plating(2);
    const UpgradeType Zerg_Carapace(3);
    const UpgradeType Zerg_Flyer_Carapace(4);
    const UpgradeType Protoss_Armor(5);
    const UpgradeType Protoss_Plating(6);
    const UpgradeType Terran_Infantry_Weapons(7);
    const UpgradeType Terran_Vehicle_Weapons(8);
    const UpgradeType Terran_Ship_Weapons(9);
    const UpgradeType Zerg_Melee_Attacks(10);
    const UpgradeType Zerg_Missile_Attacks(11);
    const UpgradeType Zerg_Flyer_Attacks(12);
    const UpgradeType Protoss_Ground_Weapons(13);
    const UpgradeType Protoss_Air_Weapons(14);
    const UpgradeType Protoss_Plasma_Shields(15);
    const UpgradeType U_238_Shells(16);
    const UpgradeType Ion_Thrusters(17);
    const UpgradeType Titan_Reactor(19);
    const UpgradeType Ocular_Implants(20);
    const UpgradeType Moebius_Reactor(21);
    const UpgradeType Apollo_Reactor(22);
    const UpgradeType Colossus_Reactor(23);
    const UpgradeType Ventral_Sacs(24);
    const UpgradeType Antennae(25);
    const UpgradeType Pneumatized_Carapace(26);
    const UpgradeType Metabolic_Boost(27);
    const UpgradeType Adrenal_Glands(28);
    const UpgradeType Muscular_Augments(29);
    const UpgradeType Grooved_Spines(30);
    const UpgradeType Gamete_Meiosis(31);
    const UpgradeType Metasynaptic_Node(32);
    const UpgradeType Singularity_Charge(33);
    const UpgradeType Leg_Enhancements(34);
    const UpgradeType Scarab_Damage(35);
    const UpgradeType Reaver_Capacity(36);
    const UpgradeType Gravitic_Drive(37);
    const UpgradeType Sensor_Array(38);
    const UpgradeType Gravitic_Boosters(39);
    const UpgradeType Khaydarin_Amulet(40);
    const UpgradeType Apial_Sensors(41);
    const UpgradeType Gravitic_Thrusters(42);
    const UpgradeType Carrier_Capacity(43);
    const UpgradeType Khaydarin_Core(44);
    const UpgradeType Argus_Jewel(47);
    const UpgradeType Argus_Talisman(49);
    const UpgradeType Caduceus_Reactor(51);
    const UpgradeType Chitinous_Plating(52);
    const UpgradeType Anabolic_Synthesis(53);
    const UpgradeType Charon_Booster(54);
    const UpgradeType None(61);
    const UpgradeType Unknown(62);
    void init()
    {
      upgradeTypeData[Terran_Infantry_Armor.getID()].name="Terran Infantry Armor";
      upgradeTypeData[Terran_Infantry_Armor.getID()].mineralPriceBase=100;
      upgradeTypeData[Terran_Infantry_Armor.getID()].mineralPriceFactor=75;
      upgradeTypeData[Terran_Infantry_Armor.getID()].gasPriceBase=100;
      upgradeTypeData[Terran_Infantry_Armor.getID()].gasPriceFactor=75;
      upgradeTypeData[Terran_Infantry_Armor.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Infantry_Armor.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Infantry_Armor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_EngineeringBay);
      upgradeTypeData[Terran_Infantry_Armor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].name="Terran Vehicle Plating";
      upgradeTypeData[Terran_Vehicle_Plating.getID()].mineralPriceBase=100;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].mineralPriceFactor=75;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].gasPriceBase=100;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].gasPriceFactor=75;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Vehicle_Plating.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Armory);
      upgradeTypeData[Terran_Vehicle_Plating.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Terran_Ship_Plating.getID()].name="Terran Ship Plating";
      upgradeTypeData[Terran_Ship_Plating.getID()].mineralPriceBase=150;
      upgradeTypeData[Terran_Ship_Plating.getID()].mineralPriceFactor=75;
      upgradeTypeData[Terran_Ship_Plating.getID()].gasPriceBase=150;
      upgradeTypeData[Terran_Ship_Plating.getID()].gasPriceFactor=75;
      upgradeTypeData[Terran_Ship_Plating.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Ship_Plating.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Ship_Plating.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Armory);
      upgradeTypeData[Terran_Ship_Plating.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Zerg_Carapace.getID()].name="Zerg Carapace";
      upgradeTypeData[Zerg_Carapace.getID()].mineralPriceBase=150;
      upgradeTypeData[Zerg_Carapace.getID()].mineralPriceFactor=75;
      upgradeTypeData[Zerg_Carapace.getID()].gasPriceBase=150;
      upgradeTypeData[Zerg_Carapace.getID()].gasPriceFactor=75;
      upgradeTypeData[Zerg_Carapace.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Zerg_Carapace.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Zerg_Carapace.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber);
      upgradeTypeData[Zerg_Carapace.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].name="Zerg Flyer Carapace";
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].mineralPriceBase=150;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].mineralPriceFactor=75;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].gasPriceBase=150;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].gasPriceFactor=75;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_Spire);
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Protoss_Armor.getID()].name="Protoss Armor";
      upgradeTypeData[Protoss_Armor.getID()].mineralPriceBase=100;
      upgradeTypeData[Protoss_Armor.getID()].mineralPriceFactor=75;
      upgradeTypeData[Protoss_Armor.getID()].gasPriceBase=100;
      upgradeTypeData[Protoss_Armor.getID()].gasPriceFactor=75;
      upgradeTypeData[Protoss_Armor.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Protoss_Armor.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Protoss_Armor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_Forge);
      upgradeTypeData[Protoss_Armor.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Protoss_Plating.getID()].name="Protoss Plating";
      upgradeTypeData[Protoss_Plating.getID()].mineralPriceBase=150;
      upgradeTypeData[Protoss_Plating.getID()].mineralPriceFactor=75;
      upgradeTypeData[Protoss_Plating.getID()].gasPriceBase=150;
      upgradeTypeData[Protoss_Plating.getID()].gasPriceFactor=75;
      upgradeTypeData[Protoss_Plating.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Protoss_Plating.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Protoss_Plating.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore);
      upgradeTypeData[Protoss_Plating.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].name="Terran Infantry Weapons";
      upgradeTypeData[Terran_Infantry_Weapons.getID()].mineralPriceBase=100;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].mineralPriceFactor=75;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].gasPriceBase=100;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].gasPriceFactor=75;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Infantry_Weapons.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_EngineeringBay);
      upgradeTypeData[Terran_Infantry_Weapons.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].name="Terran Vehicle Weapons";
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].mineralPriceBase=100;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].mineralPriceFactor=75;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].gasPriceBase=100;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].gasPriceFactor=75;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Armory);
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Terran_Ship_Weapons.getID()].name="Terran Ship Weapons";
      upgradeTypeData[Terran_Ship_Weapons.getID()].mineralPriceBase=100;
      upgradeTypeData[Terran_Ship_Weapons.getID()].mineralPriceFactor=50;
      upgradeTypeData[Terran_Ship_Weapons.getID()].gasPriceBase=100;
      upgradeTypeData[Terran_Ship_Weapons.getID()].gasPriceFactor=50;
      upgradeTypeData[Terran_Ship_Weapons.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Terran_Ship_Weapons.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Terran_Ship_Weapons.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Armory);
      upgradeTypeData[Terran_Ship_Weapons.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].name="Zerg Melee Attacks";
      upgradeTypeData[Zerg_Melee_Attacks.getID()].mineralPriceBase=100;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].mineralPriceFactor=50;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].gasPriceBase=100;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].gasPriceFactor=50;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Zerg_Melee_Attacks.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber);
      upgradeTypeData[Zerg_Melee_Attacks.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].name="Zerg Missile Attacks";
      upgradeTypeData[Zerg_Missile_Attacks.getID()].mineralPriceBase=100;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].mineralPriceFactor=50;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].gasPriceBase=100;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].gasPriceFactor=50;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Zerg_Missile_Attacks.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber);
      upgradeTypeData[Zerg_Missile_Attacks.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].name="Zerg Flyer Attacks";
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].mineralPriceBase=100;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].mineralPriceFactor=75;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].gasPriceBase=100;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].gasPriceFactor=75;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_Spire);
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].name="Protoss Ground Weapons";
      upgradeTypeData[Protoss_Ground_Weapons.getID()].mineralPriceBase=100;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].mineralPriceFactor=50;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].gasPriceBase=100;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].gasPriceFactor=50;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Protoss_Ground_Weapons.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_Forge);
      upgradeTypeData[Protoss_Ground_Weapons.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Protoss_Air_Weapons.getID()].name="Protoss Air Weapons";
      upgradeTypeData[Protoss_Air_Weapons.getID()].mineralPriceBase=100;
      upgradeTypeData[Protoss_Air_Weapons.getID()].mineralPriceFactor=75;
      upgradeTypeData[Protoss_Air_Weapons.getID()].gasPriceBase=100;
      upgradeTypeData[Protoss_Air_Weapons.getID()].gasPriceFactor=75;
      upgradeTypeData[Protoss_Air_Weapons.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Protoss_Air_Weapons.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Protoss_Air_Weapons.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore);
      upgradeTypeData[Protoss_Air_Weapons.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].name="Protoss Plasma Shields";
      upgradeTypeData[Protoss_Plasma_Shields.getID()].mineralPriceBase=200;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].mineralPriceFactor=100;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].gasPriceBase=200;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].gasPriceFactor=100;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].upgradeTimeBase=4000;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].upgradeTimeFactor=480;
      upgradeTypeData[Protoss_Plasma_Shields.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_Forge);
      upgradeTypeData[Protoss_Plasma_Shields.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[U_238_Shells.getID()].name="U-238 Shells";
      upgradeTypeData[U_238_Shells.getID()].mineralPriceBase=150;
      upgradeTypeData[U_238_Shells.getID()].mineralPriceFactor=0;
      upgradeTypeData[U_238_Shells.getID()].gasPriceBase=150;
      upgradeTypeData[U_238_Shells.getID()].gasPriceFactor=0;
      upgradeTypeData[U_238_Shells.getID()].upgradeTimeBase=1500;
      upgradeTypeData[U_238_Shells.getID()].upgradeTimeFactor=0;
      upgradeTypeData[U_238_Shells.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Academy);
      upgradeTypeData[U_238_Shells.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Ion_Thrusters.getID()].name="Ion Thrusters";
      upgradeTypeData[Ion_Thrusters.getID()].mineralPriceBase=100;
      upgradeTypeData[Ion_Thrusters.getID()].mineralPriceFactor=0;
      upgradeTypeData[Ion_Thrusters.getID()].gasPriceBase=100;
      upgradeTypeData[Ion_Thrusters.getID()].gasPriceFactor=0;
      upgradeTypeData[Ion_Thrusters.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Ion_Thrusters.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Ion_Thrusters.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_MachineShop);
      upgradeTypeData[Ion_Thrusters.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Titan_Reactor.getID()].name="Titan Reactor";
      upgradeTypeData[Titan_Reactor.getID()].mineralPriceBase=150;
      upgradeTypeData[Titan_Reactor.getID()].mineralPriceFactor=0;
      upgradeTypeData[Titan_Reactor.getID()].gasPriceBase=150;
      upgradeTypeData[Titan_Reactor.getID()].gasPriceFactor=0;
      upgradeTypeData[Titan_Reactor.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Titan_Reactor.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Titan_Reactor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_ScienceFacility);
      upgradeTypeData[Titan_Reactor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Ocular_Implants.getID()].name="Ocular Implants";
      upgradeTypeData[Ocular_Implants.getID()].mineralPriceBase=100;
      upgradeTypeData[Ocular_Implants.getID()].mineralPriceFactor=0;
      upgradeTypeData[Ocular_Implants.getID()].gasPriceBase=100;
      upgradeTypeData[Ocular_Implants.getID()].gasPriceFactor=0;
      upgradeTypeData[Ocular_Implants.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Ocular_Implants.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Ocular_Implants.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_CovertOps);
      upgradeTypeData[Ocular_Implants.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Moebius_Reactor.getID()].name="Moebius Reactor";
      upgradeTypeData[Moebius_Reactor.getID()].mineralPriceBase=150;
      upgradeTypeData[Moebius_Reactor.getID()].mineralPriceFactor=0;
      upgradeTypeData[Moebius_Reactor.getID()].gasPriceBase=150;
      upgradeTypeData[Moebius_Reactor.getID()].gasPriceFactor=0;
      upgradeTypeData[Moebius_Reactor.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Moebius_Reactor.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Moebius_Reactor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_CovertOps);
      upgradeTypeData[Moebius_Reactor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Apollo_Reactor.getID()].name="Apollo Reactor";
      upgradeTypeData[Apollo_Reactor.getID()].mineralPriceBase=200;
      upgradeTypeData[Apollo_Reactor.getID()].mineralPriceFactor=0;
      upgradeTypeData[Apollo_Reactor.getID()].gasPriceBase=200;
      upgradeTypeData[Apollo_Reactor.getID()].gasPriceFactor=0;
      upgradeTypeData[Apollo_Reactor.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Apollo_Reactor.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Apollo_Reactor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_ControlTower);
      upgradeTypeData[Apollo_Reactor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Colossus_Reactor.getID()].name="Colossus Reactor";
      upgradeTypeData[Colossus_Reactor.getID()].mineralPriceBase=150;
      upgradeTypeData[Colossus_Reactor.getID()].mineralPriceFactor=0;
      upgradeTypeData[Colossus_Reactor.getID()].gasPriceBase=150;
      upgradeTypeData[Colossus_Reactor.getID()].gasPriceFactor=0;
      upgradeTypeData[Colossus_Reactor.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Colossus_Reactor.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Colossus_Reactor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_PhysicsLab);
      upgradeTypeData[Colossus_Reactor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Ventral_Sacs.getID()].name="Ventral Sacs";
      upgradeTypeData[Ventral_Sacs.getID()].mineralPriceBase=200;
      upgradeTypeData[Ventral_Sacs.getID()].mineralPriceFactor=0;
      upgradeTypeData[Ventral_Sacs.getID()].gasPriceBase=200;
      upgradeTypeData[Ventral_Sacs.getID()].gasPriceFactor=0;
      upgradeTypeData[Ventral_Sacs.getID()].upgradeTimeBase=2400;
      upgradeTypeData[Ventral_Sacs.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Ventral_Sacs.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_Lair);
      upgradeTypeData[Ventral_Sacs.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Antennae.getID()].name="Antennae";
      upgradeTypeData[Antennae.getID()].mineralPriceBase=150;
      upgradeTypeData[Antennae.getID()].mineralPriceFactor=0;
      upgradeTypeData[Antennae.getID()].gasPriceBase=150;
      upgradeTypeData[Antennae.getID()].gasPriceFactor=0;
      upgradeTypeData[Antennae.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Antennae.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Antennae.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_Lair);
      upgradeTypeData[Antennae.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Pneumatized_Carapace.getID()].name="Pneumatized Carapace";
      upgradeTypeData[Pneumatized_Carapace.getID()].mineralPriceBase=150;
      upgradeTypeData[Pneumatized_Carapace.getID()].mineralPriceFactor=0;
      upgradeTypeData[Pneumatized_Carapace.getID()].gasPriceBase=150;
      upgradeTypeData[Pneumatized_Carapace.getID()].gasPriceFactor=0;
      upgradeTypeData[Pneumatized_Carapace.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Pneumatized_Carapace.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Pneumatized_Carapace.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_Lair);
      upgradeTypeData[Pneumatized_Carapace.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Metabolic_Boost.getID()].name="Metabolic Boost";
      upgradeTypeData[Metabolic_Boost.getID()].mineralPriceBase=100;
      upgradeTypeData[Metabolic_Boost.getID()].mineralPriceFactor=0;
      upgradeTypeData[Metabolic_Boost.getID()].gasPriceBase=100;
      upgradeTypeData[Metabolic_Boost.getID()].gasPriceFactor=0;
      upgradeTypeData[Metabolic_Boost.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Metabolic_Boost.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Metabolic_Boost.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_SpawningPool);
      upgradeTypeData[Metabolic_Boost.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Adrenal_Glands.getID()].name="Adrenal Glands";
      upgradeTypeData[Adrenal_Glands.getID()].mineralPriceBase=200;
      upgradeTypeData[Adrenal_Glands.getID()].mineralPriceFactor=0;
      upgradeTypeData[Adrenal_Glands.getID()].gasPriceBase=200;
      upgradeTypeData[Adrenal_Glands.getID()].gasPriceFactor=0;
      upgradeTypeData[Adrenal_Glands.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Adrenal_Glands.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Adrenal_Glands.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_SpawningPool);
      upgradeTypeData[Adrenal_Glands.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Muscular_Augments.getID()].name="Muscular Augments";
      upgradeTypeData[Muscular_Augments.getID()].mineralPriceBase=150;
      upgradeTypeData[Muscular_Augments.getID()].mineralPriceFactor=0;
      upgradeTypeData[Muscular_Augments.getID()].gasPriceBase=150;
      upgradeTypeData[Muscular_Augments.getID()].gasPriceFactor=0;
      upgradeTypeData[Muscular_Augments.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Muscular_Augments.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Muscular_Augments.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_HydraliskDen);
      upgradeTypeData[Muscular_Augments.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Grooved_Spines.getID()].name="Grooved Spines";
      upgradeTypeData[Grooved_Spines.getID()].mineralPriceBase=150;
      upgradeTypeData[Grooved_Spines.getID()].mineralPriceFactor=0;
      upgradeTypeData[Grooved_Spines.getID()].gasPriceBase=150;
      upgradeTypeData[Grooved_Spines.getID()].gasPriceFactor=0;
      upgradeTypeData[Grooved_Spines.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Grooved_Spines.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Grooved_Spines.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_HydraliskDen);
      upgradeTypeData[Grooved_Spines.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Gamete_Meiosis.getID()].name="Gamete Meiosis";
      upgradeTypeData[Gamete_Meiosis.getID()].mineralPriceBase=150;
      upgradeTypeData[Gamete_Meiosis.getID()].mineralPriceFactor=0;
      upgradeTypeData[Gamete_Meiosis.getID()].gasPriceBase=150;
      upgradeTypeData[Gamete_Meiosis.getID()].gasPriceFactor=0;
      upgradeTypeData[Gamete_Meiosis.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Gamete_Meiosis.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Gamete_Meiosis.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_QueensNest);
      upgradeTypeData[Gamete_Meiosis.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Metasynaptic_Node.getID()].name="Metasynaptic Node";
      upgradeTypeData[Metasynaptic_Node.getID()].mineralPriceBase=150;
      upgradeTypeData[Metasynaptic_Node.getID()].mineralPriceFactor=0;
      upgradeTypeData[Metasynaptic_Node.getID()].gasPriceBase=150;
      upgradeTypeData[Metasynaptic_Node.getID()].gasPriceFactor=0;
      upgradeTypeData[Metasynaptic_Node.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Metasynaptic_Node.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Metasynaptic_Node.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_DefilerMound);
      upgradeTypeData[Metasynaptic_Node.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Singularity_Charge.getID()].name="Singularity Charge";
      upgradeTypeData[Singularity_Charge.getID()].mineralPriceBase=150;
      upgradeTypeData[Singularity_Charge.getID()].mineralPriceFactor=0;
      upgradeTypeData[Singularity_Charge.getID()].gasPriceBase=150;
      upgradeTypeData[Singularity_Charge.getID()].gasPriceFactor=0;
      upgradeTypeData[Singularity_Charge.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Singularity_Charge.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Singularity_Charge.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore);
      upgradeTypeData[Singularity_Charge.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Leg_Enhancements.getID()].name="Leg Enhancements";
      upgradeTypeData[Leg_Enhancements.getID()].mineralPriceBase=150;
      upgradeTypeData[Leg_Enhancements.getID()].mineralPriceFactor=0;
      upgradeTypeData[Leg_Enhancements.getID()].gasPriceBase=150;
      upgradeTypeData[Leg_Enhancements.getID()].gasPriceFactor=0;
      upgradeTypeData[Leg_Enhancements.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Leg_Enhancements.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Leg_Enhancements.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_CitadelOfAdun);
      upgradeTypeData[Leg_Enhancements.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Scarab_Damage.getID()].name="Scarab Damage";
      upgradeTypeData[Scarab_Damage.getID()].mineralPriceBase=200;
      upgradeTypeData[Scarab_Damage.getID()].mineralPriceFactor=0;
      upgradeTypeData[Scarab_Damage.getID()].gasPriceBase=200;
      upgradeTypeData[Scarab_Damage.getID()].gasPriceFactor=0;
      upgradeTypeData[Scarab_Damage.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Scarab_Damage.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Scarab_Damage.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay);
      upgradeTypeData[Scarab_Damage.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Reaver_Capacity.getID()].name="Reaver Capacity";
      upgradeTypeData[Reaver_Capacity.getID()].mineralPriceBase=200;
      upgradeTypeData[Reaver_Capacity.getID()].mineralPriceFactor=0;
      upgradeTypeData[Reaver_Capacity.getID()].gasPriceBase=200;
      upgradeTypeData[Reaver_Capacity.getID()].gasPriceFactor=0;
      upgradeTypeData[Reaver_Capacity.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Reaver_Capacity.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Reaver_Capacity.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay);
      upgradeTypeData[Reaver_Capacity.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Gravitic_Drive.getID()].name="Gravitic Drive";
      upgradeTypeData[Gravitic_Drive.getID()].mineralPriceBase=200;
      upgradeTypeData[Gravitic_Drive.getID()].mineralPriceFactor=0;
      upgradeTypeData[Gravitic_Drive.getID()].gasPriceBase=200;
      upgradeTypeData[Gravitic_Drive.getID()].gasPriceFactor=0;
      upgradeTypeData[Gravitic_Drive.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Gravitic_Drive.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Gravitic_Drive.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay);
      upgradeTypeData[Gravitic_Drive.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Sensor_Array.getID()].name="Sensor Array";
      upgradeTypeData[Sensor_Array.getID()].mineralPriceBase=150;
      upgradeTypeData[Sensor_Array.getID()].mineralPriceFactor=0;
      upgradeTypeData[Sensor_Array.getID()].gasPriceBase=150;
      upgradeTypeData[Sensor_Array.getID()].gasPriceFactor=0;
      upgradeTypeData[Sensor_Array.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Sensor_Array.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Sensor_Array.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_Observatory);
      upgradeTypeData[Sensor_Array.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Gravitic_Boosters.getID()].name="Gravitic Boosters";
      upgradeTypeData[Gravitic_Boosters.getID()].mineralPriceBase=150;
      upgradeTypeData[Gravitic_Boosters.getID()].mineralPriceFactor=0;
      upgradeTypeData[Gravitic_Boosters.getID()].gasPriceBase=150;
      upgradeTypeData[Gravitic_Boosters.getID()].gasPriceFactor=0;
      upgradeTypeData[Gravitic_Boosters.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Gravitic_Boosters.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Gravitic_Boosters.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_Observatory);
      upgradeTypeData[Gravitic_Boosters.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Khaydarin_Amulet.getID()].name="Khaydarin Amulet";
      upgradeTypeData[Khaydarin_Amulet.getID()].mineralPriceBase=150;
      upgradeTypeData[Khaydarin_Amulet.getID()].mineralPriceFactor=0;
      upgradeTypeData[Khaydarin_Amulet.getID()].gasPriceBase=150;
      upgradeTypeData[Khaydarin_Amulet.getID()].gasPriceFactor=0;
      upgradeTypeData[Khaydarin_Amulet.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Khaydarin_Amulet.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Khaydarin_Amulet.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      upgradeTypeData[Khaydarin_Amulet.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Apial_Sensors.getID()].name="Apial Sensors";
      upgradeTypeData[Apial_Sensors.getID()].mineralPriceBase=100;
      upgradeTypeData[Apial_Sensors.getID()].mineralPriceFactor=0;
      upgradeTypeData[Apial_Sensors.getID()].gasPriceBase=100;
      upgradeTypeData[Apial_Sensors.getID()].gasPriceFactor=0;
      upgradeTypeData[Apial_Sensors.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Apial_Sensors.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Apial_Sensors.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      upgradeTypeData[Apial_Sensors.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Gravitic_Thrusters.getID()].name="Gravitic Thrusters";
      upgradeTypeData[Gravitic_Thrusters.getID()].mineralPriceBase=200;
      upgradeTypeData[Gravitic_Thrusters.getID()].mineralPriceFactor=0;
      upgradeTypeData[Gravitic_Thrusters.getID()].gasPriceBase=200;
      upgradeTypeData[Gravitic_Thrusters.getID()].gasPriceFactor=0;
      upgradeTypeData[Gravitic_Thrusters.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Gravitic_Thrusters.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Gravitic_Thrusters.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_FleetBeacon);
      upgradeTypeData[Gravitic_Thrusters.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Carrier_Capacity.getID()].name="Carrier Capacity";
      upgradeTypeData[Carrier_Capacity.getID()].mineralPriceBase=100;
      upgradeTypeData[Carrier_Capacity.getID()].mineralPriceFactor=0;
      upgradeTypeData[Carrier_Capacity.getID()].gasPriceBase=100;
      upgradeTypeData[Carrier_Capacity.getID()].gasPriceFactor=0;
      upgradeTypeData[Carrier_Capacity.getID()].upgradeTimeBase=1500;
      upgradeTypeData[Carrier_Capacity.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Carrier_Capacity.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_FleetBeacon);
      upgradeTypeData[Carrier_Capacity.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Khaydarin_Core.getID()].name="Khaydarin Core";
      upgradeTypeData[Khaydarin_Core.getID()].mineralPriceBase=150;
      upgradeTypeData[Khaydarin_Core.getID()].mineralPriceFactor=0;
      upgradeTypeData[Khaydarin_Core.getID()].gasPriceBase=150;
      upgradeTypeData[Khaydarin_Core.getID()].gasPriceFactor=0;
      upgradeTypeData[Khaydarin_Core.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Khaydarin_Core.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Khaydarin_Core.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_ArbiterTribunal);
      upgradeTypeData[Khaydarin_Core.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Argus_Jewel.getID()].name="Argus Jewel";
      upgradeTypeData[Argus_Jewel.getID()].mineralPriceBase=100;
      upgradeTypeData[Argus_Jewel.getID()].mineralPriceFactor=0;
      upgradeTypeData[Argus_Jewel.getID()].gasPriceBase=100;
      upgradeTypeData[Argus_Jewel.getID()].gasPriceFactor=0;
      upgradeTypeData[Argus_Jewel.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Argus_Jewel.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Argus_Jewel.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_FleetBeacon);
      upgradeTypeData[Argus_Jewel.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Argus_Talisman.getID()].name="Argus Talisman";
      upgradeTypeData[Argus_Talisman.getID()].mineralPriceBase=150;
      upgradeTypeData[Argus_Talisman.getID()].mineralPriceFactor=0;
      upgradeTypeData[Argus_Talisman.getID()].gasPriceBase=150;
      upgradeTypeData[Argus_Talisman.getID()].gasPriceFactor=0;
      upgradeTypeData[Argus_Talisman.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Argus_Talisman.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Argus_Talisman.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Protoss_TemplarArchives);
      upgradeTypeData[Argus_Talisman.getID()].race=BWAPI::Races::Protoss;
      upgradeTypeData[Caduceus_Reactor.getID()].name="Caduceus Reactor";
      upgradeTypeData[Caduceus_Reactor.getID()].mineralPriceBase=150;
      upgradeTypeData[Caduceus_Reactor.getID()].mineralPriceFactor=0;
      upgradeTypeData[Caduceus_Reactor.getID()].gasPriceBase=150;
      upgradeTypeData[Caduceus_Reactor.getID()].gasPriceFactor=0;
      upgradeTypeData[Caduceus_Reactor.getID()].upgradeTimeBase=2500;
      upgradeTypeData[Caduceus_Reactor.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Caduceus_Reactor.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_Academy);
      upgradeTypeData[Caduceus_Reactor.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[Chitinous_Plating.getID()].name="Chitinous Plating";
      upgradeTypeData[Chitinous_Plating.getID()].mineralPriceBase=150;
      upgradeTypeData[Chitinous_Plating.getID()].mineralPriceFactor=0;
      upgradeTypeData[Chitinous_Plating.getID()].gasPriceBase=150;
      upgradeTypeData[Chitinous_Plating.getID()].gasPriceFactor=0;
      upgradeTypeData[Chitinous_Plating.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Chitinous_Plating.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Chitinous_Plating.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_UltraliskCavern);
      upgradeTypeData[Chitinous_Plating.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Anabolic_Synthesis.getID()].name="Anabolic Synthesis";
      upgradeTypeData[Anabolic_Synthesis.getID()].mineralPriceBase=200;
      upgradeTypeData[Anabolic_Synthesis.getID()].mineralPriceFactor=0;
      upgradeTypeData[Anabolic_Synthesis.getID()].gasPriceBase=200;
      upgradeTypeData[Anabolic_Synthesis.getID()].gasPriceFactor=0;
      upgradeTypeData[Anabolic_Synthesis.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Anabolic_Synthesis.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Anabolic_Synthesis.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Zerg_UltraliskCavern);
      upgradeTypeData[Anabolic_Synthesis.getID()].race=BWAPI::Races::Zerg;
      upgradeTypeData[Charon_Booster.getID()].name="Charon Booster";
      upgradeTypeData[Charon_Booster.getID()].mineralPriceBase=100;
      upgradeTypeData[Charon_Booster.getID()].mineralPriceFactor=0;
      upgradeTypeData[Charon_Booster.getID()].gasPriceBase=100;
      upgradeTypeData[Charon_Booster.getID()].gasPriceFactor=0;
      upgradeTypeData[Charon_Booster.getID()].upgradeTimeBase=2000;
      upgradeTypeData[Charon_Booster.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Charon_Booster.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::Terran_MachineShop);
      upgradeTypeData[Charon_Booster.getID()].race=BWAPI::Races::Terran;
      upgradeTypeData[None.getID()].name="None";
      upgradeTypeData[None.getID()].mineralPriceBase=0;
      upgradeTypeData[None.getID()].mineralPriceFactor=0;
      upgradeTypeData[None.getID()].gasPriceBase=0;
      upgradeTypeData[None.getID()].gasPriceFactor=0;
      upgradeTypeData[None.getID()].upgradeTimeBase=0;
      upgradeTypeData[None.getID()].upgradeTimeFactor=0;
      upgradeTypeData[None.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::None);
      upgradeTypeData[None.getID()].race=BWAPI::Races::None;
      upgradeTypeData[Unknown.getID()].name="Unknown";
      upgradeTypeData[Unknown.getID()].mineralPriceBase=0;
      upgradeTypeData[Unknown.getID()].mineralPriceFactor=0;
      upgradeTypeData[Unknown.getID()].gasPriceBase=0;
      upgradeTypeData[Unknown.getID()].gasPriceFactor=0;
      upgradeTypeData[Unknown.getID()].upgradeTimeBase=0;
      upgradeTypeData[Unknown.getID()].upgradeTimeFactor=0;
      upgradeTypeData[Unknown.getID()].whatUpgrades=new BW::UnitType(BW::UnitID::None);
      upgradeTypeData[Unknown.getID()].race=BWAPI::Races::Unknown;
      upgradeTypeSet.insert(Terran_Infantry_Armor);
      upgradeTypeSet.insert(Terran_Vehicle_Plating);
      upgradeTypeSet.insert(Terran_Ship_Plating);
      upgradeTypeSet.insert(Zerg_Carapace);
      upgradeTypeSet.insert(Zerg_Flyer_Carapace);
      upgradeTypeSet.insert(Protoss_Armor);
      upgradeTypeSet.insert(Protoss_Plating);
      upgradeTypeSet.insert(Terran_Infantry_Weapons);
      upgradeTypeSet.insert(Terran_Vehicle_Weapons);
      upgradeTypeSet.insert(Terran_Ship_Weapons);
      upgradeTypeSet.insert(Zerg_Melee_Attacks);
      upgradeTypeSet.insert(Zerg_Missile_Attacks);
      upgradeTypeSet.insert(Zerg_Flyer_Attacks);
      upgradeTypeSet.insert(Protoss_Ground_Weapons);
      upgradeTypeSet.insert(Protoss_Air_Weapons);
      upgradeTypeSet.insert(Protoss_Plasma_Shields);
      upgradeTypeSet.insert(U_238_Shells);
      upgradeTypeSet.insert(Ion_Thrusters);
      upgradeTypeSet.insert(Titan_Reactor);
      upgradeTypeSet.insert(Ocular_Implants);
      upgradeTypeSet.insert(Moebius_Reactor);
      upgradeTypeSet.insert(Apollo_Reactor);
      upgradeTypeSet.insert(Colossus_Reactor);
      upgradeTypeSet.insert(Ventral_Sacs);
      upgradeTypeSet.insert(Antennae);
      upgradeTypeSet.insert(Pneumatized_Carapace);
      upgradeTypeSet.insert(Metabolic_Boost);
      upgradeTypeSet.insert(Adrenal_Glands);
      upgradeTypeSet.insert(Muscular_Augments);
      upgradeTypeSet.insert(Grooved_Spines);
      upgradeTypeSet.insert(Gamete_Meiosis);
      upgradeTypeSet.insert(Metasynaptic_Node);
      upgradeTypeSet.insert(Singularity_Charge);
      upgradeTypeSet.insert(Leg_Enhancements);
      upgradeTypeSet.insert(Scarab_Damage);
      upgradeTypeSet.insert(Reaver_Capacity);
      upgradeTypeSet.insert(Gravitic_Drive);
      upgradeTypeSet.insert(Sensor_Array);
      upgradeTypeSet.insert(Gravitic_Boosters);
      upgradeTypeSet.insert(Khaydarin_Amulet);
      upgradeTypeSet.insert(Apial_Sensors);
      upgradeTypeSet.insert(Gravitic_Thrusters);
      upgradeTypeSet.insert(Carrier_Capacity);
      upgradeTypeSet.insert(Khaydarin_Core);
      upgradeTypeSet.insert(Argus_Jewel);
      upgradeTypeSet.insert(Argus_Talisman);
      upgradeTypeSet.insert(Caduceus_Reactor);
      upgradeTypeSet.insert(Chitinous_Plating);
      upgradeTypeSet.insert(Anabolic_Synthesis);
      upgradeTypeSet.insert(Charon_Booster);
      upgradeTypeSet.insert(None);
      upgradeTypeSet.insert(Unknown);

      for(std::set<UpgradeType>::iterator i=upgradeTypeSet.begin();i!=upgradeTypeSet.end();i++)
      {
        upgradeTypeMap.insert(std::make_pair((*i).getName(),*i));
      }
    }
  }
  UpgradeType::UpgradeType()
  {
    this->id=UpgradeTypes::None.id;
  }
  UpgradeType::UpgradeType(int id)
  {
    this->id=id;
  }
  UpgradeType::UpgradeType(const UpgradeType &other)
  {
    this->id=other.id;
  }
  UpgradeType& UpgradeType::operator=(const UpgradeType &other)
  {
    this->id=other.id;
    return *this;
  }
  bool UpgradeType::operator==(const UpgradeType &other) const
  {
    return this->id==other.id;
  }
  bool UpgradeType::operator!=(const UpgradeType &other) const
  {
    return this->id!=other.id;
  }
  bool UpgradeType::operator<(const UpgradeType &other) const
  {
    return this->id<other.id;
  }
  int UpgradeType::getID() const
  {
    return this->id;
  }
  std::string UpgradeType::getName() const
  {
    return upgradeTypeData[this->id].name;
  }
  int UpgradeType::mineralPriceBase() const
  {
    return upgradeTypeData[this->id].mineralPriceBase;
  }
  int UpgradeType::mineralPriceFactor() const
  {
    return upgradeTypeData[this->id].mineralPriceFactor;
  }
  int UpgradeType::gasPriceBase() const
  {
    return upgradeTypeData[this->id].gasPriceBase;
  }
  int UpgradeType::gasPriceFactor() const
  {
    return upgradeTypeData[this->id].gasPriceFactor;
  }
  int UpgradeType::upgradeTimeBase() const
  {
    return upgradeTypeData[this->id].upgradeTimeBase;
  }
  int UpgradeType::upgradeTimeFactor() const
  {
    return upgradeTypeData[this->id].upgradeTimeFactor;
  }
  BW::UnitType* UpgradeType::whatUpgrades() const
  {
    return upgradeTypeData[this->id].whatUpgrades;
  }
  UpgradeType UpgradeTypes::getUpgradeType(std::string &name)
  {
    std::map<std::string, UpgradeType>::iterator i=upgradeTypeMap.find(name);
    if (i==upgradeTypeMap.end()) return UpgradeTypes::Unknown;
    return (*i).second;
  }
  std::set<UpgradeType>& UpgradeTypes::allUpgradeTypes()
  {
    return upgradeTypeSet;
  }
}