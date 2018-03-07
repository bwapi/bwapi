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
      constexpr ExplosionType(int id = ExplosionTypes::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace ExplosionTypes
  {
    /// <summary>Retrieves the set of all valid ExplosionTypes.</summary>
    ///
    /// @returns Set of ExplosionTypes.
    const ExplosionType::set& allExplosionTypes();

    constexpr ExplosionType None{Enum::None};
    constexpr ExplosionType Normal{Enum::Normal};
    constexpr ExplosionType Radial_Splash{Enum::Radial_Splash};
    constexpr ExplosionType Enemy_Splash{Enum::Enemy_Splash};
    constexpr ExplosionType Lockdown{Enum::Lockdown};
    constexpr ExplosionType Nuclear_Missile{Enum::Nuclear_Missile};
    constexpr ExplosionType Parasite{Enum::Parasite};
    constexpr ExplosionType Broodlings{Enum::Broodlings};
    constexpr ExplosionType EMP_Shockwave{Enum::EMP_Shockwave};
    constexpr ExplosionType Irradiate{Enum::Irradiate};
    constexpr ExplosionType Ensnare{Enum::Ensnare};
    constexpr ExplosionType Plague{Enum::Plague};
    constexpr ExplosionType Stasis_Field{Enum::Stasis_Field};
    constexpr ExplosionType Dark_Swarm{Enum::Dark_Swarm};
    constexpr ExplosionType Consume{Enum::Consume};
    constexpr ExplosionType Yamato_Gun{Enum::Yamato_Gun};
    constexpr ExplosionType Restoration{Enum::Restoration};
    constexpr ExplosionType Disruption_Web{Enum::Disruption_Web};
    constexpr ExplosionType Corrosive_Acid{Enum::Corrosive_Acid};
    constexpr ExplosionType Mind_Control{Enum::Mind_Control};
    constexpr ExplosionType Feedback{Enum::Feedback};
    constexpr ExplosionType Optical_Flare{Enum::Optical_Flare};
    constexpr ExplosionType Maelstrom{Enum::Maelstrom};
    constexpr ExplosionType Air_Splash{Enum::Air_Splash};
    constexpr ExplosionType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(ExplosionType) == sizeof(int), "Expected type to resolve to primitive size.");
}
