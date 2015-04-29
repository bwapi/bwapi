#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class Race;

  /// <summary>Namespace of upgrade types.</summary>
  namespace UpgradeTypes
  {
    /// <summary>Enumeration of upgrade types.</summary>
    namespace Enum
    {
      /// <summary>Enumeration of upgrade types.</summary>
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

  /// <summary>The upgrade type represents a passive upgrade that can be obtained with
  /// UnitInterface::upgrade. </summary>
  ///
  /// @see UpgradeTypes
  ///
  /// @ingroup TypeClasses
  class UpgradeType : public Type<UpgradeType, UpgradeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UpgradeType(int id = UpgradeTypes::Enum::None);

    /// <summary>Retrieves the race the upgrade is for.</summary>
    /// For example, UpgradeTypes::Terran_Infantry_Armor.getRace() will return Races::Terran.
    ///
    /// @returns Race that this upgrade belongs to.
    Race getRace() const;

    /// <summary>Returns the mineral price for the upgrade.</summary>
    ///
    /// <param name="level"> (optional)
    ///     The next upgrade level.
    /// </param>
    ///
    /// @note Upgrades start at level 0.
    /// 
    /// @returns The mineral cost of the upgrade for the given \p level.
    int mineralPrice(int level = 1) const;

    /// <summary>The amount that the mineral price increases for each additional upgrade.</summary>
    ///
    /// @returns The mineral cost added to the upgrade after each level.
    int mineralPriceFactor() const;

    /// <summary>Returns the vespene gas price for the first upgrade.</summary>
    ///
    /// <param name="level"> (optional)
    ///     The next upgrade level.
    /// </param>
    ///
    /// @note Upgrades start at level 0.
    /// 
    /// @returns The gas cost of the upgrade for the given \p level.
    int gasPrice(int level = 1) const;

    /// <summary>Returns the amount that the vespene gas price increases for each additional upgrade.</summary>
    ///
    /// @returns The gas cost added to the upgrade after each level.
    int gasPriceFactor() const;

    /// <summary>Returns the number of frames needed to research the first upgrade.</summary>
    ///
    /// <param name="level"> (optional)
    ///     The next upgrade level.
    /// </param>
    ///
    /// @note Upgrades start at level 0.
    /// 
    /// @returns The time cost of the upgrade for the given \p level.
    int upgradeTime(int level = 1) const;

    /// <summary>Returns the number of frames that the upgrade time increases for each additional upgrade.</summary>
    ///
    /// @returns The time cost added to the upgrade after each level.
    int upgradeTimeFactor() const;

    /// <summary>Returns the maximum number of times the upgrade can be researched.</summary>
    ///
    /// @returns Maximum number of times this upgrade can be upgraded.
    int maxRepeats() const;

    /// <summary>Returns the type of unit that researches the upgrade.</summary>
    ///
    /// @returns The UnitType that is used to upgrade this type.
    UnitType whatUpgrades() const;

    /// <summary>Returns the type of unit that is required for the upgrade.</summary> The player
    /// must have at least one of these units completed in order to start upgrading this upgrade.
    ///
    /// <param name="level"> (optional)
    ///     The next upgrade level.
    /// </param>
    ///
    /// @note Upgrades start at level 0.
    /// 
    /// @returns UnitType required to obtain this upgrade.
    UnitType whatsRequired(int level = 1) const;

    /// <summary>Returns the set of units that are affected by this upgrade.</summary>
    /// 
    /// @returns Set of unit types that passively use this upgrade type.
    const UnitType::set& whatUses() const;
  };

  /// @ingroup Types
  namespace UpgradeTypes
  {
    /// <summary>Returns the set of all the UpgradeTypes.</summary>
    ///
    /// @returns UpgradeType::set containing all of the well-defined UpgradeTypes.
    const UpgradeType::set& allUpgradeTypes();

    /// @name Terran Upgrades
    /// @{
    extern const UpgradeType Terran_Infantry_Armor;
    extern const UpgradeType Terran_Vehicle_Plating;
    extern const UpgradeType Terran_Ship_Plating;
    extern const UpgradeType Terran_Infantry_Weapons;
    extern const UpgradeType Terran_Vehicle_Weapons;
    extern const UpgradeType Terran_Ship_Weapons;
    extern const UpgradeType U_238_Shells;
    extern const UpgradeType Ion_Thrusters;
    extern const UpgradeType Titan_Reactor;
    extern const UpgradeType Ocular_Implants;
    extern const UpgradeType Moebius_Reactor;
    extern const UpgradeType Apollo_Reactor;
    extern const UpgradeType Colossus_Reactor;
    extern const UpgradeType Caduceus_Reactor;
    extern const UpgradeType Charon_Boosters;
    /// @}
    /// @name Zerg Upgrades
    /// @{
    extern const UpgradeType Zerg_Carapace;
    extern const UpgradeType Zerg_Flyer_Carapace;
    extern const UpgradeType Zerg_Melee_Attacks;
    extern const UpgradeType Zerg_Missile_Attacks;
    extern const UpgradeType Zerg_Flyer_Attacks;
    extern const UpgradeType Ventral_Sacs;
    extern const UpgradeType Antennae;
    extern const UpgradeType Pneumatized_Carapace;
    extern const UpgradeType Metabolic_Boost;
    extern const UpgradeType Adrenal_Glands;
    extern const UpgradeType Muscular_Augments;
    extern const UpgradeType Grooved_Spines;
    extern const UpgradeType Gamete_Meiosis;
    extern const UpgradeType Metasynaptic_Node;
    extern const UpgradeType Chitinous_Plating;
    extern const UpgradeType Anabolic_Synthesis;
    /// @}
    /// @name Protoss Upgrades
    /// @{
    extern const UpgradeType Protoss_Ground_Armor;
    extern const UpgradeType Protoss_Air_Armor;
    extern const UpgradeType Protoss_Ground_Weapons;
    extern const UpgradeType Protoss_Air_Weapons;
    extern const UpgradeType Protoss_Plasma_Shields;
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
    /// @}

    extern const UpgradeType Upgrade_60;
    extern const UpgradeType None;
    extern const UpgradeType Unknown;
  }

  static_assert(sizeof(UpgradeType) == sizeof(int), "Expected type to resolve to primitive size.");
}
