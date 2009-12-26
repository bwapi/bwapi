#include "DamageTypes.h"

namespace BWAPI
{
  namespace DamageTypes
  {
    DamageType damageTypeData[DamageTypeIds::count];
    std::map<std::string, DamageTypeId> damageTypeMap;
    std::set< DamageTypeId > damageTypeSet;

    void fillDamageType(DamageTypeId id, const char* name)
    {
      DamageType &target = damageTypeData[id];
      target.name = name;
    }

    void init()
    {
      fillDamageType(DamageTypeIds::Independent, "Independent");
      fillDamageType(DamageTypeIds::Explosive, "Explosive");
      fillDamageType(DamageTypeIds::Concussive, "Concussive");
      fillDamageType(DamageTypeIds::Normal, "Normal");
      fillDamageType(DamageTypeIds::Ignore_Armor, "Ignore Armor");
      fillDamageType(DamageTypeIds::None, "None");

      damageTypeSet.insert(DamageTypeIds::Independent);
      damageTypeSet.insert(DamageTypeIds::Explosive);
      damageTypeSet.insert(DamageTypeIds::Concussive);
      damageTypeSet.insert(DamageTypeIds::Normal);
      damageTypeSet.insert(DamageTypeIds::Ignore_Armor);
      damageTypeSet.insert(DamageTypeIds::None);

      for(std::set<DamageTypeId>::iterator i = damageTypeSet.begin(); i != damageTypeSet.end(); i++)
      {
        damageTypeMap.insert(std::make_pair(std::string(damageTypeData[*i].name), *i));
      }
    }
  }

  namespace DamageTypes
  {
    DamageTypeId getIdByName(const std::string& name)
    {
      std::map<std::string, DamageTypeId>::iterator i = damageTypeMap.find(name);
      if (i == damageTypeMap.end()) return DamageTypeIds::None;
      return (*i).second;
    }
  }
}
