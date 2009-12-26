#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  namespace UnitSizeTypeIds
  {
    enum Enum
    {
      Independent = 0,
      Small = 1,
      Medium = 2,
      Large = 3,
      None = 4,
      count
    };
  }
  typedef UnitSizeTypeIds::Enum UnitSizeTypeId;
}
