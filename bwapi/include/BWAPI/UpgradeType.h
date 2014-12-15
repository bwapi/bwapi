#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class Race;

  namespace UpgradeTypes
  {
    /// Enumeration of upgrade types
    namespace Enum
    {
      /// Enumeration of upgrade types
      enum Enum
      {
        Terran_Infantry_Armor   = 0,
        Terran_Vehicle_Plating  = 1,
        Terran_Ship_Plating     = 2,
        Zerg_Carapace           = 3,
        Zerg_Flyer_Carapace     = 4,
        Protoss_Ground_Armor    = 5,
        Protoss_Air_Armor       = 6,
        Terran_Infantry_Weapons = 7,
        Terran_Vehicle_Weapons  = 8,
        Terran_Ship_Weapons     = 9,
        Zerg_Melee_Attacks      = 10,
        Zerg_Missile_Attacks    = 11,
        Zerg_Flyer_Attacks      = 12,
        Protoss_Ground_Weapons  = 13,
        Protoss_Air_Weapons     = 14,
        Protoss_Plasma_Shields  = 15,
        U_238_Shells            = 16,
        Ion_Thrusters           = 17,
    
        Titan_Reactor           = 19,
        Ocular_Implants         = 20,
        Moebius_Reactor         = 21,
        Apollo_Reactor          = 22,
        Colossus_Reactor        = 23,
        Ventral_Sacs            = 24,
        Antennae                = 25,
        Pneumatized_Carapace    = 26,
        Metabolic_Boost         = 27,
        Adrenal_Glands          = 28,
        Muscular_Augments       = 29,
        Grooved_Spines          = 30,
        Gamete_Meiosis          = 31,
        Metasynaptic_Node       = 32,
        Singularity_Charge      = 33,
        Leg_Enhancements        = 34,
        Scarab_Damage           = 35,
        Reaver_Capacity         = 36,
        Gravitic_Drive          = 37,
        Sensor_Array            = 38,
        Gravitic_Boosters       = 39,
        Khaydarin_Amulet        = 40,
        Apial_Sensors           = 41,
        Gravitic_Thrusters      = 42,
        Carrier_Capacity        = 43,
        Khaydarin_Core          = 44,
    
        Argus_Jewel             = 47,
    
        Argus_Talisman          = 49,
    
        Caduceus_Reactor        = 51,
        Chitinous_Plating       = 52,
        Anabolic_Synthesis      = 53,
        Charon_Boosters         = 54,
    
        Upgrade_60              = 60,
        None                    = 61,
        Unknown,
        MAX
      };
    }
  }
  class UpgradeType : public Type<UpgradeType, UpgradeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UpgradeType(int id = UpgradeTypes::Enum::None);

    /** Returns the race the upgrade is for. For example, UpgradeTypes::Terran_Infantry_Armor.getRace()
     * will return Races::Terran. */
    Race getRace() const;

    /** Returns the mineral price for the first upgrade. */
    int mineralPrice(int level = 1) const;

    /** Returns the amount that the mineral price increases for each additional upgrade. */
    int mineralPriceFactor() const;

    /** Returns the vespene gas price for the first upgrade. */
    int gasPrice(int level = 1) const;

    /** Returns the amount that the vespene gas price increases for each additional upgrade. */
    int gasPriceFactor() const;

    /** Returns the number of frames needed to research the first upgrade. */
    int upgradeTime(int level = 1) const;

    /** Returns the number of frames that the upgrade time increases for each additional upgrade. */
    int upgradeTimeFactor() const;

    /** Returns the maximum number of times the upgrade can be researched. */
    int maxRepeats() const;

    /** Returns the type of unit that researches the upgrade. */
    UnitType whatUpgrades() const;

    /** Returns the type of unit that is additionally required for the upgrade. */
    UnitType whatsRequired(int level = 1) const;

    /** Returns the set of units that are affected by this upgrade. */
    const UnitType::set& whatUses() const;
  };
  /// Namespace of upgrade types
  namespace UpgradeTypes
  {
    /** Returns the set of all the UpgradeTypes. */
    const UpgradeType::set& allUpgradeTypes();

    extern const UpgradeType Terran_Infantry_Armor;
    extern const UpgradeType Terran_Vehicle_Plating;
    extern const UpgradeType Terran_Ship_Plating;
    extern const UpgradeType Zerg_Carapace;
    extern const UpgradeType Zerg_Flyer_Carapace;
    extern const UpgradeType Protoss_Ground_Armor;
    extern const UpgradeType Protoss_Air_Armor;
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
    extern const UpgradeType Charon_Boosters;
    extern const UpgradeType Upgrade_60;
    extern const UpgradeType None;
    extern const UpgradeType Unknown;
  }

  static_assert(sizeof(UpgradeType) == sizeof(int), "Expected type to resolve to primitive size.");
}
