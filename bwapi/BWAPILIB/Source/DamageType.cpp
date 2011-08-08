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

#define SET_DAMAGE_TYPE(x) damageTypeName[x] = underscoresToSpaces(#x); damageTypeSet.insert(x);
    void init()
    {
      SET_DAMAGE_TYPE(Independent);
      SET_DAMAGE_TYPE(Explosive);
      SET_DAMAGE_TYPE(Concussive);
      SET_DAMAGE_TYPE(Normal);
      SET_DAMAGE_TYPE(Ignore_Armor);
      SET_DAMAGE_TYPE(None);
      SET_DAMAGE_TYPE(Unknown);

      foreach(DamageType i, damageTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        damageTypeMap.insert(std::make_pair(name, i));
      }
      initializingDamageType = false;
    }
  }
  DamageType::DamageType()
  {
    this->id = DamageTypes::None;
  }
  DamageType::DamageType(int id)
  {
    this->id = id;
    if (!initializingDamageType && (id < 0 || id >= 7))
      this->id = DamageTypes::Unknown;
  }
  DamageType::DamageType(const DamageType& other)
  {
    this->id = other;
  }
  DamageType& DamageType::operator=(const DamageType& other)
  {
    this->id = other;
    return *this;
  }
  DamageType::operator int() const
  {
    return id;
  }
  int DamageType::getID() const
  {
    return this->id;
  }
  std::string DamageType::getName() const
  {
    return damageTypeName[this->id];
  }
  const char *DamageType::c_str() const
  {
    return damageTypeName[this->id].c_str();
  }
  DamageType DamageTypes::getDamageType(std::string name)
  {
    fixName(&name);
    std::map<std::string, DamageType>::iterator i = damageTypeMap.find(name);
    if (i == damageTypeMap.end())
      return DamageTypes::Unknown;
    return (*i).second;
  }
  std::set<DamageType>& DamageTypes::allDamageTypes()
  {
    return damageTypeSet;
  }
}
