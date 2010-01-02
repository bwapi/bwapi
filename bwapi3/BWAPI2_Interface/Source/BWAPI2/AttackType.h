#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
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
    BWAPI2_EXPORT extern const AttackType Melee;
    BWAPI2_EXPORT extern const AttackType Fusion_Cutter_Hit;
    BWAPI2_EXPORT extern const AttackType Gauss_Rifle_Hit;
    BWAPI2_EXPORT extern const AttackType C_10_Canister_Rifle_Hit;
    BWAPI2_EXPORT extern const AttackType Gemini_Missiles;
    BWAPI2_EXPORT extern const AttackType Fragmentation_Grenade;
    BWAPI2_EXPORT extern const AttackType Longbolt_Missile;
    BWAPI2_EXPORT extern const AttackType ATS_ATA_Laser_Battery;
    BWAPI2_EXPORT extern const AttackType Burst_Lasers;
    BWAPI2_EXPORT extern const AttackType Arclite_Shock_Cannon_Hit;
    BWAPI2_EXPORT extern const AttackType EMP_Missile;
    BWAPI2_EXPORT extern const AttackType Dual_Photon_Blasters_Hit;
    BWAPI2_EXPORT extern const AttackType Particle_Beam_Hit;
    BWAPI2_EXPORT extern const AttackType Anti_Matter_Missile;
    BWAPI2_EXPORT extern const AttackType Pulse_Cannon;
    BWAPI2_EXPORT extern const AttackType Psionic_Shockwave_Hit;
    BWAPI2_EXPORT extern const AttackType Psionic_Storm;
    BWAPI2_EXPORT extern const AttackType Yamato_Gun;
    BWAPI2_EXPORT extern const AttackType Phase_Disruptor;
    BWAPI2_EXPORT extern const AttackType STA_STS_Cannon_Overlay;
    BWAPI2_EXPORT extern const AttackType Sunken_Colony_Tentacle;
    BWAPI2_EXPORT extern const AttackType Acid_Spore;
    BWAPI2_EXPORT extern const AttackType Glave_Wurm;
    BWAPI2_EXPORT extern const AttackType Seeker_Spores;
    BWAPI2_EXPORT extern const AttackType Queen_Spell_Carrier;
    BWAPI2_EXPORT extern const AttackType Plague_Cloud;
    BWAPI2_EXPORT extern const AttackType Consume;
    BWAPI2_EXPORT extern const AttackType Needle_Spine_Hit;
    BWAPI2_EXPORT extern const AttackType Invisible;
    BWAPI2_EXPORT extern const AttackType Optical_Flare_Grenade;
    BWAPI2_EXPORT extern const AttackType Halo_Rockets;
    BWAPI2_EXPORT extern const AttackType Subterranean_Spines;
    BWAPI2_EXPORT extern const AttackType Corrosive_Acid_Shot;
    BWAPI2_EXPORT extern const AttackType Neutron_Flare;
    BWAPI2_EXPORT extern const AttackType None;
    BWAPI2_EXPORT extern const AttackType Unknown;
  };
}
