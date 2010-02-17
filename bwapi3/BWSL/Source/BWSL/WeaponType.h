#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>
namespace BWSL
{
  class TechType;
  class UpgradeType;
  class DamageType;
  class ExplosionType;
  class BWSL_EXPORT WeaponType
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
    BWSL_EXPORT extern const WeaponType Gauss_Rifle;
    BWSL_EXPORT extern const WeaponType C_10_Canister_Rifle;
    BWSL_EXPORT extern const WeaponType Fragmentation_Grenade;
    BWSL_EXPORT extern const WeaponType Spider_Mines;
    BWSL_EXPORT extern const WeaponType Twin_Autocannons;
    BWSL_EXPORT extern const WeaponType Hellfire_Missile_Pack;
    BWSL_EXPORT extern const WeaponType Arclite_Cannon;
    BWSL_EXPORT extern const WeaponType Fusion_Cutter;
    BWSL_EXPORT extern const WeaponType Gemini_Missiles;
    BWSL_EXPORT extern const WeaponType Burst_Lasers;
    BWSL_EXPORT extern const WeaponType ATS_Laser_Battery;
    BWSL_EXPORT extern const WeaponType ATA_Laser_Battery;
    BWSL_EXPORT extern const WeaponType Flame_Thrower;
    BWSL_EXPORT extern const WeaponType Arclite_Shock_Cannon;
    BWSL_EXPORT extern const WeaponType Longbolt_Missile;
    BWSL_EXPORT extern const WeaponType Claws;
    BWSL_EXPORT extern const WeaponType Needle_Spines;
    BWSL_EXPORT extern const WeaponType Kaiser_Blades;
    BWSL_EXPORT extern const WeaponType Toxic_Spores;
    BWSL_EXPORT extern const WeaponType Spines;
    BWSL_EXPORT extern const WeaponType Acid_Spore;
    BWSL_EXPORT extern const WeaponType Glave_Wurm;
    BWSL_EXPORT extern const WeaponType Seeker_Spores;
    BWSL_EXPORT extern const WeaponType Subterranean_Tentacle;
    BWSL_EXPORT extern const WeaponType Suicide_Infested_Terran;
    BWSL_EXPORT extern const WeaponType Suicide_Scourge;
    BWSL_EXPORT extern const WeaponType Particle_Beam;
    BWSL_EXPORT extern const WeaponType Psi_Blades;
    BWSL_EXPORT extern const WeaponType Phase_Disruptor;
    BWSL_EXPORT extern const WeaponType Psionic_Shockwave;
    BWSL_EXPORT extern const WeaponType Dual_Photon_Blasters;
    BWSL_EXPORT extern const WeaponType Anti_Matter_Missiles;
    BWSL_EXPORT extern const WeaponType Phase_Disruptor_Cannon;
    BWSL_EXPORT extern const WeaponType Pulse_Cannon;
    BWSL_EXPORT extern const WeaponType STS_Photon_Cannon;
    BWSL_EXPORT extern const WeaponType STA_Photon_Cannon;
    BWSL_EXPORT extern const WeaponType Scarab;
    BWSL_EXPORT extern const WeaponType Neutron_Flare;
    BWSL_EXPORT extern const WeaponType Halo_Rockets;
    BWSL_EXPORT extern const WeaponType Corrosive_Acid;
    BWSL_EXPORT extern const WeaponType Subterranean_Spines;
    BWSL_EXPORT extern const WeaponType Warp_Blades;

    BWSL_EXPORT extern const WeaponType Yamato_Gun;
    BWSL_EXPORT extern const WeaponType Nuclear_Strike;
    BWSL_EXPORT extern const WeaponType Lockdown;
    BWSL_EXPORT extern const WeaponType EMP_Shockwave;
    BWSL_EXPORT extern const WeaponType Irradiate;
    BWSL_EXPORT extern const WeaponType Parasite;
    BWSL_EXPORT extern const WeaponType Spawn_Broodlings;
    BWSL_EXPORT extern const WeaponType Ensnare;
    BWSL_EXPORT extern const WeaponType Dark_Swarm;
    BWSL_EXPORT extern const WeaponType Plague;
    BWSL_EXPORT extern const WeaponType Consume;
    BWSL_EXPORT extern const WeaponType Stasis_Field;
    BWSL_EXPORT extern const WeaponType Psionic_Storm;
    BWSL_EXPORT extern const WeaponType Disruption_Web;
    BWSL_EXPORT extern const WeaponType Restoration;
    BWSL_EXPORT extern const WeaponType Mind_Control;
    BWSL_EXPORT extern const WeaponType Feedback;
    BWSL_EXPORT extern const WeaponType Optical_Flare;
    BWSL_EXPORT extern const WeaponType Maelstrom;

    BWSL_EXPORT extern const WeaponType None;
    BWSL_EXPORT extern const WeaponType Unknown;
  }
}
