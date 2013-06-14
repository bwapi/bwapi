#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace ExplosionTypes
  {
    /// Enumeration of explosion types
    namespace Enum
    {
      /// Enumeration of explosion types
      enum Enum
      {
        None = 0,
        Normal,
        Radial_Splash,
        Enemy_Splash,
        Lockdown,
        Nuclear_Missile,
        Parasite,
        Broodlings,
        EMP_Shockwave,
        Irradiate,
        Ensnare,
        Plague,
        Stasis_Field,
        Dark_Swarm,
        Consume,
        Yamato_Gun,
        Restoration,
        Disruption_Web,
        Corrosive_Acid,
        Mind_Control,
        Feedback,
        Optical_Flare,
        Maelstrom,
        Unused,
        Air_Splash,
        Unknown,
        MAX
      };
    };
  };
  class ExplosionType : public Type<ExplosionType, ExplosionTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      ExplosionType(int id = ExplosionTypes::Enum::None);
  };
  /// Namespace containing explosion types
  namespace ExplosionTypes
  {
    /// Retrieves the set of all ExplosionTypes.
    ///
    /// @returns Set of ExplosionTypes.
    const ExplosionType::const_set& allExplosionTypes();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const ExplosionType x
    BWAPI_DECL(None);
    BWAPI_DECL(Normal);
    BWAPI_DECL(Radial_Splash);
    BWAPI_DECL(Enemy_Splash);
    BWAPI_DECL(Lockdown);
    BWAPI_DECL(Nuclear_Missile);
    BWAPI_DECL(Parasite);
    BWAPI_DECL(Broodlings);
    BWAPI_DECL(EMP_Shockwave);
    BWAPI_DECL(Irradiate);
    BWAPI_DECL(Ensnare);
    BWAPI_DECL(Plague);
    BWAPI_DECL(Stasis_Field);
    BWAPI_DECL(Dark_Swarm);
    BWAPI_DECL(Consume);
    BWAPI_DECL(Yamato_Gun);
    BWAPI_DECL(Restoration);
    BWAPI_DECL(Disruption_Web);
    BWAPI_DECL(Corrosive_Acid);
    BWAPI_DECL(Mind_Control);
    BWAPI_DECL(Feedback);
    BWAPI_DECL(Optical_Flare);
    BWAPI_DECL(Maelstrom);
    BWAPI_DECL(Air_Splash);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
