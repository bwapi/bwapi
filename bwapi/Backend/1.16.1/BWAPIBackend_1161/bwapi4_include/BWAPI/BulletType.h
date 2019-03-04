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
      constexpr BulletType(int id = BulletTypes::Enum::None) : Type(id) {}
  };
  /// @ingroup Types
  namespace BulletTypes
  {
    /// <summary>Retrieves the set of all the BulletTypes.</summary>
    ///
    /// @returns Set of BulletTypes.
    const BulletType::set& allBulletTypes();

    constexpr BulletType Melee{Enum::Melee};
    constexpr BulletType Fusion_Cutter_Hit{Enum::Fusion_Cutter_Hit};
    constexpr BulletType Gauss_Rifle_Hit{Enum::Gauss_Rifle_Hit};
    constexpr BulletType C_10_Canister_Rifle_Hit{Enum::C_10_Canister_Rifle_Hit};
    constexpr BulletType Gemini_Missiles{Enum::Gemini_Missiles};
    constexpr BulletType Fragmentation_Grenade{Enum::Fragmentation_Grenade};
    constexpr BulletType Longbolt_Missile{Enum::Longbolt_Missile};
    constexpr BulletType ATS_ATA_Laser_Battery{Enum::ATS_ATA_Laser_Battery};
    constexpr BulletType Burst_Lasers{Enum::Burst_Lasers};
    constexpr BulletType Arclite_Shock_Cannon_Hit{Enum::Arclite_Shock_Cannon_Hit};
    constexpr BulletType EMP_Missile{Enum::EMP_Missile};
    constexpr BulletType Dual_Photon_Blasters_Hit{Enum::Dual_Photon_Blasters_Hit};
    constexpr BulletType Particle_Beam_Hit{Enum::Particle_Beam_Hit};
    constexpr BulletType Anti_Matter_Missile{Enum::Anti_Matter_Missile};
    constexpr BulletType Pulse_Cannon{Enum::Pulse_Cannon};
    constexpr BulletType Psionic_Shockwave_Hit{Enum::Psionic_Shockwave_Hit};
    constexpr BulletType Psionic_Storm{Enum::Psionic_Storm};
    constexpr BulletType Yamato_Gun{Enum::Yamato_Gun};
    constexpr BulletType Phase_Disruptor{Enum::Phase_Disruptor};
    constexpr BulletType STA_STS_Cannon_Overlay{Enum::STA_STS_Cannon_Overlay};
    constexpr BulletType Sunken_Colony_Tentacle{Enum::Sunken_Colony_Tentacle};
    constexpr BulletType Acid_Spore{Enum::Acid_Spore};
    constexpr BulletType Glave_Wurm{Enum::Glave_Wurm};
    constexpr BulletType Seeker_Spores{Enum::Seeker_Spores};
    constexpr BulletType Queen_Spell_Carrier{Enum::Queen_Spell_Carrier};
    constexpr BulletType Plague_Cloud{Enum::Plague_Cloud};
    constexpr BulletType Consume{Enum::Consume};
    constexpr BulletType Ensnare{Enum::Ensnare};
    constexpr BulletType Needle_Spine_Hit{Enum::Needle_Spine_Hit};
    constexpr BulletType Invisible{Enum::Invisible};
    constexpr BulletType Optical_Flare_Grenade{Enum::Optical_Flare_Grenade};
    constexpr BulletType Halo_Rockets{Enum::Halo_Rockets};
    constexpr BulletType Subterranean_Spines{Enum::Subterranean_Spines};
    constexpr BulletType Corrosive_Acid_Shot{Enum::Corrosive_Acid_Shot};
    constexpr BulletType Neutron_Flare{Enum::Neutron_Flare};
    constexpr BulletType None{Enum::None};
    constexpr BulletType Unknown{Enum::Unknown};
  };
  static_assert(sizeof(BulletType) == sizeof(int), "Expected type to resolve to primitive size.");
}
