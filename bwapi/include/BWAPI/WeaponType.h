#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class TechType;
  class UpgradeType;
  class DamageType;
  class ExplosionType;
  class UnitType;

  /// <summary>namespace containing weapon types.</summary>
  /// @see WeaponType
  namespace WeaponTypes
  {
    /// <summary>Enumeration of weapon types.</summary>
    /// @see WeaponType
    namespace Enum
    {
      /// <summary>Enumeration of weapon types.</summary>
      /// @see WeaponType
      enum Enum
      {
        Gauss_Rifle = 0,
        Gauss_Rifle_Jim_Raynor = 1,
        C_10_Canister_Rifle = 2,
        C_10_Canister_Rifle_Sarah_Kerrigan = 3,
        Fragmentation_Grenade = 4,
        Fragmentation_Grenade_Jim_Raynor = 5,
        Spider_Mines = 6,
        Twin_Autocannons = 7,
        Hellfire_Missile_Pack = 8,
        Twin_Autocannons_Alan_Schezar = 9,
        Hellfire_Missile_Pack_Alan_Schezar = 10,
        Arclite_Cannon = 11,
        Arclite_Cannon_Edmund_Duke = 12,
        Fusion_Cutter = 13,
        
        Gemini_Missiles = 15,
        Burst_Lasers = 16,
        Gemini_Missiles_Tom_Kazansky = 17,
        Burst_Lasers_Tom_Kazansky = 18,
        ATS_Laser_Battery = 19,
        ATA_Laser_Battery = 20,
        ATS_Laser_Battery_Hero = 21,
        ATA_Laser_Battery_Hero = 22,
        ATS_Laser_Battery_Hyperion = 23,
        ATA_Laser_Battery_Hyperion = 24,
        Flame_Thrower = 25,
        Flame_Thrower_Gui_Montag = 26,
        Arclite_Shock_Cannon = 27,
        Arclite_Shock_Cannon_Edmund_Duke = 28,
        Longbolt_Missile = 29,
        Yamato_Gun = 30,
        Nuclear_Strike = 31,
        Lockdown = 32,
        EMP_Shockwave = 33,
        Irradiate = 34,
        Claws = 35,
        Claws_Devouring_One = 36,
        Claws_Infested_Kerrigan = 37,
        Needle_Spines = 38,
        Needle_Spines_Hunter_Killer = 39,
        Kaiser_Blades = 40,
        Kaiser_Blades_Torrasque = 41,
        Toxic_Spores = 42,
        Spines = 43,
        
        Acid_Spore = 46,
        Acid_Spore_Kukulza = 47,
        Glave_Wurm = 48,
        Glave_Wurm_Kukulza = 49,
        
        Seeker_Spores = 52,
        Subterranean_Tentacle = 53,
        Suicide_Infested_Terran = 54,
        Suicide_Scourge = 55,
        Parasite = 56,
        Spawn_Broodlings = 57,
        Ensnare = 58,
        Dark_Swarm = 59,
        Plague = 60,
        Consume = 61,
        Particle_Beam = 62,
        
        Psi_Blades = 64,
        Psi_Blades_Fenix = 65,
        Phase_Disruptor = 66,
        Phase_Disruptor_Fenix = 67,
        
        Psi_Assault = 69,
        Psionic_Shockwave = 70,
        Psionic_Shockwave_TZ_Archon = 71,
        
        Dual_Photon_Blasters = 73,
        Anti_Matter_Missiles = 74,
        Dual_Photon_Blasters_Mojo = 75,
        Anti_Matter_Missiles_Mojo = 76,
        Phase_Disruptor_Cannon = 77,
        Phase_Disruptor_Cannon_Danimoth = 78,
        Pulse_Cannon = 79,
        STS_Photon_Cannon = 80,
        STA_Photon_Cannon = 81,
        Scarab = 82,
        Stasis_Field = 83,
        Psionic_Storm = 84,
        Warp_Blades_Zeratul = 85,
        Warp_Blades_Hero = 86,
        
        Platform_Laser_Battery = 92,
        Independant_Laser_Battery = 93,
        
        Twin_Autocannons_Floor_Trap = 96,
        Hellfire_Missile_Pack_Wall_Trap = 97,
        Flame_Thrower_Wall_Trap = 98,
        Hellfire_Missile_Pack_Floor_Trap = 99,
    
        Neutron_Flare = 100,
        Disruption_Web = 101,
        Restoration = 102,
        Halo_Rockets = 103,
        Corrosive_Acid = 104,
        Mind_Control = 105,
        Feedback = 106,
        Optical_Flare = 107,
        Maelstrom = 108,
        Subterranean_Spines = 109,
        
        Warp_Blades = 111,
        C_10_Canister_Rifle_Samir_Duran = 112,
        C_10_Canister_Rifle_Infested_Duran = 113,
        Dual_Photon_Blasters_Artanis = 114,
        Anti_Matter_Missiles_Artanis = 115,
        C_10_Canister_Rifle_Alexei_Stukov = 116,
    
        None = 130,
        Unknown,
        MAX
      };
    }
  }
  /// <summary>This object identifies a weapon type used by a unit to attack and deal damage.</summary>
  /// Some weapon types can be upgraded while others are used for special abilities.
  ///
  /// @see WeaponTypes
  /// @ingroup TypeClasses
  class WeaponType : public Type<WeaponType, WeaponTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr explicit WeaponType(int id = WeaponTypes::Enum::None) : Type(id) {}

      /// <summary>Retrieves the technology type that must be researched before this weapon can
      /// be used.</summary>
      ///
      /// @returns TechType required by this weapon.
      /// @retval TechTypes::None if no tech type is required to use this weapon.
      /// @see TechType::getWeapon
      TechType getTech() const;

      /// <summary>Retrieves the unit type that is intended to use this weapon type.</summary>
      ///
      /// @note There is a rare case where some hero unit types use the same weapon.
      ///
      /// @todo specify which types use the same weapon
      ///
      /// @returns The UnitType that uses this weapon.
      /// @see UnitType::groundWeapon, UnitType::airWeapon
      UnitType whatUses() const;

      /// <summary>Retrieves the base amount of damage that this weapon can deal per attack.</summary>
      /// 
      /// @note That this damage amount must go through a DamageType and UnitSizeType filter
      /// before it is applied to a unit.
      ///
      /// @returns Amount of base damage that this weapon deals.
      int damageAmount() const;

      /// <summary>Determines the bonus amount of damage that this weapon type increases by for every
      /// upgrade to this type.</summary>
      ///
      /// @see upgradeType
      /// @returns Amount of damage added for every weapon upgrade.
      int damageBonus() const;

      /// <summary>Retrieves the base amount of cooldown time between each attack, in frames.</summary>
      ///
      /// @returns The amount of base cooldown applied to the unit after an attack.
      /// @see Unit::getGroundWeaponCooldown, Unit::getAirWeaponCooldown
      int damageCooldown() const;

      /// <summary>Obtains the intended number of missiles/attacks that are used.</summary>
      /// This is used to multiply with the damage amount to obtain the full amount of damage
      /// for an attack.
      ///
      /// @returns The damage factor multiplied by the amount to obtain the total damage.
      /// @see damageAmount
      int damageFactor() const;

      /// <summary>Retrieves the upgrade type that increases this weapon's damage output.</summary>
      /// 
      /// @returns The UpgradeType used to upgrade this weapon's damage.
      /// @see damageBonus
      UpgradeType upgradeType() const;

      /// <summary>Retrieves the damage type that this weapon applies to a unit type.</summary>
      ///
      /// @returns DamageType used for damage calculation.
      /// @see DamageType, UnitSizeType
      DamageType damageType() const;

      /// <summary>Retrieves the explosion type that indicates how the weapon deals damage.</summary>
      ///
      /// @returns ExplosionType identifying how damage is applied to a target location.
      ExplosionType explosionType() const;

      /// <summary>Retrieves the minimum attack range of the weapon, measured in pixels.</summary>
      /// This value is 0 for almost all weapon types, except for WeaponTypes::Arclite_Shock_Cannon
      /// and WeaponTypes::Arclite_Shock_Cannon_Edmund_Duke.
      ///
      /// @returns Minimum attack range, in pixels.
      int minRange() const;

      /// <summary>Retrieves the maximum attack range of the weapon, measured in pixels.</summary>
      ///
      /// @returns Maximum attack range, in pixels.
      int maxRange() const;

      /// <summary>Retrieves the inner radius used for splash damage calculations, in pixels.
      /// Units in this area take 100% damage.</summary>
      /// 
      /// @returns Radius of the inner splash area, in pixels.
      int innerSplashRadius() const;

      /// <summary>Retrieves the middle radius used for splash damage calculations, in pixels.
      /// Units in this area take 50% damage.</summary>
      /// 
      /// @returns Radius of the middle splash area, in pixels.
      int medianSplashRadius() const;

      /// <summary>Retrieves the outer radius used for splash damage calculations, in pixels.</summary>
      /// Units in this area take 25% damage.</summary>
      /// 
      /// @returns Radius of the outer splash area, in pixels.
      int outerSplashRadius() const;

      /// <summary>Checks if this weapon type can target air units.</summary>
      ///
      /// @returns true if this weapon type can target air units, and false otherwise.
      /// @see Unit::isFlying, UnitType::isFlyer
      bool targetsAir() const;

      /// <summary>Checks if this weapon type can target ground units.</summary>
      ///
      /// @returns true if this weapon type can target ground units, and false otherwise.
      /// @see Unit::isFlying, UnitType::isFlyer
      bool targetsGround() const;

      /// <summary>Checks if this weapon type can only target mechanical units.</summary>
      ///
      /// @returns true if this weapon type can only target mechanical units, and false otherwise.
      /// @see targetsOrgOrMech, UnitType::isMechanical
      bool targetsMechanical() const;
      
      /// <summary>Checks if this weapon type can only target organic units.</summary>
      ///
      /// @returns true if this weapon type can only target organic units, and false otherwise.
      /// @see targetsOrgOrMech, UnitType::isOrganic
      bool targetsOrganic() const;
      
      /// <summary>Checks if this weapon type cannot target structures.</summary>
      ///
      /// @returns true if this weapon type cannot target buildings, and false if it can.
      /// @see UnitType::isBuilding
      bool targetsNonBuilding() const;

      /// <summary>Checks if this weapon type cannot target robotic units.</summary>
      ///
      /// @returns true if this weapon type cannot target robotic units, and false if it can.
      /// @see UnitType::isRobotic
      bool targetsNonRobotic() const;

      /// <summary>Checks if this weapon type can target the ground.</summary>
      ///
      /// @note This is more for attacks like @Psi_Storm which can target a location, not to be
      /// confused with attack move.
      ///
      /// @returns true if this weapon type can target a location, and false otherwise.
      bool targetsTerrain() const;
      
      /// <summary>Checks if this weapon type can only target organic or mechanical units.</summary>
      ///
      /// @returns true if this weapon type can only target organic or mechanical units, and false otherwise.
      /// @see targetsOrganic, targetsMechanical, UnitType::isOrganic, UnitType::isMechanical
      bool targetsOrgOrMech() const;

      /// <summary>Checks if this weapon type can only target units owned by the same player.</summary>
      /// This is used for WeaponTypes::Consume.
      ///
      /// @returns true if this weapon type can only target your own units, and false otherwise.
      /// @see Unit::getPlayer
      bool targetsOwn() const;
  };

  /// @ingroup Types
  namespace WeaponTypes
  {
    /// <summary>Retrieves the set of all defined weapon types.</summary> This is a union between
    /// the normal and special weapon types.
    ///
    /// @returns set consisting of all defined weapon types.
    /// @see normalWeaponTypes, specialWeaponTypes
    const WeaponType::set& allWeaponTypes();

    /// <summary>Retrieves the set of all defined normal weapon types.</summary> This set contains
    /// all weapons that are not used for abilities.
    ///
    /// @returns constant set consisting of all normal weapon types.
    const WeaponType::set& normalWeaponTypes();

    /// <summary>Retrieves the set of all special weapon types.</summary> This set contains all
    /// weapons that are used exclusively for special unit abilities.
    ///
    /// @returns constant set consisting of all special weapon types.
    const WeaponType::set& specialWeaponTypes();

    /// @name Normal Weapons
    ///@{
    constexpr WeaponType Gauss_Rifle{Enum::Gauss_Rifle};
    constexpr WeaponType Gauss_Rifle_Jim_Raynor{Enum::Gauss_Rifle_Jim_Raynor};
    constexpr WeaponType C_10_Canister_Rifle{Enum::C_10_Canister_Rifle};
    constexpr WeaponType C_10_Canister_Rifle_Sarah_Kerrigan{Enum::C_10_Canister_Rifle_Sarah_Kerrigan};
    constexpr WeaponType C_10_Canister_Rifle_Samir_Duran{Enum::C_10_Canister_Rifle_Samir_Duran};
    constexpr WeaponType C_10_Canister_Rifle_Infested_Duran{Enum::C_10_Canister_Rifle_Infested_Duran};
    constexpr WeaponType C_10_Canister_Rifle_Alexei_Stukov{Enum::C_10_Canister_Rifle_Alexei_Stukov};
    constexpr WeaponType Fragmentation_Grenade{Enum::Fragmentation_Grenade};
    constexpr WeaponType Fragmentation_Grenade_Jim_Raynor{Enum::Fragmentation_Grenade_Jim_Raynor};
    constexpr WeaponType Spider_Mines{Enum::Spider_Mines};
    constexpr WeaponType Twin_Autocannons{Enum::Twin_Autocannons};
    constexpr WeaponType Twin_Autocannons_Alan_Schezar{Enum::Twin_Autocannons_Alan_Schezar};
    constexpr WeaponType Hellfire_Missile_Pack{Enum::Hellfire_Missile_Pack};
    constexpr WeaponType Hellfire_Missile_Pack_Alan_Schezar{Enum::Hellfire_Missile_Pack_Alan_Schezar};
    constexpr WeaponType Arclite_Cannon{Enum::Arclite_Cannon};
    constexpr WeaponType Arclite_Cannon_Edmund_Duke{Enum::Arclite_Cannon_Edmund_Duke};
    constexpr WeaponType Fusion_Cutter{Enum::Fusion_Cutter};
    constexpr WeaponType Gemini_Missiles{Enum::Gemini_Missiles};
    constexpr WeaponType Gemini_Missiles_Tom_Kazansky{Enum::Gemini_Missiles_Tom_Kazansky};
    constexpr WeaponType Burst_Lasers{Enum::Burst_Lasers};
    constexpr WeaponType Burst_Lasers_Tom_Kazansky{Enum::Burst_Lasers_Tom_Kazansky};
    constexpr WeaponType ATS_Laser_Battery{Enum::ATS_Laser_Battery};
    constexpr WeaponType ATS_Laser_Battery_Hero{Enum::ATS_Laser_Battery_Hero};
    constexpr WeaponType ATS_Laser_Battery_Hyperion{Enum::ATS_Laser_Battery_Hyperion};
    constexpr WeaponType ATA_Laser_Battery{Enum::ATA_Laser_Battery};
    constexpr WeaponType ATA_Laser_Battery_Hero{Enum::ATA_Laser_Battery_Hero};
    constexpr WeaponType ATA_Laser_Battery_Hyperion{Enum::ATA_Laser_Battery_Hyperion};
    constexpr WeaponType Flame_Thrower{Enum::Flame_Thrower};
    constexpr WeaponType Flame_Thrower_Gui_Montag{Enum::Flame_Thrower_Gui_Montag};
    constexpr WeaponType Arclite_Shock_Cannon{Enum::Arclite_Shock_Cannon};
    constexpr WeaponType Arclite_Shock_Cannon_Edmund_Duke{Enum::Arclite_Shock_Cannon_Edmund_Duke};
    constexpr WeaponType Longbolt_Missile{Enum::Longbolt_Missile};
    constexpr WeaponType Claws{Enum::Claws};
    constexpr WeaponType Claws_Devouring_One{Enum::Claws_Devouring_One};
    constexpr WeaponType Claws_Infested_Kerrigan{Enum::Claws_Infested_Kerrigan};
    constexpr WeaponType Needle_Spines{Enum::Needle_Spines};
    constexpr WeaponType Needle_Spines_Hunter_Killer{Enum::Needle_Spines_Hunter_Killer};
    constexpr WeaponType Kaiser_Blades{Enum::Kaiser_Blades};
    constexpr WeaponType Kaiser_Blades_Torrasque{Enum::Kaiser_Blades_Torrasque};
    constexpr WeaponType Toxic_Spores{Enum::Toxic_Spores};
    constexpr WeaponType Spines{Enum::Spines};
    constexpr WeaponType Acid_Spore{Enum::Acid_Spore};
    constexpr WeaponType Acid_Spore_Kukulza{Enum::Acid_Spore_Kukulza};
    constexpr WeaponType Glave_Wurm{Enum::Glave_Wurm};
    constexpr WeaponType Glave_Wurm_Kukulza{Enum::Glave_Wurm_Kukulza};
    constexpr WeaponType Seeker_Spores{Enum::Seeker_Spores};
    constexpr WeaponType Subterranean_Tentacle{Enum::Subterranean_Tentacle};
    constexpr WeaponType Suicide_Infested_Terran{Enum::Suicide_Infested_Terran};
    constexpr WeaponType Suicide_Scourge{Enum::Suicide_Scourge};
    constexpr WeaponType Particle_Beam{Enum::Particle_Beam};
    constexpr WeaponType Psi_Blades{Enum::Psi_Blades};
    constexpr WeaponType Psi_Blades_Fenix{Enum::Psi_Blades_Fenix};
    constexpr WeaponType Phase_Disruptor{Enum::Phase_Disruptor};
    constexpr WeaponType Phase_Disruptor_Fenix{Enum::Phase_Disruptor_Fenix};
    constexpr WeaponType Psi_Assault{Enum::Psi_Assault};
    constexpr WeaponType Psionic_Shockwave{Enum::Psionic_Shockwave};
    constexpr WeaponType Psionic_Shockwave_TZ_Archon{Enum::Psionic_Shockwave_TZ_Archon};
    constexpr WeaponType Dual_Photon_Blasters{Enum::Dual_Photon_Blasters};
    constexpr WeaponType Dual_Photon_Blasters_Mojo{Enum::Dual_Photon_Blasters_Mojo};
    constexpr WeaponType Dual_Photon_Blasters_Artanis{Enum::Dual_Photon_Blasters_Artanis};
    constexpr WeaponType Anti_Matter_Missiles{Enum::Anti_Matter_Missiles};
    constexpr WeaponType Anti_Matter_Missiles_Mojo{Enum::Anti_Matter_Missiles_Mojo};
    constexpr WeaponType Anti_Matter_Missiles_Artanis{Enum::Anti_Matter_Missiles_Artanis};
    constexpr WeaponType Phase_Disruptor_Cannon{Enum::Phase_Disruptor_Cannon};
    constexpr WeaponType Phase_Disruptor_Cannon_Danimoth{Enum::Phase_Disruptor_Cannon_Danimoth};
    constexpr WeaponType Pulse_Cannon{Enum::Pulse_Cannon};
    constexpr WeaponType STS_Photon_Cannon{Enum::STS_Photon_Cannon};
    constexpr WeaponType STA_Photon_Cannon{Enum::STA_Photon_Cannon};
    constexpr WeaponType Scarab{Enum::Scarab};
    constexpr WeaponType Neutron_Flare{Enum::Neutron_Flare};
    constexpr WeaponType Halo_Rockets{Enum::Halo_Rockets};
    constexpr WeaponType Corrosive_Acid{Enum::Corrosive_Acid};
    constexpr WeaponType Subterranean_Spines{Enum::Subterranean_Spines};
    constexpr WeaponType Warp_Blades{Enum::Warp_Blades};
    constexpr WeaponType Warp_Blades_Hero{Enum::Warp_Blades_Hero};
    constexpr WeaponType Warp_Blades_Zeratul{Enum::Warp_Blades_Zeratul};
    constexpr WeaponType Platform_Laser_Battery{Enum::Platform_Laser_Battery};
    constexpr WeaponType Independant_Laser_Battery{Enum::Independant_Laser_Battery};
    constexpr WeaponType Twin_Autocannons_Floor_Trap{Enum::Twin_Autocannons_Floor_Trap};
    constexpr WeaponType Hellfire_Missile_Pack_Wall_Trap{Enum::Hellfire_Missile_Pack_Wall_Trap};
    constexpr WeaponType Flame_Thrower_Wall_Trap{Enum::Flame_Thrower_Wall_Trap};
    constexpr WeaponType Hellfire_Missile_Pack_Floor_Trap{Enum::Hellfire_Missile_Pack_Floor_Trap};
    ///@}

    /// @name Special Weapons
    ///@{
    constexpr WeaponType Yamato_Gun{Enum::Yamato_Gun};
    constexpr WeaponType Nuclear_Strike{Enum::Nuclear_Strike};
    constexpr WeaponType Lockdown{Enum::Lockdown};
    constexpr WeaponType EMP_Shockwave{Enum::EMP_Shockwave};
    constexpr WeaponType Irradiate{Enum::Irradiate};
    constexpr WeaponType Parasite{Enum::Parasite};
    constexpr WeaponType Spawn_Broodlings{Enum::Spawn_Broodlings};
    constexpr WeaponType Ensnare{Enum::Ensnare};
    constexpr WeaponType Dark_Swarm{Enum::Dark_Swarm};
    constexpr WeaponType Plague{Enum::Plague};
    constexpr WeaponType Consume{Enum::Consume};
    constexpr WeaponType Stasis_Field{Enum::Stasis_Field};
    constexpr WeaponType Psionic_Storm{Enum::Psionic_Storm};
    constexpr WeaponType Disruption_Web{Enum::Disruption_Web};
    constexpr WeaponType Restoration{Enum::Restoration};
    constexpr WeaponType Mind_Control{Enum::Mind_Control};
    constexpr WeaponType Feedback{Enum::Feedback};
    constexpr WeaponType Optical_Flare{Enum::Optical_Flare};
    constexpr WeaponType Maelstrom{Enum::Maelstrom};
    ///@}

    constexpr WeaponType None{Enum::None};
    constexpr WeaponType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(WeaponType) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::WeaponType> {
    auto operator()(BWAPI::WeaponType id) const {
      return BWAPI::WeaponType::Hash{}(id);
    }
  };
} // namespace std
