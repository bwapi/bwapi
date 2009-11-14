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
      WeaponType& __cdecl operator=(const WeaponType& other);
      bool __cdecl operator==(const WeaponType& other) const;
      bool __cdecl operator!=(const WeaponType& other) const;
      bool __cdecl operator<(const WeaponType& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
      const TechType* __cdecl getTech() const;
      const UnitType* __cdecl whatUses() const;
      int __cdecl damageAmount() const;
      int __cdecl damageBonus() const;
      int __cdecl damageCooldown() const;
      int __cdecl damageFactor() const;
      const UpgradeType* __cdecl upgradeType() const;
      const DamageType* __cdecl damageType() const;
      const ExplosionType* __cdecl explosionType() const;
      int __cdecl minRange() const;
      int __cdecl maxRange() const;
      int __cdecl innerSplashRadius() const;
      int __cdecl medianSplashRadius() const;
      int __cdecl outerSplashRadius() const;
      bool __cdecl targetsAir() const;
      bool __cdecl targetsGround() const;
      bool __cdecl targetsMechanical() const;
      bool __cdecl targetsOrganic() const;
      bool __cdecl targetsNonBuilding() const;
      bool __cdecl targetsNonRobotic() const;
      bool __cdecl targetsTerrain() const;
      bool __cdecl targetsOrgOrMech() const;
      bool __cdecl targetsOwn() const;
    private:
      int id;
  };
  namespace WeaponTypes
  {
    WeaponType __cdecl getWeaponType(std::string& name);
    std::set<WeaponType>& __cdecl allWeaponTypes();
    std::set<WeaponType>& __cdecl normalWeaponTypes();
    std::set<WeaponType>& __cdecl specialWeaponTypes();
    void __cdecl init();
    extern const WeaponType Gauss_Rifle;
    extern const WeaponType C_10_Canister_Rifle;
    extern const WeaponType Fragmentation_Grenade;
    extern const WeaponType Spider_Mines;
    extern const WeaponType Twin_Autocannons;
    extern const WeaponType Hellfire_Missile_Pack;
    extern const WeaponType Arclite_Cannon;
    extern const WeaponType Fusion_Cutter;
    extern const WeaponType Gemini_Missiles;
    extern const WeaponType Burst_Lasers;
    extern const WeaponType ATS_Laser_Battery;
    extern const WeaponType ATA_Laser_Battery;
    extern const WeaponType Flame_Thrower;
    extern const WeaponType Arclite_Shock_Cannon;
    extern const WeaponType Longbolt_Missile;
    extern const WeaponType Claws;
    extern const WeaponType Needle_Spines;
    extern const WeaponType Kaiser_Blades;
    extern const WeaponType Toxic_Spores;
    extern const WeaponType Spines;
    extern const WeaponType Acid_Spore;
    extern const WeaponType Glave_Wurm;
    extern const WeaponType Seeker_Spores;
    extern const WeaponType Subterranean_Tentacle;
    extern const WeaponType Suicide_Infested_Terran;
    extern const WeaponType Suicide_Scourge;
    extern const WeaponType Particle_Beam;
    extern const WeaponType Psi_Blades;
    extern const WeaponType Phase_Disruptor;
    extern const WeaponType Psionic_Shockwave;
    extern const WeaponType Dual_Photon_Blasters;
    extern const WeaponType Anti_matter_Missiles;
    extern const WeaponType Phase_Disruptor_Cannon;
    extern const WeaponType Pulse_Cannon;
    extern const WeaponType STS_Photon_Cannon;
    extern const WeaponType STA_Photon_Cannon;
    extern const WeaponType Scarab;
    extern const WeaponType Neutron_Flare;
    extern const WeaponType Halo_Rockets;
    extern const WeaponType Corrosive_Acid;
    extern const WeaponType Subterranean_Spines;
    extern const WeaponType Warp_Blades;

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
