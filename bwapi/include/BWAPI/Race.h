#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  class UnitType;

  namespace Races
  {
    /// Enumeration of races
    namespace Enum
    {
      /// Enumeration of races
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

  /// The Race object is used to get information about a particular race. For example, the default
  /// worker and supply provider UnitType.
  ///
  /// As you should already know, Starcraft has three races: @Terran , @Protoss , and @Zerg .
  /// @see UnitType::getRace, PlayerInterface::getRace
  class Race : public Type<Race, Races::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      Race(int id = Races::Enum::None);

      /// Retrieves the default worker type for this Race.
      /// 
      /// @note In Starcraft, workers are the units that are used to construct structures.
      ///
      /// @returns UnitType of the worker that this race uses.
      UnitType getWorker() const;

      /// Retrieves the default resource center UnitType that is used to create expansions for
      /// this Race.
      ///
      /// @note In Starcraft, the center is the very first structure of the Race's technology
      /// tree. Also known as its base of operations or resource depot.
      /// 
      /// @returns UnitType of the center that this race uses.
      UnitType getCenter() const;

      /// Retrieves the default structure UnitType for this Race that is used to harvest gas from
      /// @Geysers .
      ///
      /// @note In Starcraft, you must first construct a structure over a @Geyser in order to
      /// begin harvesting Vespene Gas.
      /// 
      /// @returns UnitType of the structure used to harvest gas.
      UnitType getRefinery() const;

      /// Retrieves the default transport UnitType for this race that is used to transport ground 
      /// units across the map.
      ///
      /// @note In Starcraft, transports will allow you to carry ground units over unpassable
      /// terrain.
      /// 
      /// @returns UnitType for transportation.
      UnitType getTransport() const;

      /// Retrieves the default supply provider UnitType for this race that is used to  construct
      /// units.
      ///
      /// @note In Starcraft, training, morphing, or warping in units requires that the player
      /// has sufficient supply available for their Race.
      /// 
      /// @returns UnitType that provides the player with supply.
      UnitType getSupplyProvider() const;
  };
  /// Namespace containing all races
  namespace Races
  {
    /// Retrieves the set of all races.
    ///
    /// @returns Race::set containing all the Race types.
    const Race::const_set& allRaces();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const Race x
    BWAPI_DECL(Zerg);
    BWAPI_DECL(Terran);
    BWAPI_DECL(Protoss);
    BWAPI_DECL(Random);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
