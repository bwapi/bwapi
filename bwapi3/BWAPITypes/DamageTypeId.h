#pragma once

namespace BWAPI
{
  namespace DamageTypeIds
  {
    enum Enum
    {
      Independent = 0,
      Explosive = 1,
      Concussive = 2,
      Normal = 3,
      Ignore_Armor = 4,
      None = 5,
      count
    };
  }
  typedef DamageTypeIds::Enum DamageTypeId;
}
