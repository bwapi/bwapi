#include <string>
#include <BWAPI/Error.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  template <>
  const std::string Type<Error, Errors::Enum::Unknown>::typeNames[Errors::Enum::MAX] =
  {
    "Unit_Does_Not_Exist",
    "Unit_Not_Visible",
    "Unit_Not_Owned",
    "Unit_Busy",
    "Incompatible_UnitType",
    "Incompatible_TechType",
    "Incompatible_State",
    "Already_Researched",
    "Fully_Upgraded",
    "Currently_Researching",
    "Currently_Upgrading",
    "Insufficient_Minerals",
    "Insufficient_Gas",
    "Insufficient_Supply",
    "Insufficient_Energy",
    "Insufficient_Tech",
    "Insufficient_Ammo",
    "Insufficient_Space",
    "Invalid_Tile_Position",
    "Unbuildable_Location",
    "Unreachable_Location",
    "Out_Of_Range",
    "Unable_To_Hit",
    "Access_Denied",
    "File_Not_Found",
    "Invalid_Parameter",
    "None",
    "Unknown"
  };

  namespace Errors
  {
      static const Error::set typeSet = {
      Enum::Unit_Does_Not_Exist,
      Enum::Unit_Not_Visible,
      Enum::Unit_Not_Owned,
      Enum::Unit_Busy,
      Enum::Incompatible_UnitType,
      Enum::Incompatible_TechType,
      Enum::Incompatible_State,
      Enum::Already_Researched,
      Enum::Fully_Upgraded,
      Enum::Currently_Researching,
      Enum::Currently_Upgrading,
      Enum::Insufficient_Minerals,
      Enum::Insufficient_Gas,
      Enum::Insufficient_Supply,
      Enum::Insufficient_Energy,
      Enum::Insufficient_Tech,
      Enum::Insufficient_Ammo,
      Enum::Insufficient_Space,
      Enum::Invalid_Tile_Position,
      Enum::Unbuildable_Location,
      Enum::Unreachable_Location,
      Enum::Out_Of_Range,
      Enum::Unable_To_Hit,
      Enum::Access_Denied,
      Enum::File_Not_Found,
      Enum::Invalid_Parameter,
      Enum::None,
      Enum::Unknown
    };

    BWAPI_TYPEDEF(Error,Unit_Does_Not_Exist);
    BWAPI_TYPEDEF(Error,Unit_Not_Visible);
    BWAPI_TYPEDEF(Error,Unit_Not_Owned);
    BWAPI_TYPEDEF(Error,Unit_Busy);
    BWAPI_TYPEDEF(Error,Incompatible_UnitType);
    BWAPI_TYPEDEF(Error,Incompatible_TechType);
    BWAPI_TYPEDEF(Error,Incompatible_State);
    BWAPI_TYPEDEF(Error,Already_Researched);
    BWAPI_TYPEDEF(Error,Fully_Upgraded);
    BWAPI_TYPEDEF(Error,Currently_Researching);
    BWAPI_TYPEDEF(Error,Currently_Upgrading);
    BWAPI_TYPEDEF(Error,Insufficient_Minerals);
    BWAPI_TYPEDEF(Error,Insufficient_Gas);
    BWAPI_TYPEDEF(Error,Insufficient_Supply);
    BWAPI_TYPEDEF(Error,Insufficient_Energy);
    BWAPI_TYPEDEF(Error,Insufficient_Tech);
    BWAPI_TYPEDEF(Error,Insufficient_Ammo);
    BWAPI_TYPEDEF(Error,Insufficient_Space);
    BWAPI_TYPEDEF(Error,Invalid_Tile_Position);
    BWAPI_TYPEDEF(Error,Unbuildable_Location);
    BWAPI_TYPEDEF(Error,Unreachable_Location);
    BWAPI_TYPEDEF(Error,Out_Of_Range);
    BWAPI_TYPEDEF(Error,Unable_To_Hit);
    BWAPI_TYPEDEF(Error,Access_Denied);
    BWAPI_TYPEDEF(Error,File_Not_Found);
    BWAPI_TYPEDEF(Error,Invalid_Parameter);
    BWAPI_TYPEDEF(Error,None);
    BWAPI_TYPEDEF(Error,Unknown);
  }
  Error::Error(int id) : Type( id ) {}
  const Error::set& Errors::allErrors()
  {
    return Errors::typeSet;
  }
}
