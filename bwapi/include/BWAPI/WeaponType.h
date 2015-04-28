#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class TechType;
  class UpgradeType;
  class DamageType;
  class ExplosionType;
  class UnitType;

  namespace WeaponTypes
  {
    /// Enumeration of weapon types
    namespace Enum
    {
      /// Enumeration of weapon types
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
  class WeaponType : public Type<WeaponType, WeaponTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      WeaponType(int id = WeaponTypes::Enum::None);

      /** Returns the tech type that must be researched before this weapon can be used, or TechTypes::None if
       * no tech type is required. */
      TechType getTech() const;

      /** Returns the unit that can use this weapon. */
      UnitType whatUses() const;

      /** Returns the amount of damage that this weapon deals per attack. */
      int damageAmount() const;

      // TODO: add doc
      int damageBonus() const;

      /** Returns the amount of cooldown time between attacks. */
      int damageCooldown() const;

      /** Returns the amount that the damage increases per upgrade.
       * \see WeaponType::upgradeType. */
      int damageFactor() const;

      /** Returns the upgrade type that can be upgraded to increase the attack damage. */
      UpgradeType upgradeType() const;

      /** Returns the type of damage that this weapon uses (i.e. concussive, normal, explosive, etc). */
      DamageType damageType() const;

      /** Returns the type of explosion that this weapon uses. */
      ExplosionType explosionType() const;

      /** Returns the minimum attack range of the weapon, measured in pixels, 0 for most things except
       * WeaponTypes::Arclite_Shock_Cannon (the weapon of the Terran Siege Tank in Siege Mode). */
      int minRange() const;

      /** Returns the maximum attack range of the weapon, measured in pixels. */
      int maxRange() const;

      /** Inner radius used in splash damage calculations. */
      int innerSplashRadius() const;

      /** Median radius used in splash damage calculations. */
      int medianSplashRadius() const;

      /** Outer radius used in splash damage calculations. */
      int outerSplashRadius() const;

      /** Returns true if this weapon can attack air units. */
      bool targetsAir() const;

      // TODO: group these methods
      /** Returns true if this weapon can attack ground units. */
      bool targetsGround() const;
      bool targetsMechanical() const;
      bool targetsOrganic() const;
      bool targetsNonBuilding() const;
      bool targetsNonRobotic() const;
      bool targetsTerrain() const;
      bool targetsOrgOrMech() const;
      bool targetsOwn() const;
  };
  /// namespace containing weapon types
  namespace WeaponTypes
  {
    /// Retrieves the set of all weapon types. This is a union between the normalWeaponTypes and
    /// specialWeaponTypes.
    ///
    /// @returns set consisting of all weapon types.
    const WeaponType::set& allWeaponTypes();

    /// Retrieves the set of all normal weapon types. This set contains all weapons that are
    /// not used for abilities.
    ///
    /// @returns constant set consisting of all normal weapon types.
    const WeaponType::set& normalWeaponTypes();

    /// Retrieves the set of all special weapon types. This set contains all weapons that are
    /// used exclusively for special unit abilities.
    ///
    /// @returns constant set consisting of all special weapon types.
    const WeaponType::set& specialWeaponTypes();

    extern const WeaponType Gauss_Rifle;
    extern const WeaponType Gauss_Rifle_Jim_Raynor;
    extern const WeaponType C_10_Canister_Rifle;
    extern const WeaponType C_10_Canister_Rifle_Sarah_Kerrigan;
    extern const WeaponType C_10_Canister_Rifle_Samir_Duran;
    extern const WeaponType C_10_Canister_Rifle_Infested_Duran;
    extern const WeaponType C_10_Canister_Rifle_Alexei_Stukov;
    extern const WeaponType Fragmentation_Grenade;
    extern const WeaponType Fragmentation_Grenade_Jim_Raynor;
    extern const WeaponType Spider_Mines;
    extern const WeaponType Twin_Autocannons;
    extern const WeaponType Twin_Autocannons_Alan_Schezar;
    extern const WeaponType Hellfire_Missile_Pack;
    extern const WeaponType Hellfire_Missile_Pack_Alan_Schezar;
    extern const WeaponType Arclite_Cannon;
    extern const WeaponType Arclite_Cannon_Edmund_Duke;
    extern const WeaponType Fusion_Cutter;
    extern const WeaponType Gemini_Missiles;
    extern const WeaponType Gemini_Missiles_Tom_Kazansky;
    extern const WeaponType Burst_Lasers;
    extern const WeaponType Burst_Lasers_Tom_Kazansky;
    extern const WeaponType ATS_Laser_Battery;
    extern const WeaponType ATS_Laser_Battery_Hero;
    extern const WeaponType ATS_Laser_Battery_Hyperion;
    extern const WeaponType ATA_Laser_Battery;
    extern const WeaponType ATA_Laser_Battery_Hero;
    extern const WeaponType ATA_Laser_Battery_Hyperion;
    extern const WeaponType Flame_Thrower;
    extern const WeaponType Flame_Thrower_Gui_Montag;
    extern const WeaponType Arclite_Shock_Cannon;
    extern const WeaponType Arclite_Shock_Cannon_Edmund_Duke;
    extern const WeaponType Longbolt_Missile;
    extern const WeaponType Claws;
    extern const WeaponType Claws_Devouring_One;
    extern const WeaponType Claws_Infested_Kerrigan;
    extern const WeaponType Needle_Spines;
    extern const WeaponType Needle_Spines_Hunter_Killer;
    extern const WeaponType Kaiser_Blades;
    extern const WeaponType Kaiser_Blades_Torrasque;
    extern const WeaponType Toxic_Spores;
    extern const WeaponType Spines;
    extern const WeaponType Acid_Spore;
    extern const WeaponType Acid_Spore_Kukulza;
    extern const WeaponType Glave_Wurm;
    extern const WeaponType Glave_Wurm_Kukulza;
    extern const WeaponType Seeker_Spores;
    extern const WeaponType Subterranean_Tentacle;
    extern const WeaponType Suicide_Infested_Terran;
    extern const WeaponType Suicide_Scourge;
    extern const WeaponType Particle_Beam;
    extern const WeaponType Psi_Blades;
    extern const WeaponType Psi_Blades_Fenix;
    extern const WeaponType Phase_Disruptor;
    extern const WeaponType Phase_Disruptor_Fenix;
    extern const WeaponType Psi_Assault;
    extern const WeaponType Psionic_Shockwave;
    extern const WeaponType Psionic_Shockwave_TZ_Archon;
    extern const WeaponType Dual_Photon_Blasters;
    extern const WeaponType Dual_Photon_Blasters_Mojo;
    extern const WeaponType Dual_Photon_Blasters_Artanis;
    extern const WeaponType Anti_Matter_Missiles;
    extern const WeaponType Anti_Matter_Missiles_Mojo;
    extern const WeaponType Anti_Matter_Missiles_Artanis;
    extern const WeaponType Phase_Disruptor_Cannon;
    extern const WeaponType Phase_Disruptor_Cannon_Danimoth;
    extern const WeaponType Pulse_Cannon;
    extern const WeaponType STS_Photon_Cannon;
    extern const WeaponType STA_Photon_Cannon;
    extern const WeaponType Scarab;
    extern const WeaponType Neutron_Flare;
    extern const WeaponType Halo_Rockets;
    extern const WeaponType Corrosive_Acid;
    extern const WeaponType Subterranean_Spines;
    extern const WeaponType Warp_Blades;
    extern const WeaponType Warp_Blades_Hero;
    extern const WeaponType Warp_Blades_Zeratul;
    extern const WeaponType Independant_Laser_Battery;
    extern const WeaponType Twin_Autocannons_Floor_Trap;
    extern const WeaponType Hellfire_Missile_Pack_Wall_Trap;
    extern const WeaponType Flame_Thrower_Wall_Trap;
    extern const WeaponType Hellfire_Missile_Pack_Floor_Trap;

    extern const WeaponType Yamato_Gun;
    extern const WeaponType Nuclear_Strike;
    extern const WeaponType Lockdown;
    extern const WeaponType EMP_Shockwave;
    extern const WeaponType Irradiate;
    extern const WeaponType Parasite;
    extern const WeaponType Spawn_Broodlings;
    extern const WeaponType Ensnare;
    extern const WeaponType Dark_Swarm;
    extern const WeaponType Plague;
    extern const WeaponType Consume;
    extern const WeaponType Stasis_Field;
    extern const WeaponType Psionic_Storm;
    extern const WeaponType Disruption_Web;
    extern const WeaponType Restoration;
    extern const WeaponType Mind_Control;
    extern const WeaponType Feedback;
    extern const WeaponType Optical_Flare;
    extern const WeaponType Maelstrom;

    extern const WeaponType None;
    extern const WeaponType Unknown;
  }

  static_assert(sizeof(WeaponType) == sizeof(int), "Expected type to resolve to primitive size.");
}
