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
      Unknown
    };
  }
  const Error::set& Errors::allErrors()
  {
    return Errors::typeSet;
  }
}
