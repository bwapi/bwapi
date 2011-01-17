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
      damageTypeName[Independent.getID()]  = "Independent";
      damageTypeName[Explosive.getID()]    = "Explosive";
      damageTypeName[Concussive.getID()]   = "Concussive";
      damageTypeName[Normal.getID()]       = "Normal";
      damageTypeName[Ignore_Armor.getID()] = "Ignore Armor";
      damageTypeName[None.getID()]         = "None";
      damageTypeName[Unknown.getID()]      = "Unknown";

      damageTypeSet.insert(Independent);
      damageTypeSet.insert(Explosive);
      damageTypeSet.insert(Concussive);
      damageTypeSet.insert(Normal);
      damageTypeSet.insert(Ignore_Armor);
      damageTypeSet.insert(None);
      damageTypeSet.insert(Unknown);

      foreach(DamageType i, damageTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        damageTypeMap.insert(std::make_pair(name, i));
      }
      initializingDamageType = false;
    }
  }
  DamageType::DamageType()
  {
    this->id = DamageTypes::None.id;
  }
  DamageType::DamageType(int id)
  {
    this->id = id;
    if (!initializingDamageType && (id < 0 || id >= 7))
      this->id = DamageTypes::Unknown.id;
  }
  DamageType::DamageType(const DamageType& other)
  {
    this->id = other.id;
  }
  DamageType& DamageType::operator=(const DamageType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool DamageType::operator==(const DamageType& other) const
  {
    return this->id == other.id;
  }
  bool DamageType::operator!=(const DamageType& other) const
  {
    return this->id != other.id;
  }
  bool DamageType::operator<(const DamageType& other) const
  {
    return this->id < other.id;
  }
  int DamageType::getID() const
  {
    return this->id;
  }
  std::string DamageType::getName() const
  {
    return damageTypeName[this->id];
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
