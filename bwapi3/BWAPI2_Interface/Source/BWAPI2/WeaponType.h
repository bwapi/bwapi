#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>
namespace BWAPI2
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
      int getID() const;
      std::string getName() const;
      const TechType* getTech() const;
      const UnitType* whatUses() const;
      int damageAmount() const;
      int damageBonus() const;
      int damageCooldown() const;
      int damageFactor() const;
      const UpgradeType* upgradeType() const;
      const DamageType* damageType() const;
      const ExplosionType* explosionType() const;
      int minRange() const;
      int maxRange() const;
      int innerSplashRadius() const;
      int medianSplashRadius() const;
      int outerSplashRadius() const;
      bool targetsAir() const;
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
    WeaponType getWeaponType(std::string& name);
    std::set<WeaponType>& allWeaponTypes();
    std::set<WeaponType>& normalWeaponTypes();
    std::set<WeaponType>& specialWeaponTypes();
    void init();
    BWAPI2_EXPORT extern const WeaponType Gauss_Rifle;
    BWAPI2_EXPORT extern const WeaponType C_10_Canister_Rifle;
    BWAPI2_EXPORT extern const WeaponType Fragmentation_Grenade;
    BWAPI2_EXPORT extern const WeaponType Spider_Mines;
    BWAPI2_EXPORT extern const WeaponType Twin_Autocannons;
    BWAPI2_EXPORT extern const WeaponType Hellfire_Missile_Pack;
    BWAPI2_EXPORT extern const WeaponType Arclite_Cannon;
    BWAPI2_EXPORT extern const WeaponType Fusion_Cutter;
    BWAPI2_EXPORT extern const WeaponType Gemini_Missiles;
    BWAPI2_EXPORT extern const WeaponType Burst_Lasers;
    BWAPI2_EXPORT extern const WeaponType ATS_Laser_Battery;
    BWAPI2_EXPORT extern const WeaponType ATA_Laser_Battery;
    BWAPI2_EXPORT extern const WeaponType Flame_Thrower;
    BWAPI2_EXPORT extern const WeaponType Arclite_Shock_Cannon;
    BWAPI2_EXPORT extern const WeaponType Longbolt_Missile;
    BWAPI2_EXPORT extern const WeaponType Claws;
    BWAPI2_EXPORT extern const WeaponType Needle_Spines;
    BWAPI2_EXPORT extern const WeaponType Kaiser_Blades;
    BWAPI2_EXPORT extern const WeaponType Toxic_Spores;
    BWAPI2_EXPORT extern const WeaponType Spines;
    BWAPI2_EXPORT extern const WeaponType Acid_Spore;
    BWAPI2_EXPORT extern const WeaponType Glave_Wurm;
    BWAPI2_EXPORT extern const WeaponType Seeker_Spores;
    BWAPI2_EXPORT extern const WeaponType Subterranean_Tentacle;
    BWAPI2_EXPORT extern const WeaponType Suicide_Infested_Terran;
    BWAPI2_EXPORT extern const WeaponType Suicide_Scourge;
    BWAPI2_EXPORT extern const WeaponType Particle_Beam;
    BWAPI2_EXPORT extern const WeaponType Psi_Blades;
    BWAPI2_EXPORT extern const WeaponType Phase_Disruptor;
    BWAPI2_EXPORT extern const WeaponType Psionic_Shockwave;
    BWAPI2_EXPORT extern const WeaponType Dual_Photon_Blasters;
    BWAPI2_EXPORT extern const WeaponType Anti_Matter_Missiles;
    BWAPI2_EXPORT extern const WeaponType Phase_Disruptor_Cannon;
    BWAPI2_EXPORT extern const WeaponType Pulse_Cannon;
    BWAPI2_EXPORT extern const WeaponType STS_Photon_Cannon;
    BWAPI2_EXPORT extern const WeaponType STA_Photon_Cannon;
    BWAPI2_EXPORT extern const WeaponType Scarab;
    BWAPI2_EXPORT extern const WeaponType Neutron_Flare;
    BWAPI2_EXPORT extern const WeaponType Halo_Rockets;
    BWAPI2_EXPORT extern const WeaponType Corrosive_Acid;
    BWAPI2_EXPORT extern const WeaponType Subterranean_Spines;
    BWAPI2_EXPORT extern const WeaponType Warp_Blades;

    BWAPI2_EXPORT extern const WeaponType Yamato_Gun;
    BWAPI2_EXPORT extern const WeaponType Nuclear_Strike;
    BWAPI2_EXPORT extern const WeaponType Lockdown;
    BWAPI2_EXPORT extern const WeaponType EMP_Shockwave;
    BWAPI2_EXPORT extern const WeaponType Irradiate;
    BWAPI2_EXPORT extern const WeaponType Parasite;
    BWAPI2_EXPORT extern const WeaponType Spawn_Broodlings;
    BWAPI2_EXPORT extern const WeaponType Ensnare;
    BWAPI2_EXPORT extern const WeaponType Dark_Swarm;
    BWAPI2_EXPORT extern const WeaponType Plague;
    BWAPI2_EXPORT extern const WeaponType Consume;
    BWAPI2_EXPORT extern const WeaponType Stasis_Field;
    BWAPI2_EXPORT extern const WeaponType Psionic_Storm;
    BWAPI2_EXPORT extern const WeaponType Disruption_Web;
    BWAPI2_EXPORT extern const WeaponType Restoration;
    BWAPI2_EXPORT extern const WeaponType Mind_Control;
    BWAPI2_EXPORT extern const WeaponType Feedback;
    BWAPI2_EXPORT extern const WeaponType Optical_Flare;
    BWAPI2_EXPORT extern const WeaponType Maelstrom;

    BWAPI2_EXPORT extern const WeaponType None;
    BWAPI2_EXPORT extern const WeaponType Unknown;
  }
}
