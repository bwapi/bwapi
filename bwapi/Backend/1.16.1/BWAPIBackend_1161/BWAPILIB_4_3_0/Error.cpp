#include <string>
#include <BWAPI/Error.h>

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
  }
  const Error::set& Errors::allErrors()
  {
    return Errors::typeSet;
  }
}
