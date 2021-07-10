#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class UnitType;

  /// <summary>Namespace containing all valid races.</summary>
  /// @see Race
  namespace Races
  {
    /// <summary>Enumeration of races.</summary>
    /// @see Race
    namespace Enum
    {
      /// <summary>Enumeration of races.</summary>
      /// @see Race
      enum Enum
      {
        Zerg = 0,
        Terran,
        Protoss,
        Other,
        Unused,
        Select,
        Random,
        None,
        Unknown,
        MAX
      };
    }
  }

  /// <summary>The Race object is used to get information about a particular race.</summary> For
  /// example, the default worker and supply provider UnitType.
  ///
  /// As you should already know, Starcraft has three races: @Terran , @Protoss , and @Zerg .
  /// @see UnitType::getRace, Player::getRace, Races
  /// @ingroup TypeClasses
  class Race : public Type<Race, Races::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr explicit Race(int id = Races::Enum::None) : Type(id) {}

      /// <summary>Retrieves the default worker type for this Race.</summary>
      /// 
      /// @note In Starcraft, workers are the units that are used to construct structures.
      ///
      /// @returns UnitType of the worker that this race uses.
      UnitType getWorker() const;

      /// <summary>Retrieves the default resource depot UnitType that workers of this race can
      /// construct and return resources to.</summary>
      ///
      /// @note In Starcraft, the center is the very first structure of the Race's technology
      /// tree. Also known as its base of operations or resource depot.
      /// 
      /// @returns UnitType of the center that this race uses.
      ///
      /// @m_since{4,2,0}
      UnitType getResourceDepot() const;

      /// <summary>Deprecated. Use getResourceDepot instead.</summary>
      /// @deprecated As of 4.2.0 due to naming inconsistency. Use #getResourceDepot instead.
      /// See https://github.com/bwapi/bwapi/issues/621 for more information.
#ifndef SWIG // Deprecated tags not supported https://github.com/swig/swig/issues/490
      [[deprecated("getCenter() was renamed to getResourceDepot()")]]
#endif
      UnitType getCenter() const;

      /// <summary>Retrieves the default structure UnitType for this Race that is used to harvest gas from
      /// @Geysers.</summary>
      ///
      /// @note In Starcraft, you must first construct a structure over a @Geyser in order to
      /// begin harvesting Vespene Gas.
      /// 
      /// @returns UnitType of the structure used to harvest gas.
      UnitType getRefinery() const;

      /// <summary>Retrieves the default transport UnitType for this race that is used to transport ground 
      /// units across the map.</summary>
      ///
      /// @note In Starcraft, transports will allow you to carry ground units over unpassable
      /// terrain.
      /// 
      /// @returns UnitType for transportation.
      UnitType getTransport() const;

      /// <summary>Retrieves the default supply provider UnitType for this race that is used to  construct
      /// units.</summary>
      ///
      /// @note In Starcraft, training, morphing, or warping in units requires that the player
      /// has sufficient supply available for their Race.
      /// 
      /// @returns UnitType that provides the player with supply.
      UnitType getSupplyProvider() const;
  };

  /// @ingroup Types
  namespace Races
  {
    /// <summary>Retrieves the set of all valid races.</summary>
    ///
    /// @returns Race::set containing all the Race types.
    const Race::set& allRaces();

    constexpr Race Zerg{Enum::Zerg};
    constexpr Race Terran{Enum::Terran};
    constexpr Race Protoss{Enum::Protoss};
    constexpr Race Random{Enum::Random};
    constexpr Race None{Enum::None};
    constexpr Race Unknown{Enum::Unknown};
  }

  static_assert(sizeof(Race) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::Race> {
    auto operator()(BWAPI::Race id) const {
      return BWAPI::Race::Hash{}(id);
    }
  };
} // namespace std
