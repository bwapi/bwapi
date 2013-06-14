#include <string>
#include <BWAPI/DamageType.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  const std::string DamageType::typeNames[DamageTypes::Enum::MAX] = 
  {
    "Independent",
    "Explosive",
    "Concussive",
    "Normal",
    "Ignore_Armor",
    "None",
    "Unknown"
  };

  namespace DamageTypes
  {
    static const int setArr[DamageTypes::Enum::MAX]={ 
        Enum::Independent, Enum::Explosive, Enum::Concussive, Enum::Normal, Enum::Ignore_Armor, Enum::None, Enum::Unknown
    };
    static const DamageType::const_set typeSet(setArr, countof(setArr));

    BWAPI_TYPEDEF(DamageType,Independent);
    BWAPI_TYPEDEF(DamageType,Explosive);
    BWAPI_TYPEDEF(DamageType,Concussive);
    BWAPI_TYPEDEF(DamageType,Normal);
    BWAPI_TYPEDEF(DamageType,Ignore_Armor);
    BWAPI_TYPEDEF(DamageType,None);
    BWAPI_TYPEDEF(DamageType,Unknown);
  }
  DamageType::DamageType(int id) : Type( id )
  {
  }
  const DamageType::const_set& DamageTypes::allDamageTypes()
  {
    return DamageTypes::typeSet;
  }
}
