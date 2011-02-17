#pragma once
#include <string>
#include <set>

namespace BWAPI
{
  class BulletType
  {
    public:
      BulletType();
      BulletType(int id);
      BulletType(const BulletType& other);
      BulletType& operator=(const BulletType& other);
      operator int() const;

      /** Returns the unique ID for this bullet type. */
      int getID() const;

      /** Returns the name of this bullet type. */
      std::string getName() const;
    private:
      int id;
  };

  namespace BulletTypes
  {
    /** Given the name of an bullet type, getBulletType() will return the corresponding BulletType object. */
    BulletType getBulletType(std::string name);

    /** Returns the set of all the BulletTypes. */
    std::set<BulletType>& allBulletTypes();
    void init();
    extern const BulletType Melee;
    extern const BulletType Fusion_Cutter_Hit;
    extern const BulletType Gauss_Rifle_Hit;
    extern const BulletType C_10_Canister_Rifle_Hit;
    extern const BulletType Gemini_Missiles;
    extern const BulletType Fragmentation_Grenade;
    extern const BulletType Longbolt_Missile;
    extern const BulletType ATS_ATA_Laser_Battery;
    extern const BulletType Burst_Lasers;
    extern const BulletType Arclite_Shock_Cannon_Hit;
    extern const BulletType EMP_Missile;
    extern const BulletType Dual_Photon_Blasters_Hit;
    extern const BulletType Particle_Beam_Hit;
    extern const BulletType Anti_Matter_Missile;
    extern const BulletType Pulse_Cannon;
    extern const BulletType Psionic_Shockwave_Hit;
    extern const BulletType Psionic_Storm;
    extern const BulletType Yamato_Gun;
    extern const BulletType Phase_Disruptor;
    extern const BulletType STA_STS_Cannon_Overlay;
    extern const BulletType Sunken_Colony_Tentacle;
    extern const BulletType Acid_Spore;
    extern const BulletType Glave_Wurm;
    extern const BulletType Seeker_Spores;
    extern const BulletType Queen_Spell_Carrier;
    extern const BulletType Plague_Cloud;
    extern const BulletType Consume;
    extern const BulletType Needle_Spine_Hit;
    extern const BulletType Invisible;
    extern const BulletType Optical_Flare_Grenade;
    extern const BulletType Halo_Rockets;
    extern const BulletType Subterranean_Spines;
    extern const BulletType Corrosive_Acid_Shot;
    extern const BulletType Neutron_Flare;
    extern const BulletType None;
    extern const BulletType Unknown;
  };
}
