#pragma once

#include "BWSLInterface.h"
#include "Race.h"

#include <string>
#include <set>
namespace BWSL
{
  class UnitType;
  class BWSL_EXPORT UpgradeType
  {
    public:
      UpgradeType();
      UpgradeType(int id);
      UpgradeType(const UpgradeType& other);
      UpgradeType& operator=(const UpgradeType& other);
      bool operator==(const UpgradeType& other) const;
      bool operator!=(const UpgradeType& other) const;
      bool operator<(const UpgradeType& other) const;
      int getID() const;
      std::string getName() const;
      Race getRace() const;
      int mineralPriceBase() const;
      int mineralPriceFactor() const;
      int gasPriceBase() const;
      int gasPriceFactor() const;
      int upgradeTimeBase() const;
      int upgradeTimeFactor() const;
      int maxRepeats() const;
      const UnitType* whatUpgrades() const;
      const std::set<const UnitType*>& whatUses() const;
    private:
      int id;
  };
  namespace UpgradeTypes
  {
    UpgradeType getUpgradeType(std::string& name);
    std::set<UpgradeType>& allUpgradeTypes();
    void init();
    BWSL_EXPORT extern const UpgradeType Terran_Infantry_Armor;
    BWSL_EXPORT extern const UpgradeType Terran_Vehicle_Plating;
    BWSL_EXPORT extern const UpgradeType Terran_Ship_Plating;
    BWSL_EXPORT extern const UpgradeType Zerg_Carapace;
    BWSL_EXPORT extern const UpgradeType Zerg_Flyer_Carapace;
    BWSL_EXPORT extern const UpgradeType Protoss_Armor;
    BWSL_EXPORT extern const UpgradeType Protoss_Plating;
    BWSL_EXPORT extern const UpgradeType Terran_Infantry_Weapons;
    BWSL_EXPORT extern const UpgradeType Terran_Vehicle_Weapons;
    BWSL_EXPORT extern const UpgradeType Terran_Ship_Weapons;
    BWSL_EXPORT extern const UpgradeType Zerg_Melee_Attacks;
    BWSL_EXPORT extern const UpgradeType Zerg_Missile_Attacks;
    BWSL_EXPORT extern const UpgradeType Zerg_Flyer_Attacks;
    BWSL_EXPORT extern const UpgradeType Protoss_Ground_Weapons;
    BWSL_EXPORT extern const UpgradeType Protoss_Air_Weapons;
    BWSL_EXPORT extern const UpgradeType Protoss_Plasma_Shields;
    BWSL_EXPORT extern const UpgradeType U_238_Shells;
    BWSL_EXPORT extern const UpgradeType Ion_Thrusters;
    BWSL_EXPORT extern const UpgradeType Titan_Reactor;
    BWSL_EXPORT extern const UpgradeType Ocular_Implants;
    BWSL_EXPORT extern const UpgradeType Moebius_Reactor;
    BWSL_EXPORT extern const UpgradeType Apollo_Reactor;
    BWSL_EXPORT extern const UpgradeType Colossus_Reactor;
    BWSL_EXPORT extern const UpgradeType Ventral_Sacs;
    BWSL_EXPORT extern const UpgradeType Antennae;
    BWSL_EXPORT extern const UpgradeType Pneumatized_Carapace;
    BWSL_EXPORT extern const UpgradeType Metabolic_Boost;
    BWSL_EXPORT extern const UpgradeType Adrenal_Glands;
    BWSL_EXPORT extern const UpgradeType Muscular_Augments;
    BWSL_EXPORT extern const UpgradeType Grooved_Spines;
    BWSL_EXPORT extern const UpgradeType Gamete_Meiosis;
    BWSL_EXPORT extern const UpgradeType Metasynaptic_Node;
    BWSL_EXPORT extern const UpgradeType Singularity_Charge;
    BWSL_EXPORT extern const UpgradeType Leg_Enhancements;
    BWSL_EXPORT extern const UpgradeType Scarab_Damage;
    BWSL_EXPORT extern const UpgradeType Reaver_Capacity;
    BWSL_EXPORT extern const UpgradeType Gravitic_Drive;
    BWSL_EXPORT extern const UpgradeType Sensor_Array;
    BWSL_EXPORT extern const UpgradeType Gravitic_Boosters;
    BWSL_EXPORT extern const UpgradeType Khaydarin_Amulet;
    BWSL_EXPORT extern const UpgradeType Apial_Sensors;
    BWSL_EXPORT extern const UpgradeType Gravitic_Thrusters;
    BWSL_EXPORT extern const UpgradeType Carrier_Capacity;
    BWSL_EXPORT extern const UpgradeType Khaydarin_Core;
    BWSL_EXPORT extern const UpgradeType Argus_Jewel;
    BWSL_EXPORT extern const UpgradeType Argus_Talisman;
    BWSL_EXPORT extern const UpgradeType Caduceus_Reactor;
    BWSL_EXPORT extern const UpgradeType Chitinous_Plating;
    BWSL_EXPORT extern const UpgradeType Anabolic_Synthesis;
    BWSL_EXPORT extern const UpgradeType Charon_Booster;
    BWSL_EXPORT extern const UpgradeType None;
    BWSL_EXPORT extern const UpgradeType Unknown;
  }
}
