#pragma once
#include <BWAPI/Type.h>
#include <string>

namespace BWAPI
{
  class UnitType;

  /// <summary>Namespace containing error codes</summary>
  /// @see Error
  namespace Errors
  {
    /// <summary>Enumeration of Error types.</summary>
    /// @see Error
    namespace Enum
    {
      /// <summary>Enumeration of Error types.</summary>
      /// @see Error
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
  
  /// <summary>The Error object is generally used to determine why certain functions in BWAPI
  /// have failed.</summary>
  ///
  /// For example, you may not have enough resources to construct a unit.
  /// @see Game::getLastError, Game::setLastError, Errors
  /// @ingroup TypeClasses
  class Error : public Type<Error, Errors::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    constexpr Error(int id = Errors::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace Errors
  {
    /// <summary>Retrieves the set of all the error codes.</summary>
    ///
    /// @returns Set of error types.
    const Error::set& allErrors();
    
    constexpr Error Unit_Does_Not_Exist{Enum::Unit_Does_Not_Exist};
    constexpr Error Unit_Not_Visible{Enum::Unit_Not_Visible};
    constexpr Error Unit_Not_Owned{Enum::Unit_Not_Owned};
    constexpr Error Unit_Busy{Enum::Unit_Busy};
    constexpr Error Incompatible_UnitType{Enum::Incompatible_UnitType};
    constexpr Error Incompatible_TechType{Enum::Incompatible_TechType};
    constexpr Error Incompatible_State{Enum::Incompatible_State};
    constexpr Error Already_Researched{Enum::Already_Researched};
    constexpr Error Fully_Upgraded{Enum::Fully_Upgraded};
    constexpr Error Currently_Researching{Enum::Currently_Researching};
    constexpr Error Currently_Upgrading{Enum::Currently_Upgrading};
    constexpr Error Insufficient_Minerals{Enum::Insufficient_Minerals};
    constexpr Error Insufficient_Gas{Enum::Insufficient_Gas};
    constexpr Error Insufficient_Supply{Enum::Insufficient_Supply};
    constexpr Error Insufficient_Energy{Enum::Insufficient_Energy};
    constexpr Error Insufficient_Tech{Enum::Insufficient_Tech};
    constexpr Error Insufficient_Ammo{Enum::Insufficient_Ammo};
    constexpr Error Insufficient_Space{Enum::Insufficient_Space};
    constexpr Error Invalid_Tile_Position{Enum::Invalid_Tile_Position};
    constexpr Error Unbuildable_Location{Enum::Unbuildable_Location};
    constexpr Error Unreachable_Location{Enum::Unreachable_Location};
    constexpr Error Out_Of_Range{Enum::Out_Of_Range};
    constexpr Error Unable_To_Hit{Enum::Unable_To_Hit};
    constexpr Error Access_Denied{Enum::Access_Denied};
    constexpr Error File_Not_Found{Enum::File_Not_Found};
    constexpr Error Invalid_Parameter{Enum::Invalid_Parameter};
    constexpr Error None{Enum::None};
    constexpr Error Unknown{Enum::Unknown};
  }

  static_assert(sizeof(Error) == sizeof(int), "Expected type to resolve to primitive size.");
}
