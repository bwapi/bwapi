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
    };
  }

  /// <summary>The Race object is used to get information about a particular race.</summary> For
  /// example, the default worker and supply provider UnitType.
  ///
  /// As you should already know, Starcraft has three races: @Terran , @Protoss , and @Zerg .
  /// @see UnitType::getRace, PlayerInterface::getRace, Races
  /// @ingroup TypeClasses
  class Race : public Type<Race, Races::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      Race(int id = Races::Enum::None);

      /// <summary>Retrieves the default worker type for this Race.</summary>
      /// 
      /// @note In Starcraft, workers are the units that are used to construct structures.
      ///
      /// @returns UnitType of the worker that this race uses.
      UnitType getWorker() const;

      /// <summary>Retrieves the default resource center UnitType that is used to create expansions for
      /// this Race.</summary>
      ///
      /// @note In Starcraft, the center is the very first structure of the Race's technology
      /// tree. Also known as its base of operations or resource depot.
      /// 
      /// @returns UnitType of the center that this race uses.
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

    extern const Race Zerg;
    extern const Race Terran;
    extern const Race Protoss;
    extern const Race Random;
    extern const Race None;
    extern const Race Unknown;
  }

  static_assert(sizeof(Race) == sizeof(int), "Expected type to resolve to primitive size.");
}
