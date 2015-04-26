#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing explosion types.</summary>
  /// @see ExplosionType
  namespace ExplosionTypes
  {
    /// <summary>Enumeration of explosion types.</summary>
    /// @see ExplosionType
    namespace Enum
    {
      /// <summary>Enumeration of explosion types.</summary>
      /// @see ExplosionType
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
  /// <summary>A representation of a weapon's explosion type.</summary> This indicates how the
  /// weapon behaves, such as if it deals splash damage or causes an effect to occur.
  ///
  /// @see ExplosionTypes
  /// @ingroup TypeClasses
  class ExplosionType : public Type<ExplosionType, ExplosionTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      ExplosionType(int id = ExplosionTypes::Enum::None);
  };

  /// @ingroup Types
  namespace ExplosionTypes
  {
    /// <summary>Retrieves the set of all valid ExplosionTypes.</summary>
    ///
    /// @returns Set of ExplosionTypes.
    const ExplosionType::set& allExplosionTypes();

    extern const ExplosionType None;
    extern const ExplosionType Normal;
    extern const ExplosionType Radial_Splash;
    extern const ExplosionType Enemy_Splash;
    extern const ExplosionType Lockdown;
    extern const ExplosionType Nuclear_Missile;
    extern const ExplosionType Parasite;
    extern const ExplosionType Broodlings;
    extern const ExplosionType EMP_Shockwave;
    extern const ExplosionType Irradiate;
    extern const ExplosionType Ensnare;
    extern const ExplosionType Plague;
    extern const ExplosionType Stasis_Field;
    extern const ExplosionType Dark_Swarm;
    extern const ExplosionType Consume;
    extern const ExplosionType Yamato_Gun;
    extern const ExplosionType Restoration;
    extern const ExplosionType Disruption_Web;
    extern const ExplosionType Corrosive_Acid;
    extern const ExplosionType Mind_Control;
    extern const ExplosionType Feedback;
    extern const ExplosionType Optical_Flare;
    extern const ExplosionType Maelstrom;
    extern const ExplosionType Air_Splash;
    extern const ExplosionType Unknown;
  }

  static_assert(sizeof(ExplosionType) == sizeof(int), "Expected type to resolve to primitive size.");
}
