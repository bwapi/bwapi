#include <string>
#include <BWAPI/UnitSizeType.h>

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
    using namespace UnitSizeTypes;
    const UnitSizeType::set unitSizeTypeSet = { Independent, Small, Medium, Large, None, Unknown };
  }
  const UnitSizeType::set& UnitSizeTypes::allUnitSizeTypes()
  {
    return UnitSizeTypeSet::unitSizeTypeSet;
  }
}

