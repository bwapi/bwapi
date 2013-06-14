#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class TechType;
  class UpgradeType;
  class DamageType;
  class ExplosionType;

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
    const WeaponType::const_set& normalWeaponTypes();

    /// Retrieves the set of all special weapon types. This set contains all weapons that are
    /// used exclusively for special unit abilities.
    ///
    /// @returns constant set consisting of all special weapon types.
    const WeaponType::const_set& specialWeaponTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const WeaponType x
    BWAPI_DECL(Gauss_Rifle);
    BWAPI_DECL(Gauss_Rifle_Jim_Raynor);
    BWAPI_DECL(C_10_Canister_Rifle);
    BWAPI_DECL(C_10_Canister_Rifle_Sarah_Kerrigan);
    BWAPI_DECL(C_10_Canister_Rifle_Samir_Duran);
    BWAPI_DECL(C_10_Canister_Rifle_Infested_Duran);
    BWAPI_DECL(C_10_Canister_Rifle_Alexei_Stukov);
    BWAPI_DECL(Fragmentation_Grenade);
    BWAPI_DECL(Fragmentation_Grenade_Jim_Raynor);
    BWAPI_DECL(Spider_Mines);
    BWAPI_DECL(Twin_Autocannons);
    BWAPI_DECL(Twin_Autocannons_Alan_Schezar);
    BWAPI_DECL(Hellfire_Missile_Pack);
    BWAPI_DECL(Hellfire_Missile_Pack_Alan_Schezar);
    BWAPI_DECL(Arclite_Cannon);
    BWAPI_DECL(Arclite_Cannon_Edmund_Duke);
    BWAPI_DECL(Fusion_Cutter);
    BWAPI_DECL(Gemini_Missiles);
    BWAPI_DECL(Gemini_Missiles_Tom_Kazansky);
    BWAPI_DECL(Burst_Lasers);
    BWAPI_DECL(Burst_Lasers_Tom_Kazansky);
    BWAPI_DECL(ATS_Laser_Battery);
    BWAPI_DECL(ATS_Laser_Battery_Hero);
    BWAPI_DECL(ATS_Laser_Battery_Hyperion);
    BWAPI_DECL(ATA_Laser_Battery);
    BWAPI_DECL(ATA_Laser_Battery_Hero);
    BWAPI_DECL(ATA_Laser_Battery_Hyperion);
    BWAPI_DECL(Flame_Thrower);
    BWAPI_DECL(Flame_Thrower_Gui_Montag);
    BWAPI_DECL(Arclite_Shock_Cannon);
    BWAPI_DECL(Arclite_Shock_Cannon_Edmund_Duke);
    BWAPI_DECL(Longbolt_Missile);
    BWAPI_DECL(Claws);
    BWAPI_DECL(Claws_Devouring_One);
    BWAPI_DECL(Claws_Infested_Kerrigan);
    BWAPI_DECL(Needle_Spines);
    BWAPI_DECL(Needle_Spines_Hunter_Killer);
    BWAPI_DECL(Kaiser_Blades);
    BWAPI_DECL(Kaiser_Blades_Torrasque);
    BWAPI_DECL(Toxic_Spores);
    BWAPI_DECL(Spines);
    BWAPI_DECL(Acid_Spore);
    BWAPI_DECL(Acid_Spore_Kukulza);
    BWAPI_DECL(Glave_Wurm);
    BWAPI_DECL(Glave_Wurm_Kukulza);
    BWAPI_DECL(Seeker_Spores);
    BWAPI_DECL(Subterranean_Tentacle);
    BWAPI_DECL(Suicide_Infested_Terran);
    BWAPI_DECL(Suicide_Scourge);
    BWAPI_DECL(Particle_Beam);
    BWAPI_DECL(Psi_Blades);
    BWAPI_DECL(Psi_Blades_Fenix);
    BWAPI_DECL(Phase_Disruptor);
    BWAPI_DECL(Phase_Disruptor_Fenix);
    BWAPI_DECL(Psi_Assault);
    BWAPI_DECL(Psionic_Shockwave);
    BWAPI_DECL(Psionic_Shockwave_TZ_Archon);
    BWAPI_DECL(Dual_Photon_Blasters);
    BWAPI_DECL(Dual_Photon_Blasters_Mojo);
    BWAPI_DECL(Dual_Photon_Blasters_Artanis);
    BWAPI_DECL(Anti_Matter_Missiles);
    BWAPI_DECL(Anti_Matter_Missiles_Mojo);
    BWAPI_DECL(Anti_Matter_Missiles_Artanis);
    BWAPI_DECL(Phase_Disruptor_Cannon);
    BWAPI_DECL(Phase_Disruptor_Cannon_Danimoth);
    BWAPI_DECL(Pulse_Cannon);
    BWAPI_DECL(STS_Photon_Cannon);
    BWAPI_DECL(STA_Photon_Cannon);
    BWAPI_DECL(Scarab);
    BWAPI_DECL(Neutron_Flare);
    BWAPI_DECL(Halo_Rockets);
    BWAPI_DECL(Corrosive_Acid);
    BWAPI_DECL(Subterranean_Spines);
    BWAPI_DECL(Warp_Blades);
    BWAPI_DECL(Warp_Blades_Hero);
    BWAPI_DECL(Warp_Blades_Zeratul);
    BWAPI_DECL(Independant_Laser_Battery);
    BWAPI_DECL(Twin_Autocannons_Floor_Trap);
    BWAPI_DECL(Hellfire_Missile_Pack_Wall_Trap);
    BWAPI_DECL(Flame_Thrower_Wall_Trap);
    BWAPI_DECL(Hellfire_Missile_Pack_Floor_Trap);

    BWAPI_DECL(Yamato_Gun);
    BWAPI_DECL(Nuclear_Strike);
    BWAPI_DECL(Lockdown);
    BWAPI_DECL(EMP_Shockwave);
    BWAPI_DECL(Irradiate);
    BWAPI_DECL(Parasite);
    BWAPI_DECL(Spawn_Broodlings);
    BWAPI_DECL(Ensnare);
    BWAPI_DECL(Dark_Swarm);
    BWAPI_DECL(Plague);
    BWAPI_DECL(Consume);
    BWAPI_DECL(Stasis_Field);
    BWAPI_DECL(Psionic_Storm);
    BWAPI_DECL(Disruption_Web);
    BWAPI_DECL(Restoration);
    BWAPI_DECL(Mind_Control);
    BWAPI_DECL(Feedback);
    BWAPI_DECL(Optical_Flare);
    BWAPI_DECL(Maelstrom);

    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
