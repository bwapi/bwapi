#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class Race;
  class UnitType;

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
  /// Unit::upgrade. </summary>
  ///
  /// @see UpgradeTypes
  ///
  /// @ingroup TypeClasses
  class UpgradeType : public Type<UpgradeType, UpgradeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    constexpr explicit UpgradeType(int id = UpgradeTypes::Enum::None) : Type(id) {}

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
    const SetContainer<UnitType>& whatUses() const;
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
    constexpr UpgradeType Terran_Infantry_Armor{Enum::Terran_Infantry_Armor};
    constexpr UpgradeType Terran_Vehicle_Plating{Enum::Terran_Vehicle_Plating};
    constexpr UpgradeType Terran_Ship_Plating{Enum::Terran_Ship_Plating};
    constexpr UpgradeType Terran_Infantry_Weapons{Enum::Terran_Infantry_Weapons};
    constexpr UpgradeType Terran_Vehicle_Weapons{Enum::Terran_Vehicle_Weapons};
    constexpr UpgradeType Terran_Ship_Weapons{Enum::Terran_Ship_Weapons};
    constexpr UpgradeType U_238_Shells{Enum::U_238_Shells};
    constexpr UpgradeType Ion_Thrusters{Enum::Ion_Thrusters};
    constexpr UpgradeType Titan_Reactor{Enum::Titan_Reactor};
    constexpr UpgradeType Ocular_Implants{Enum::Ocular_Implants};
    constexpr UpgradeType Moebius_Reactor{Enum::Moebius_Reactor};
    constexpr UpgradeType Apollo_Reactor{Enum::Apollo_Reactor};
    constexpr UpgradeType Colossus_Reactor{Enum::Colossus_Reactor};
    constexpr UpgradeType Caduceus_Reactor{Enum::Caduceus_Reactor};
    constexpr UpgradeType Charon_Boosters{Enum::Charon_Boosters};
    /// @}
    /// @name Zerg Upgrades
    /// @{
    constexpr UpgradeType Zerg_Carapace{Enum::Zerg_Carapace};
    constexpr UpgradeType Zerg_Flyer_Carapace{Enum::Zerg_Flyer_Carapace};
    constexpr UpgradeType Zerg_Melee_Attacks{Enum::Zerg_Melee_Attacks};
    constexpr UpgradeType Zerg_Missile_Attacks{Enum::Zerg_Missile_Attacks};
    constexpr UpgradeType Zerg_Flyer_Attacks{Enum::Zerg_Flyer_Attacks};
    constexpr UpgradeType Ventral_Sacs{Enum::Ventral_Sacs};
    constexpr UpgradeType Antennae{Enum::Antennae};
    constexpr UpgradeType Pneumatized_Carapace{Enum::Pneumatized_Carapace};
    constexpr UpgradeType Metabolic_Boost{Enum::Metabolic_Boost};
    constexpr UpgradeType Adrenal_Glands{Enum::Adrenal_Glands};
    constexpr UpgradeType Muscular_Augments{Enum::Muscular_Augments};
    constexpr UpgradeType Grooved_Spines{Enum::Grooved_Spines};
    constexpr UpgradeType Gamete_Meiosis{Enum::Gamete_Meiosis};
    constexpr UpgradeType Metasynaptic_Node{Enum::Metasynaptic_Node};
    constexpr UpgradeType Chitinous_Plating{Enum::Chitinous_Plating};
    constexpr UpgradeType Anabolic_Synthesis{Enum::Anabolic_Synthesis};
    /// @}
    /// @name Protoss Upgrades
    /// @{
    constexpr UpgradeType Protoss_Ground_Armor{Enum::Protoss_Ground_Armor};
    constexpr UpgradeType Protoss_Air_Armor{Enum::Protoss_Air_Armor};
    constexpr UpgradeType Protoss_Ground_Weapons{Enum::Protoss_Ground_Weapons};
    constexpr UpgradeType Protoss_Air_Weapons{Enum::Protoss_Air_Weapons};
    constexpr UpgradeType Protoss_Plasma_Shields{Enum::Protoss_Plasma_Shields};
    constexpr UpgradeType Singularity_Charge{Enum::Singularity_Charge};
    constexpr UpgradeType Leg_Enhancements{Enum::Leg_Enhancements};
    constexpr UpgradeType Scarab_Damage{Enum::Scarab_Damage};
    constexpr UpgradeType Reaver_Capacity{Enum::Reaver_Capacity};
    constexpr UpgradeType Gravitic_Drive{Enum::Gravitic_Drive};
    constexpr UpgradeType Sensor_Array{Enum::Sensor_Array};
    constexpr UpgradeType Gravitic_Boosters{Enum::Gravitic_Boosters};
    constexpr UpgradeType Khaydarin_Amulet{Enum::Khaydarin_Amulet};
    constexpr UpgradeType Apial_Sensors{Enum::Apial_Sensors};
    constexpr UpgradeType Gravitic_Thrusters{Enum::Gravitic_Thrusters};
    constexpr UpgradeType Carrier_Capacity{Enum::Carrier_Capacity};
    constexpr UpgradeType Khaydarin_Core{Enum::Khaydarin_Core};
    constexpr UpgradeType Argus_Jewel{Enum::Argus_Jewel};
    constexpr UpgradeType Argus_Talisman{Enum::Argus_Talisman};
    /// @}

    constexpr UpgradeType Upgrade_60{Enum::Upgrade_60};
    constexpr UpgradeType None{Enum::None};
    constexpr UpgradeType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(UpgradeType) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::UpgradeType> {
    auto operator()(BWAPI::UpgradeType id) const {
      return BWAPI::UpgradeType::Hash{}(id);
    }
  };
} // namespace std
