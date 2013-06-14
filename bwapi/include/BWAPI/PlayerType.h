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
    const PlayerType::const_set& allPlayerTypes();
    
#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const PlayerType x
    BWAPI_DECL(None);
    BWAPI_DECL(Computer);
    BWAPI_DECL(Player);
    BWAPI_DECL(RescuePassive);
    BWAPI_DECL(EitherPreferComputer);
    BWAPI_DECL(EitherPreferHuman);
    BWAPI_DECL(Neutral);
    BWAPI_DECL(Closed);
    BWAPI_DECL(PlayerLeft);
    BWAPI_DECL(ComputerLeft);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
