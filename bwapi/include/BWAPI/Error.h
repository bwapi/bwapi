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
    Error(int id = Errors::Enum::None);
  };

  /// @ingroup Types
  namespace Errors
  {
    /// <summary>Retrieves the set of all the error codes.</summary>
    ///
    /// @returns Set of error types.
    const Error::set& allErrors();
    
    extern const Error Unit_Does_Not_Exist;
    extern const Error Unit_Not_Visible;
    extern const Error Unit_Not_Owned;
    extern const Error Unit_Busy;
    extern const Error Incompatible_UnitType;
    extern const Error Incompatible_TechType;
    extern const Error Incompatible_State;
    extern const Error Already_Researched;
    extern const Error Fully_Upgraded;
    extern const Error Currently_Researching;
    extern const Error Currently_Upgrading;
    extern const Error Insufficient_Minerals;
    extern const Error Insufficient_Gas;
    extern const Error Insufficient_Supply;
    extern const Error Insufficient_Energy;
    extern const Error Insufficient_Tech;
    extern const Error Insufficient_Ammo;
    extern const Error Insufficient_Space;
    extern const Error Invalid_Tile_Position;
    extern const Error Unbuildable_Location;
    extern const Error Unreachable_Location;
    extern const Error Out_Of_Range;
    extern const Error Unable_To_Hit;
    extern const Error Access_Denied;
    extern const Error File_Not_Found;
    extern const Error Invalid_Parameter;
    extern const Error None;
    extern const Error Unknown;
  }

  static_assert(sizeof(Error) == sizeof(int), "Expected type to resolve to primitive size.");
}
