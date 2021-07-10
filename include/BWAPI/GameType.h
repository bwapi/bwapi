#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing game types.</summary>
  /// @see GameType
  namespace GameTypes
  {
    /// <summary>Enumeration of game types.</summary>
    /// @see GameType
    namespace Enum
    {
      /// <summary>Enumeration of game types.</summary>
      /// @see GameType
      enum Enum
      {
        None = 0,
        Custom,          // Warcraft III
        Melee,
        Free_For_All,
        One_on_One,
        Capture_The_Flag,
        Greed,
        Slaughter,
        Sudden_Death,
        Ladder,
        Use_Map_Settings,
        Team_Melee,
        Team_Free_For_All,
        Team_Capture_The_Flag,
        Unknown_0x0E,
        Top_vs_Bottom,
        Iron_Man_Ladder,    // Warcraft II

        Pro_Gamer_League = 32,  // Not valid
        Unknown,
        MAX
      };
    }
  }
  /// <summary>A class that represents game types in Broodwar.</summary> A game type is selected
  /// when creating a game.
  ///
  /// @see GameTypes
  /// @ingroup TypeClasses
  class GameType : public Type<GameType, GameTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr explicit GameType(int id = GameTypes::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace GameTypes
  {
    /// <summary>Retrieves the set of all the valid GameTypes.</summary>
    ///
    /// @returns Set of available GameTypes.
    const GameType::set& allGameTypes();
    
    constexpr GameType Melee{Enum::Melee};
    constexpr GameType Free_For_All{Enum::Free_For_All};
    constexpr GameType One_on_One{Enum::One_on_One};
    constexpr GameType Capture_The_Flag{Enum::Capture_The_Flag};
    constexpr GameType Greed{Enum::Greed};
    constexpr GameType Slaughter{Enum::Slaughter};
    constexpr GameType Sudden_Death{Enum::Sudden_Death};
    constexpr GameType Ladder{Enum::Ladder};
    constexpr GameType Use_Map_Settings{Enum::Use_Map_Settings};
    constexpr GameType Team_Melee{Enum::Team_Melee};
    constexpr GameType Team_Free_For_All{Enum::Team_Free_For_All};
    constexpr GameType Team_Capture_The_Flag{Enum::Team_Capture_The_Flag};
    constexpr GameType Top_vs_Bottom{Enum::Top_vs_Bottom};
    constexpr GameType None{Enum::None};
    constexpr GameType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(GameType) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::GameType> {
    auto operator()(BWAPI::GameType id) const {
      return BWAPI::GameType::Hash{}(id);
    }
  };
} // namespace std
