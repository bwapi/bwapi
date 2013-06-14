#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace BulletTypes
  {
    /// Enumeration of bullet types
    namespace Enum
    {
      /// Enumeration of bullet types
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
  class BulletType : public Type<BulletType, BulletTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      BulletType(int id = BulletTypes::Enum::None);
  };

  /// Namespace containing bullet types
  namespace BulletTypes
  {
    /// Retrieves the set of all the BulletTypes.
    ///
    /// @returns Set of BulletTypes.
    const BulletType::const_set& allBulletTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const BulletType x
    BWAPI_DECL(Melee);
    BWAPI_DECL(Fusion_Cutter_Hit);
    BWAPI_DECL(Gauss_Rifle_Hit);
    BWAPI_DECL(C_10_Canister_Rifle_Hit);
    BWAPI_DECL(Gemini_Missiles);
    BWAPI_DECL(Fragmentation_Grenade);
    BWAPI_DECL(Longbolt_Missile);
    BWAPI_DECL(ATS_ATA_Laser_Battery);
    BWAPI_DECL(Burst_Lasers);
    BWAPI_DECL(Arclite_Shock_Cannon_Hit);
    BWAPI_DECL(EMP_Missile);
    BWAPI_DECL(Dual_Photon_Blasters_Hit);
    BWAPI_DECL(Particle_Beam_Hit);
    BWAPI_DECL(Anti_Matter_Missile);
    BWAPI_DECL(Pulse_Cannon);
    BWAPI_DECL(Psionic_Shockwave_Hit);
    BWAPI_DECL(Psionic_Storm);
    BWAPI_DECL(Yamato_Gun);
    BWAPI_DECL(Phase_Disruptor);
    BWAPI_DECL(STA_STS_Cannon_Overlay);
    BWAPI_DECL(Sunken_Colony_Tentacle);
    BWAPI_DECL(Acid_Spore);
    BWAPI_DECL(Glave_Wurm);
    BWAPI_DECL(Seeker_Spores);
    BWAPI_DECL(Queen_Spell_Carrier);
    BWAPI_DECL(Plague_Cloud);
    BWAPI_DECL(Consume);
    BWAPI_DECL(Ensnare);
    BWAPI_DECL(Needle_Spine_Hit);
    BWAPI_DECL(Invisible);
    BWAPI_DECL(Optical_Flare_Grenade);
    BWAPI_DECL(Halo_Rockets);
    BWAPI_DECL(Subterranean_Spines);
    BWAPI_DECL(Corrosive_Acid_Shot);
    BWAPI_DECL(Neutron_Flare);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  };
}
