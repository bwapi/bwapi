#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing bullet types.</summary>
  /// @see BulletType
  namespace BulletTypes
  {
    /// <summary>Enumeration of bullet types.</summary>
    /// @see BulletType
    namespace Enum
    {
      /// <summary>Enumeration of bullet types.</summary>
      /// @see BulletType
      enum Enum
      {
        Melee = 0,

        Fusion_Cutter_Hit = 141,
        Gauss_Rifle_Hit,
        C_10_Canister_Rifle_Hit,
        Gemini_Missiles,
        Fragmentation_Grenade,
        Longbolt_Missile,
        Unused_Lockdown,
        ATS_ATA_Laser_Battery,
        Burst_Lasers,
        Arclite_Shock_Cannon_Hit,
        EMP_Missile,
        Dual_Photon_Blasters_Hit,
        Particle_Beam_Hit,
        Anti_Matter_Missile,
        Pulse_Cannon,
        Psionic_Shockwave_Hit,
        Psionic_Storm,
        Yamato_Gun,
        Phase_Disruptor,
        STA_STS_Cannon_Overlay,
        Sunken_Colony_Tentacle,
        Venom_Unused,
        Acid_Spore,
        Plasma_Drip_Unused,
        Glave_Wurm,
        Seeker_Spores,
        Queen_Spell_Carrier,
        Plague_Cloud,
        Consume,
        Ensnare,
        Needle_Spine_Hit,
        Invisible,

        Optical_Flare_Grenade = 201,
        Halo_Rockets,
        Subterranean_Spines,
        Corrosive_Acid_Shot,
        Corrosive_Acid_Hit,
        Neutron_Flare,

        None = 209,
        Unknown,
        MAX
      };
    }
  }
  /// <summary>This class represents a type of bullet. </summary>
  ///
  /// @note Internally, these are the same IDs as flingy types in Broodwar.
  ///
  /// @see BulletTypes
  /// @ingroup TypeClasses
  class BulletType : public Type<BulletType, BulletTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      BulletType(int id = BulletTypes::Enum::None);
  };

  /// @ingroup Types
  namespace BulletTypes
  {
    /// <summary>Retrieves the set of all the BulletTypes.</summary>
    ///
    /// @returns Set of BulletTypes.
    const BulletType::set& allBulletTypes();

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
    extern const BulletType Ensnare;
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

  static_assert(sizeof(BulletType) == sizeof(int), "Expected type to resolve to primitive size.");
}
