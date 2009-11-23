#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class AttackType
  {
    public:
      AttackType();
      AttackType(int id);
      AttackType(const AttackType& other);
      AttackType& operator=(const AttackType& other);
      bool operator==(const AttackType& other) const;
      bool operator!=(const AttackType& other) const;
      bool operator<(const AttackType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace AttackTypes
  {
    AttackType getAttackType(std::string& name);
    std::set<AttackType>& allAttackTypes();
    void init();
    extern const AttackType Archon_Energy;
    extern const AttackType Fusion_Cutter_Hit;
    extern const AttackType Gauss_Rifle_Hit;
    extern const AttackType C_10_Canister_Rifle_Hit;
    extern const AttackType Gemini_Missiles;
    extern const AttackType Fragmentation_Grenade;
    extern const AttackType Hellfire_Missile;
    extern const AttackType Unknown147;
    extern const AttackType ATS_ATA_Laser_Battery;
    extern const AttackType Burst_Lasers;
    extern const AttackType Arclite_Shock_Cannon_Hit;
    extern const AttackType EMP_Missile;
    extern const AttackType Dual_Photon_Blasters_Hit;
    extern const AttackType Particle_Beam_Hit;
    extern const AttackType Anti_Matter_Missile;
    extern const AttackType Pulse_Cannon;
    extern const AttackType Psionic_Shockwave_Hit;
    extern const AttackType Psionic_Storm;
    extern const AttackType Yamato_Gun;
    extern const AttackType Phase_Disruptor;
    extern const AttackType STA_STS_Cannon_Overlay;
    extern const AttackType Sunken_Colony_Tentacle;
    extern const AttackType Venom_Unused_Zerg_Weapon;
    extern const AttackType Acid_Spore;
    extern const AttackType Unknown164;
    extern const AttackType Glave_Wurm;
    extern const AttackType Seeker_Spores;
    extern const AttackType Queen_Spell_Carrier;
    extern const AttackType Plague_Cloud;
    extern const AttackType Consume;
    extern const AttackType Ensnare;
    extern const AttackType Needle_Spine_Hit;
    extern const AttackType White_Circle_Invisible;
    extern const AttackType Dark_Archon_Energy;
    extern const AttackType Optical_Flare_Grenade;
    extern const AttackType Halo_Rockets;
    extern const AttackType Subterranean_Spines;
    extern const AttackType Corrosive_Acid_Shot;
    extern const AttackType Corrosive_Acid_Hit;
    extern const AttackType Neutron_Flare;
    extern const AttackType None;
    extern const AttackType Unknown;
  };
}
