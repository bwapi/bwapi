#pragma once
#include <BWAPI/Type.h>
#include <string>

namespace BWAPI
{
  class UnitType;

  namespace Errors
  {
    /// Enumeration of Error types
    namespace Enum
    {
      /// Enumeration of Error types
      enum Enum
      {
        Unit_Does_Not_Exist,
        Unit_Not_Visible,
        Unit_Not_Owned,
        Unit_Busy,
        Incompatible_UnitType,
        Incompatible_TechType,
        Incompatible_State,
        Already_Researched,
        Fully_Upgraded,
        Currently_Researching,
        Currently_Upgrading,
        Insufficient_Minerals,
        Insufficient_Gas,
        Insufficient_Supply,
        Insufficient_Energy,
        Insufficient_Tech,
        Insufficient_Ammo,
        Insufficient_Space,
        Invalid_Tile_Position,
        Unbuildable_Location,
        Unreachable_Location,
        Out_Of_Range,
        Unable_To_Hit,
        Access_Denied,
        File_Not_Found,
        Invalid_Parameter,
        None,
        Unknown,
        MAX
      };
    };
  };
  
  /// The Error object is generally used to determine why certain functions in BWAPI have failed.
  ///
  /// For example, you may not have enough resources to construct a unit.
  /// @see Game::getLastError, Game::setLastError
  class Error : public Type<Error, Errors::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    Error(int id = Errors::Enum::None);
  };

  /// Namespace containing error codes
  namespace Errors
  {
    /// Retrieves the set of all the error codes.
    ///
    /// @returns Set of error types.
    const Error::const_set& allErrors();

#ifdef BWAPI_DECL
#undef BWAPI_DECL
#endif
#define BWAPI_DECL(x) /** x */ extern const Error x
    BWAPI_DECL(Unit_Does_Not_Exist);
    BWAPI_DECL(Unit_Not_Visible);
    BWAPI_DECL(Unit_Not_Owned);
    BWAPI_DECL(Unit_Busy);
    BWAPI_DECL(Incompatible_UnitType);
    BWAPI_DECL(Incompatible_TechType);
    BWAPI_DECL(Incompatible_State);
    BWAPI_DECL(Already_Researched);
    BWAPI_DECL(Fully_Upgraded);
    BWAPI_DECL(Currently_Researching);
    BWAPI_DECL(Currently_Upgrading);
    BWAPI_DECL(Insufficient_Minerals);
    BWAPI_DECL(Insufficient_Gas);
    BWAPI_DECL(Insufficient_Supply);
    BWAPI_DECL(Insufficient_Energy);
    BWAPI_DECL(Insufficient_Tech);
    BWAPI_DECL(Insufficient_Ammo);
    BWAPI_DECL(Insufficient_Space);
    BWAPI_DECL(Invalid_Tile_Position);
    BWAPI_DECL(Unbuildable_Location);
    BWAPI_DECL(Unreachable_Location);
    BWAPI_DECL(Out_Of_Range);
    BWAPI_DECL(Unable_To_Hit);
    BWAPI_DECL(Access_Denied);
    BWAPI_DECL(File_Not_Found);
    BWAPI_DECL(Invalid_Parameter);
    BWAPI_DECL(None);
    BWAPI_DECL(Unknown);
#undef BWAPI_DECL
  }
}
