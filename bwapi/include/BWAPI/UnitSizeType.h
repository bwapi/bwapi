#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing unit size types.</summary>
  ///
  /// @see UnitSizeType
  ///
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  namespace UnitSizeTypes
  {
    /// <summary>Enumeration of unit size types.</summary>
    /// @see UnitSizeType
    namespace Enum
    {
      /// <summary>Enumeration of unit size types.</summary>
      /// @see UnitSizeType
      enum Enum
      {
        Independent = 0,
        Small,
        Medium,
        Large,
        None,
        Unknown,
        MAX
      };
    };
  };

  /// <summary>Size types are used by unit types in Broodwar to determine how much damage will be
  /// applied.</summary> This corresponds with DamageType for several different damage reduction
  /// applications.
  ///
  /// @see DamageType, UnitType, UnitSizeTypes
  ///
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  /// @ingroup TypeClasses
  class UnitSizeType : public Type<UnitSizeType, UnitSizeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    constexpr UnitSizeType(int id = UnitSizeTypes::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace UnitSizeTypes
  {
    /// <summary>Retrieves the set of all valid UnitSizeTypes.</summary>
    ///
    /// @returns Set of all UnitSizeTypes.
    const UnitSizeType::set& allUnitSizeTypes();
    
    constexpr UnitSizeType Independent{Enum::Independent};
    constexpr UnitSizeType Small{Enum::Small};
    constexpr UnitSizeType Medium{Enum::Medium};
    constexpr UnitSizeType Large{Enum::Large};
    constexpr UnitSizeType None{Enum::None};
    constexpr UnitSizeType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(UnitSizeType) == sizeof(int), "Expected type to resolve to primitive size.");
}
