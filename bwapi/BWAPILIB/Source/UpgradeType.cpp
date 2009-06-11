#include <string>
#include <map>
#include <set>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/Race.h>
#include <BW/UnitType.h> // Need to include this until a corresponding BWAPI::UnitType class is added to BWAPILIB

namespace BWAPI
{
  bool initializingUpgradeType=true;
  class UpgradeTypeInternal
  {
  public:
    UpgradeTypeInternal() {valid=false;}
    void set(const char* name, int mineralPriceBase, int mineralPriceFactor, int gasPriceBase, int gasPriceFactor, int upgradeTimeBase, int upgradeTimeFactor, BW::UnitType* whatUpgrades, Race race)
    {
      if (initializingUpgradeType)
      {
        this->name=name;
        this->mineralPriceBase=mineralPriceBase;
        this->mineralPriceFactor=mineralPriceFactor;
        this->gasPriceBase=gasPriceBase;
        this->gasPriceFactor=gasPriceFactor;
        this->upgradeTimeBase=upgradeTimeBase;
        this->upgradeTimeFactor=upgradeTimeFactor;
        this->whatUpgrades=whatUpgrades;
        this->race=race;
        this->valid=true;
      }
    }
    std::string name;
    int mineralPriceBase;
    int mineralPriceFactor;
    int gasPriceBase;
    int gasPriceFactor;
    int upgradeTimeBase;
    int upgradeTimeFactor;
    BW::UnitType* whatUpgrades;
    Race race;
    bool valid;
    static bool initializing;
  };
  UpgradeTypeInternal upgradeTypeData[63];
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
      upgradeTypeData[Terran_Infantry_Armor.getID()].set("Terran Infantry Armor",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Terran_EngineeringBay),Races::Terran);
      upgradeTypeData[Terran_Vehicle_Plating.getID()].set("Terran Vehicle Plating",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Terran_Armory),Races::Terran);
      upgradeTypeData[Terran_Ship_Plating.getID()].set("Terran Ship Plating",150,75,150,75,4000,480,new BW::UnitType(BW::UnitID::Terran_Armory),Races::Terran);
      upgradeTypeData[Zerg_Carapace.getID()].set("Zerg Carapace",150,75,150,75,4000,480,new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber),Races::Zerg);
      upgradeTypeData[Zerg_Flyer_Carapace.getID()].set("Zerg Flyer Carapace",150,75,150,75,4000,480,new BW::UnitType(BW::UnitID::Zerg_Spire),Races::Zerg);
      upgradeTypeData[Protoss_Armor.getID()].set("Protoss Armor",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Protoss_Forge),Races::Protoss);
      upgradeTypeData[Protoss_Plating.getID()].set("Protoss Plating",150,75,150,75,4000,480,new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore),Races::Protoss);
      upgradeTypeData[Terran_Infantry_Weapons.getID()].set("Terran Infantry Weapons",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Terran_EngineeringBay),Races::Terran);
      upgradeTypeData[Terran_Vehicle_Weapons.getID()].set("Terran Vehicle Weapons",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Terran_Armory),Races::Terran);
      upgradeTypeData[Terran_Ship_Weapons.getID()].set("Terran Ship Weapons",100,50,100,50,4000,480,new BW::UnitType(BW::UnitID::Terran_Armory),Races::Terran);
      upgradeTypeData[Zerg_Melee_Attacks.getID()].set("Zerg Melee Attacks",100,50,100,50,4000,480,new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber),Races::Zerg);
      upgradeTypeData[Zerg_Missile_Attacks.getID()].set("Zerg Missile Attacks",100,50,100,50,4000,480,new BW::UnitType(BW::UnitID::Zerg_EvolutionChamber),Races::Zerg);
      upgradeTypeData[Zerg_Flyer_Attacks.getID()].set("Zerg Flyer Attacks",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Zerg_Spire),Races::Zerg);
      upgradeTypeData[Protoss_Ground_Weapons.getID()].set("Protoss Ground Weapons",100,50,100,50,4000,480,new BW::UnitType(BW::UnitID::Protoss_Forge),Races::Protoss);
      upgradeTypeData[Protoss_Air_Weapons.getID()].set("Protoss Air Weapons",100,75,100,75,4000,480,new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore),Races::Protoss);
      upgradeTypeData[Protoss_Plasma_Shields.getID()].set("Protoss Plasma Shields",200,100,200,100,4000,480,new BW::UnitType(BW::UnitID::Protoss_Forge),Races::Protoss);
      upgradeTypeData[U_238_Shells.getID()].set("U-238 Shells",150,0,150,0,1500,0,new BW::UnitType(BW::UnitID::Terran_Academy),Races::Terran);
      upgradeTypeData[Ion_Thrusters.getID()].set("Ion Thrusters",100,0,100,0,1500,0,new BW::UnitType(BW::UnitID::Terran_MachineShop),Races::Terran);
      upgradeTypeData[Titan_Reactor.getID()].set("Titan Reactor",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Terran_ScienceFacility),Races::Terran);
      upgradeTypeData[Ocular_Implants.getID()].set("Ocular Implants",100,0,100,0,2500,0,new BW::UnitType(BW::UnitID::Terran_CovertOps),Races::Terran);
      upgradeTypeData[Moebius_Reactor.getID()].set("Moebius Reactor",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Terran_CovertOps),Races::Terran);
      upgradeTypeData[Apollo_Reactor.getID()].set("Apollo Reactor",200,0,200,0,2500,0,new BW::UnitType(BW::UnitID::Terran_ControlTower),Races::Terran);
      upgradeTypeData[Colossus_Reactor.getID()].set("Colossus Reactor",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Terran_PhysicsLab),Races::Terran);
      upgradeTypeData[Ventral_Sacs.getID()].set("Ventral Sacs",200,0,200,0,2400,0,new BW::UnitType(BW::UnitID::Zerg_Lair),Races::Zerg);
      upgradeTypeData[Antennae.getID()].set("Antennae",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Zerg_Lair),Races::Zerg);
      upgradeTypeData[Pneumatized_Carapace.getID()].set("Pneumatized Carapace",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Zerg_Lair),Races::Zerg);
      upgradeTypeData[Metabolic_Boost.getID()].set("Metabolic Boost",100,0,100,0,1500,0,new BW::UnitType(BW::UnitID::Zerg_SpawningPool),Races::Zerg);
      upgradeTypeData[Adrenal_Glands.getID()].set("Adrenal Glands",200,0,200,0,1500,0,new BW::UnitType(BW::UnitID::Zerg_SpawningPool),Races::Zerg);
      upgradeTypeData[Muscular_Augments.getID()].set("Muscular Augments",150,0,150,0,1500,0,new BW::UnitType(BW::UnitID::Zerg_HydraliskDen),Races::Zerg);
      upgradeTypeData[Grooved_Spines.getID()].set("Grooved Spines",150,0,150,0,1500,0,new BW::UnitType(BW::UnitID::Zerg_HydraliskDen),Races::Zerg);
      upgradeTypeData[Gamete_Meiosis.getID()].set("Gamete Meiosis",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Zerg_QueensNest),Races::Zerg);
      upgradeTypeData[Metasynaptic_Node.getID()].set("Metasynaptic Node",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Zerg_DefilerMound),Races::Zerg);
      upgradeTypeData[Singularity_Charge.getID()].set("Singularity Charge",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_CyberneticsCore),Races::Protoss);
      upgradeTypeData[Leg_Enhancements.getID()].set("Leg Enhancements",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Protoss_CitadelOfAdun),Races::Protoss);
      upgradeTypeData[Scarab_Damage.getID()].set("Scarab Damage",200,0,200,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay),Races::Protoss);
      upgradeTypeData[Reaver_Capacity.getID()].set("Reaver Capacity",200,0,200,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay),Races::Protoss);
      upgradeTypeData[Gravitic_Drive.getID()].set("Gravitic Drive",200,0,200,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_RoboticsSupportBay),Races::Protoss);
      upgradeTypeData[Sensor_Array.getID()].set("Sensor Array",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Protoss_Observatory),Races::Protoss);
      upgradeTypeData[Gravitic_Boosters.getID()].set("Gravitic Boosters",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Protoss_Observatory),Races::Protoss);
      upgradeTypeData[Khaydarin_Amulet.getID()].set("Khaydarin Amulet",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_TemplarArchives),Races::Protoss);
      upgradeTypeData[Apial_Sensors.getID()].set("Apial Sensors",100,0,100,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_TemplarArchives),Races::Protoss);
      upgradeTypeData[Gravitic_Thrusters.getID()].set("Gravitic Thrusters",200,0,200,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_FleetBeacon),Races::Protoss);
      upgradeTypeData[Carrier_Capacity.getID()].set("Carrier Capacity",100,0,100,0,1500,0,new BW::UnitType(BW::UnitID::Protoss_FleetBeacon),Races::Protoss);
      upgradeTypeData[Khaydarin_Core.getID()].set("Khaydarin Core",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_ArbiterTribunal),Races::Protoss);
      upgradeTypeData[Argus_Jewel.getID()].set("Argus Jewel",100,0,100,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_FleetBeacon),Races::Protoss);
      upgradeTypeData[Argus_Talisman.getID()].set("Argus Talisman",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Protoss_TemplarArchives),Races::Protoss);
      upgradeTypeData[Caduceus_Reactor.getID()].set("Caduceus Reactor",150,0,150,0,2500,0,new BW::UnitType(BW::UnitID::Terran_Academy),Races::Terran);
      upgradeTypeData[Chitinous_Plating.getID()].set("Chitinous Plating",150,0,150,0,2000,0,new BW::UnitType(BW::UnitID::Zerg_UltraliskCavern),Races::Zerg);
      upgradeTypeData[Anabolic_Synthesis.getID()].set("Anabolic Synthesis",200,0,200,0,2000,0,new BW::UnitType(BW::UnitID::Zerg_UltraliskCavern),Races::Zerg);
      upgradeTypeData[Charon_Booster.getID()].set("Charon Booster",100,0,100,0,2000,0,new BW::UnitType(BW::UnitID::Terran_MachineShop),Races::Terran);
      upgradeTypeData[None.getID()].set("None",0,0,0,0,0,0,new BW::UnitType(BW::UnitID::None),Races::None);
      upgradeTypeData[Unknown.getID()].set("Unknown",0,0,0,0,0,0,new BW::UnitType(BW::UnitID::None),Races::Unknown);
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
      initializingUpgradeType=false;
    }
  }
  UpgradeType::UpgradeType()
  {
    this->id=UpgradeTypes::None.id;
  }
  UpgradeType::UpgradeType(int id)
  {
    this->id=id;
    if (!initializingUpgradeType)
    {
      if (id<0 || id>=63 || !upgradeTypeData[id].valid)
      {
        this->id=UpgradeTypes::Unknown.id;
      }
    }
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
  BWAPI::Race UpgradeType::getRace() const
  {
    return upgradeTypeData[this->id].race;
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