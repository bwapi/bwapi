#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
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
    BWSL_EXPORT extern const AttackType Melee;
    BWSL_EXPORT extern const AttackType Fusion_Cutter_Hit;
    BWSL_EXPORT extern const AttackType Gauss_Rifle_Hit;
    BWSL_EXPORT extern const AttackType C_10_Canister_Rifle_Hit;
    BWSL_EXPORT extern const AttackType Gemini_Missiles;
    BWSL_EXPORT extern const AttackType Fragmentation_Grenade;
    BWSL_EXPORT extern const AttackType Longbolt_Missile;
    BWSL_EXPORT extern const AttackType ATS_ATA_Laser_Battery;
    BWSL_EXPORT extern const AttackType Burst_Lasers;
    BWSL_EXPORT extern const AttackType Arclite_Shock_Cannon_Hit;
    BWSL_EXPORT extern const AttackType EMP_Missile;
    BWSL_EXPORT extern const AttackType Dual_Photon_Blasters_Hit;
    BWSL_EXPORT extern const AttackType Particle_Beam_Hit;
    BWSL_EXPORT extern const AttackType Anti_Matter_Missile;
    BWSL_EXPORT extern const AttackType Pulse_Cannon;
    BWSL_EXPORT extern const AttackType Psionic_Shockwave_Hit;
    BWSL_EXPORT extern const AttackType Psionic_Storm;
    BWSL_EXPORT extern const AttackType Yamato_Gun;
    BWSL_EXPORT extern const AttackType Phase_Disruptor;
    BWSL_EXPORT extern const AttackType STA_STS_Cannon_Overlay;
    BWSL_EXPORT extern const AttackType Sunken_Colony_Tentacle;
    BWSL_EXPORT extern const AttackType Acid_Spore;
    BWSL_EXPORT extern const AttackType Glave_Wurm;
    BWSL_EXPORT extern const AttackType Seeker_Spores;
    BWSL_EXPORT extern const AttackType Queen_Spell_Carrier;
    BWSL_EXPORT extern const AttackType Plague_Cloud;
    BWSL_EXPORT extern const AttackType Consume;
    BWSL_EXPORT extern const AttackType Needle_Spine_Hit;
    BWSL_EXPORT extern const AttackType Invisible;
    BWSL_EXPORT extern const AttackType Optical_Flare_Grenade;
    BWSL_EXPORT extern const AttackType Halo_Rockets;
    BWSL_EXPORT extern const AttackType Subterranean_Spines;
    BWSL_EXPORT extern const AttackType Corrosive_Acid_Shot;
    BWSL_EXPORT extern const AttackType Neutron_Flare;
    BWSL_EXPORT extern const AttackType None;
    BWSL_EXPORT extern const AttackType Unknown;
  };
}
