#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class TechType;
  class UpgradeType;
  class DamageType;
  class ExplosionType;
  class WeaponType
  {
    public:
      WeaponType();
      WeaponType(int id);
      WeaponType(const WeaponType& other);
      WeaponType& operator=(const WeaponType& other);
      bool operator==(const WeaponType& other) const;
      bool operator!=(const WeaponType& other) const;
      bool operator<(const WeaponType& other) const;

      /** Returns a unique ID for this weapon type. */
      int getID() const;

      /** Returns the name of the weapon. */
      std::string getName() const;

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
    private:
      int id;
  };
  namespace WeaponTypes
  {
    /** Given the name of a weapon, this will return the corresponding weapon type object. */
    WeaponType getWeaponType(std::string name);

    /** Returns the set of all the WeaponTypes. */
    std::set<WeaponType>& allWeaponTypes();

    /** Returns the set of all normal weapons in WeaponTypes. */
    std::set<WeaponType>& normalWeaponTypes();

    /** Returns the set of all special weapons in WeaponTypes. */
    std::set<WeaponType>& specialWeaponTypes();
    void init();
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
}
