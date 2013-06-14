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
    const UnitType::const_set& whatUses() const;
  };
  /// Namespace of upgrade types
  namespace UpgradeTypes
  {
    /** Returns the set of all the UpgradeTypes. */
    const UpgradeType::const_set& allUpgradeTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const UpgradeType x
    BWAPI_DECL(Terran_Infantry_Armor);
    BWAPI_DECL(Terran_Vehicle_Plating);
    BWAPI_DECL(Terran_Ship_Plating);
    BWAPI_DECL(Zerg_Carapace);
    BWAPI_DECL(Zerg_Flyer_Carapace);
    BWAPI_DECL(Protoss_Ground_Armor);
    BWAPI_DECL(Protoss_Air_Armor);
    BWAPI_DECL(Terran_Infantry_Weapons);
    BWAPI_DECL(Terran_Vehicle_Weapons);
    BWAPI_DECL(Terran_Ship_Weapons);
    BWAPI_DECL(Zerg_Melee_Attacks);
    BWAPI_DECL(Zerg_Missile_Attacks);
    BWAPI_DECL(Zerg_Flyer_Attacks);
    BWAPI_DECL(Protoss_Ground_Weapons);
    BWAPI_DECL(Protoss_Air_Weapons);
    BWAPI_DECL(Protoss_Plasma_Shields);
    BWAPI_DECL(U_238_Shells);
    BWAPI_DECL(Ion_Thrusters);
    BWAPI_DECL(Titan_Reactor);
    BWAPI_DECL(Ocular_Implants);
    BWAPI_DECL(Moebius_Reactor);
    BWAPI_DECL(Apollo_Reactor);
    BWAPI_DECL(Colossus_Reactor);
    BWAPI_DECL(Ventral_Sacs);
    BWAPI_DECL(Antennae);
    BWAPI_DECL(Pneumatized_Carapace);
    BWAPI_DECL(Metabolic_Boost);
    BWAPI_DECL(Adrenal_Glands);
    BWAPI_DECL(Muscular_Augments);
    BWAPI_DECL(Grooved_Spines);
    BWAPI_DECL(Gamete_Meiosis);
    BWAPI_DECL(Metasynaptic_Node);
    BWAPI_DECL(Singularity_Charge);
    BWAPI_DECL(Leg_Enhancements);
    BWAPI_DECL(Scarab_Damage);
    BWAPI_DECL(Reaver_Capacity);
    BWAPI_DECL(Gravitic_Drive);
    BWAPI_DECL(Sensor_Array);
    BWAPI_DECL(Gravitic_Boosters);
    BWAPI_DECL(Khaydarin_Amulet);
    BWAPI_DECL(Apial_Sensors);
    BWAPI_DECL(Gravitic_Thrusters);
    BWAPI_DECL(Carrier_Capacity);
    BWAPI_DECL(Khaydarin_Core);
    BWAPI_DECL(Argus_Jewel);
    BWAPI_DECL(Argus_Talisman);
    BWAPI_DECL(Caduceus_Reactor);
    BWAPI_DECL(Chitinous_Plating);
    BWAPI_DECL(Anabolic_Synthesis);
    BWAPI_DECL(Charon_Boosters);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
