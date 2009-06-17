#pragma once
#include <string>
#include <set>
#include <BWAPI/Race.h>
namespace BWAPI
{
  class UnitType;
  class UpgradeType
  {
  public:
    UpgradeType();
    UpgradeType(int id);
    UpgradeType(const UpgradeType &other);
    UpgradeType& operator=(const UpgradeType &other);
    bool operator==(const UpgradeType &other) const;
    bool operator!=(const UpgradeType &other) const;
    bool operator<(const UpgradeType &other) const;
    int getID() const;
    std::string getName() const;
    Race getRace() const;
    int mineralPriceBase() const;
    int mineralPriceFactor() const;
    int gasPriceBase() const;
    int gasPriceFactor() const;
    int upgradeTimeBase() const;
    int upgradeTimeFactor() const;
    const UnitType* whatUpgrades() const;
  private:
    int id;
  };
  namespace UpgradeTypes
  {
    UpgradeType getUpgradeType(std::string &name);
    std::set<UpgradeType>& allUpgradeTypes();
    void init();
    extern const UpgradeType Terran_Infantry_Armor;
    extern const UpgradeType Terran_Vehicle_Plating;
    extern const UpgradeType Terran_Ship_Plating;
    extern const UpgradeType Zerg_Carapace;
    extern const UpgradeType Zerg_Flyer_Carapace;
    extern const UpgradeType Protoss_Armor;
    extern const UpgradeType Protoss_Plating;
    extern const UpgradeType Terran_Infantry_Weapons;
    extern const UpgradeType Terran_Vehicle_Weapons;
    extern const UpgradeType Terran_Ship_Weapons;
    extern const UpgradeType Zerg_Melee_Attacks;
    extern const UpgradeType Zerg_Missile_Attacks;
    extern const UpgradeType Zerg_Flyer_Attacks;
    extern const UpgradeType Protoss_Ground_Weapons;
    extern const UpgradeType Protoss_Air_Weapons;
    extern const UpgradeType Protoss_Plasma_Shields;
    extern const UpgradeType U_238_Shells;
    extern const UpgradeType Ion_Thrusters;
    extern const UpgradeType Titan_Reactor;
    extern const UpgradeType Ocular_Implants;
    extern const UpgradeType Moebius_Reactor;
    extern const UpgradeType Apollo_Reactor;
    extern const UpgradeType Colossus_Reactor;
    extern const UpgradeType Ventral_Sacs;
    extern const UpgradeType Antennae;
    extern const UpgradeType Pneumatized_Carapace;
    extern const UpgradeType Metabolic_Boost;
    extern const UpgradeType Adrenal_Glands;
    extern const UpgradeType Muscular_Augments;
    extern const UpgradeType Grooved_Spines;
    extern const UpgradeType Gamete_Meiosis;
    extern const UpgradeType Metasynaptic_Node;
    extern const UpgradeType Singularity_Charge;
    extern const UpgradeType Leg_Enhancements;
    extern const UpgradeType Scarab_Damage;
    extern const UpgradeType Reaver_Capacity;
    extern const UpgradeType Gravitic_Drive;
    extern const UpgradeType Sensor_Array;
    extern const UpgradeType Gravitic_Boosters;
    extern const UpgradeType Khaydarin_Amulet;
    extern const UpgradeType Apial_Sensors;
    extern const UpgradeType Gravitic_Thrusters;
    extern const UpgradeType Carrier_Capacity;
    extern const UpgradeType Khaydarin_Core;
    extern const UpgradeType Argus_Jewel;
    extern const UpgradeType Argus_Talisman;
    extern const UpgradeType Caduceus_Reactor;
    extern const UpgradeType Chitinous_Plating;
    extern const UpgradeType Anabolic_Synthesis;
    extern const UpgradeType Charon_Booster;
    extern const UpgradeType None;
    extern const UpgradeType Unknown;
  }
}
