#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing player types (player controllers).</summary>
  /// @see PlayerType
  namespace PlayerTypes
  {
    /// <summary>Enumeration of player types (player controllers).</summary>
    /// @see PlayerType
    namespace Enum
    {
      /// <summary>Enumeration of player types (player controllers).</summary>
      /// @see PlayerType
      enum Enum
      {
        None = 0,
        Computer,
        Player,
        RescuePassive,
        RescueActive,
        EitherPreferComputer,
        EitherPreferHuman,
        Neutral,
        Closed,
        Observer,
        PlayerLeft,
        ComputerLeft,
        Unknown,
        MAX
      };
    };
  };
  /// <summary>Represents the type of controller for the player slot (i.e. human, computer).</summary>
  /// 
  /// @see PlayerTypes
  /// @ingroup TypeClasses
  class PlayerType : public Type<PlayerType, PlayerTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr PlayerType(int id = PlayerTypes::Enum::None) : Type(id) {}

      /// <summary>Identifies whether or not this type is used for the pre-game lobby.</summary>
      /// A type such as PlayerTypes::ComputerLeft would only appear in-game when a computer
      /// player is defeated.
      ///
      /// @returns true if this type can appear in the pre-game lobby, false otherwise.
      bool isLobbyType() const;

      /// <summary>Identifies whether or not this type is used in-game.</summary> A type such as
      /// PlayerTypes::Closed would not be a valid in-game type.
      ///
      /// @returns true if the type can appear in-game, false otherwise.
      /// @see isLobbyType
      bool isGameType() const;
  };

  /// @ingroup Types
  namespace PlayerTypes
  {
    /// <summary>Retrieves the set of all the PlayerTypes.</summary>
    ///
    /// @returns Set consisting of all valid PlayerTypes.
    const PlayerType::set& allPlayerTypes();
    
    constexpr PlayerType None{Enum::None};
    constexpr PlayerType Computer{Enum::Computer};
    constexpr PlayerType Player{Enum::Player};
    constexpr PlayerType RescuePassive{Enum::RescuePassive};
    constexpr PlayerType EitherPreferComputer{Enum::EitherPreferComputer};
    constexpr PlayerType EitherPreferHuman{Enum::EitherPreferHuman};
    constexpr PlayerType Neutral{Enum::Neutral};
    constexpr PlayerType Closed{Enum::Closed};
    constexpr PlayerType PlayerLeft{Enum::PlayerLeft};
    constexpr PlayerType ComputerLeft{Enum::ComputerLeft};
    constexpr PlayerType Unknown{Enum::Unknown};
  }

  static_assert(sizeof(PlayerType) == sizeof(int), "Expected type to resolve to primitive size.");
}
