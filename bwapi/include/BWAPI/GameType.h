#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace GameTypes
  {
    /// Enumeration of game types
    namespace Enum
    {
      /// Enumeration of game types
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
    };
  };
  class GameType : public Type<GameType, GameTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      GameType(int id = GameTypes::Enum::None);
  };
  /// Namespace containing game types
  namespace GameTypes
  {
    /// Retrieves the set of all the valid GameTypes.
    ///
    /// @returns Set of available GameTypes.
    const GameType::const_set& allGameTypes();
    
#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const GameType x
    BWAPI_DECL(Melee);
    BWAPI_DECL(Free_For_All);
    BWAPI_DECL(One_on_One);
    BWAPI_DECL(Capture_The_Flag);
    BWAPI_DECL(Greed);
    BWAPI_DECL(Slaughter);
    BWAPI_DECL(Sudden_Death);
    BWAPI_DECL(Ladder);
    BWAPI_DECL(Use_Map_Settings);
    BWAPI_DECL(Team_Melee);
    BWAPI_DECL(Team_Free_For_All);
    BWAPI_DECL(Team_Capture_The_Flag);
    BWAPI_DECL(Top_vs_Bottom);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
