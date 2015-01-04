#include <string>
#include <BWAPI/UnitSizeType.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  template <>
  const std::string Type<UnitSizeType, UnitSizeTypes::Enum::Unknown>::typeNames[UnitSizeTypes::Enum::MAX] =
  {
    "Independent",
    "Small",
    "Medium",
    "Large",
    "None",
    "Unknown"  
  };
  namespace UnitSizeTypeSet
  {
    using namespace UnitSizeTypes::Enum;
    const UnitSizeType::set unitSizeTypeSet = { Independent, Small, Medium, Large, None, Unknown };
  }
  namespace UnitSizeTypes
  {
    BWAPI_TYPEDEF(UnitSizeType,Independent);
    BWAPI_TYPEDEF(UnitSizeType,Small);
    BWAPI_TYPEDEF(UnitSizeType,Medium);
    BWAPI_TYPEDEF(UnitSizeType,Large);
    BWAPI_TYPEDEF(UnitSizeType,None);
    BWAPI_TYPEDEF(UnitSizeType,Unknown);

  }
  UnitSizeType::UnitSizeType(int id) : Type( id )
  {}
  const UnitSizeType::set& UnitSizeTypes::allUnitSizeTypes()
  {
    return UnitSizeTypeSet::unitSizeTypeSet;
  }
}

