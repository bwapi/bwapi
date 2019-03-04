#include <string>
#include <BWAPI/DamageType.h>

namespace BWAPI
{
  template <>
  const std::string Type<DamageType, DamageTypes::Enum::Unknown>::typeNames[DamageTypes::Enum::MAX] =
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
    static const DamageType::set typeSet = {
      Independent, Explosive, Concussive, Normal, Ignore_Armor, None, Unknown
    };
  }
  const DamageType::set& DamageTypes::allDamageTypes()
  {
    return DamageTypes::typeSet;
  }
}
