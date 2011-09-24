#include <string>
#include <map>
#include <set>
#include <BWAPI/DamageType.h>

#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingDamageType = true;
  std::string damageTypeName[7];
  std::map<std::string, DamageType> damageTypeMap;
  std::set< DamageType > damageTypeSet;
  namespace DamageTypes
  {
    const DamageType Independent(0);
    const DamageType Explosive(1);
    const DamageType Concussive(2);
    const DamageType Normal(3);
    const DamageType Ignore_Armor(4);
    const DamageType None(5);
    const DamageType Unknown(6);

    void init()
    {
      damageTypeName[Independent]  = "Independent";
      damageTypeName[Explosive]    = "Explosive";
      damageTypeName[Concussive]   = "Concussive";
      damageTypeName[Normal]       = "Normal";
      damageTypeName[Ignore_Armor] = "Ignore Armor";
      damageTypeName[None]         = "None";
      damageTypeName[Unknown]      = "Unknown";

      damageTypeSet.insert(Independent);
      damageTypeSet.insert(Explosive);
      damageTypeSet.insert(Concussive);
      damageTypeSet.insert(Normal);
      damageTypeSet.insert(Ignore_Armor);
      damageTypeSet.insert(None);
      damageTypeSet.insert(Unknown);

      foreach(DamageType i, damageTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        damageTypeMap.insert(std::make_pair(name, i));
      }
      initializingDamageType = false;
    }
  }
  DamageType::DamageType() : Type(DamageTypes::None)
  {
  }
  int getValidDamageTypeID(int id)
  {
    if ( !initializingDamageType && (id < 0 || id >= 7) ) 
      return DamageTypes::Unknown;
    return id;
  }
  DamageType::DamageType(int id) : Type( getValidDamageTypeID(id) )
  {
  }
  const std::string &DamageType::getName() const
  {
    return damageTypeName[this->getID()];
  }
  const char *DamageType::c_str() const
  {
    return damageTypeName[this->getID()].c_str();
  }
  DamageType DamageTypes::getDamageType(std::string name)
  {
    fixName(&name);
    std::map<std::string, DamageType>::iterator i = damageTypeMap.find(name);
    if (i == damageTypeMap.end())
      return DamageTypes::Unknown;
    return (*i).second;
  }
  const std::set<DamageType>& DamageTypes::allDamageTypes()
  {
    return damageTypeSet;
  }
}
