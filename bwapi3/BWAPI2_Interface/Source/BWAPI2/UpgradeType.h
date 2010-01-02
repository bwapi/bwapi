#pragma once

#include "BWAPI2Interface.h"
#include "Race.h"

#include <string>
#include <set>
namespace BWAPI2
{
  class UnitType;
  class UpgradeType
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
    BWAPI2_EXPORT extern const UpgradeType Terran_Infantry_Armor;
    BWAPI2_EXPORT extern const UpgradeType Terran_Vehicle_Plating;
    BWAPI2_EXPORT extern const UpgradeType Terran_Ship_Plating;
    BWAPI2_EXPORT extern const UpgradeType Zerg_Carapace;
    BWAPI2_EXPORT extern const UpgradeType Zerg_Flyer_Carapace;
    BWAPI2_EXPORT extern const UpgradeType Protoss_Armor;
    BWAPI2_EXPORT extern const UpgradeType Protoss_Plating;
    BWAPI2_EXPORT extern const UpgradeType Terran_Infantry_Weapons;
    BWAPI2_EXPORT extern const UpgradeType Terran_Vehicle_Weapons;
    BWAPI2_EXPORT extern const UpgradeType Terran_Ship_Weapons;
    BWAPI2_EXPORT extern const UpgradeType Zerg_Melee_Attacks;
    BWAPI2_EXPORT extern const UpgradeType Zerg_Missile_Attacks;
    BWAPI2_EXPORT extern const UpgradeType Zerg_Flyer_Attacks;
    BWAPI2_EXPORT extern const UpgradeType Protoss_Ground_Weapons;
    BWAPI2_EXPORT extern const UpgradeType Protoss_Air_Weapons;
    BWAPI2_EXPORT extern const UpgradeType Protoss_Plasma_Shields;
    BWAPI2_EXPORT extern const UpgradeType U_238_Shells;
    BWAPI2_EXPORT extern const UpgradeType Ion_Thrusters;
    BWAPI2_EXPORT extern const UpgradeType Titan_Reactor;
    BWAPI2_EXPORT extern const UpgradeType Ocular_Implants;
    BWAPI2_EXPORT extern const UpgradeType Moebius_Reactor;
    BWAPI2_EXPORT extern const UpgradeType Apollo_Reactor;
    BWAPI2_EXPORT extern const UpgradeType Colossus_Reactor;
    BWAPI2_EXPORT extern const UpgradeType Ventral_Sacs;
    BWAPI2_EXPORT extern const UpgradeType Antennae;
    BWAPI2_EXPORT extern const UpgradeType Pneumatized_Carapace;
    BWAPI2_EXPORT extern const UpgradeType Metabolic_Boost;
    BWAPI2_EXPORT extern const UpgradeType Adrenal_Glands;
    BWAPI2_EXPORT extern const UpgradeType Muscular_Augments;
    BWAPI2_EXPORT extern const UpgradeType Grooved_Spines;
    BWAPI2_EXPORT extern const UpgradeType Gamete_Meiosis;
    BWAPI2_EXPORT extern const UpgradeType Metasynaptic_Node;
    BWAPI2_EXPORT extern const UpgradeType Singularity_Charge;
    BWAPI2_EXPORT extern const UpgradeType Leg_Enhancements;
    BWAPI2_EXPORT extern const UpgradeType Scarab_Damage;
    BWAPI2_EXPORT extern const UpgradeType Reaver_Capacity;
    BWAPI2_EXPORT extern const UpgradeType Gravitic_Drive;
    BWAPI2_EXPORT extern const UpgradeType Sensor_Array;
    BWAPI2_EXPORT extern const UpgradeType Gravitic_Boosters;
    BWAPI2_EXPORT extern const UpgradeType Khaydarin_Amulet;
    BWAPI2_EXPORT extern const UpgradeType Apial_Sensors;
    BWAPI2_EXPORT extern const UpgradeType Gravitic_Thrusters;
    BWAPI2_EXPORT extern const UpgradeType Carrier_Capacity;
    BWAPI2_EXPORT extern const UpgradeType Khaydarin_Core;
    BWAPI2_EXPORT extern const UpgradeType Argus_Jewel;
    BWAPI2_EXPORT extern const UpgradeType Argus_Talisman;
    BWAPI2_EXPORT extern const UpgradeType Caduceus_Reactor;
    BWAPI2_EXPORT extern const UpgradeType Chitinous_Plating;
    BWAPI2_EXPORT extern const UpgradeType Anabolic_Synthesis;
    BWAPI2_EXPORT extern const UpgradeType Charon_Booster;
    BWAPI2_EXPORT extern const UpgradeType None;
    BWAPI2_EXPORT extern const UpgradeType Unknown;
  }
}
