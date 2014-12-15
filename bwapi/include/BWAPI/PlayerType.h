#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace PlayerTypes
  {
    /// Enumeration of player types (player controllers)
    namespace Enum
    {
      /// Enumeration of player types (player controllers)
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
  class PlayerType : public Type<PlayerType, PlayerTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      PlayerType(int id = PlayerTypes::Enum::None);

      bool isLobbyType() const;
      bool isGameType() const;
  };
  /// Namespace containing player types (player controllers)
  namespace PlayerTypes
  {
    /// Retrieves the set of all the PlayerTypes.
    ///
    /// @returns Set consisting of all valid PlayerTypes.
    const PlayerType::set& allPlayerTypes();
    
    extern const PlayerType None;
    extern const PlayerType Computer;
    extern const PlayerType Player;
    extern const PlayerType RescuePassive;
    extern const PlayerType EitherPreferComputer;
    extern const PlayerType EitherPreferHuman;
    extern const PlayerType Neutral;
    extern const PlayerType Closed;
    extern const PlayerType PlayerLeft;
    extern const PlayerType ComputerLeft;
    extern const PlayerType Unknown;
  }

  static_assert(sizeof(PlayerType) == sizeof(int), "Expected type to resolve to primitive size.");
}
