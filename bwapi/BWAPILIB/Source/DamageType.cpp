#include <string>
#include <map>
#include <set>
#include <BWAPI/DamageType.h>

namespace BWAPI
{
  bool initializingDamageType=true;
  std::string damageTypeName[5];
  std::map<std::string, DamageType> damageTypeMap;
  std::set< DamageType > damageTypeSet;
  namespace DamageTypes
  {
    const DamageType Concussive(0);
    const DamageType Normal(1);
    const DamageType Explosive(2);
    const DamageType None(3);
    const DamageType Unknown(4);
    void init()
    {
      damageTypeName[Concussive.getID()]="Concussive";
      damageTypeName[Normal.getID()]="Normal";
      damageTypeName[Explosive.getID()]="Explosive";
      damageTypeName[None.getID()]="None";
      damageTypeName[Unknown.getID()]="Unknown";

      damageTypeSet.insert(Concussive);
      damageTypeSet.insert(Normal);
      damageTypeSet.insert(Explosive);
      damageTypeSet.insert(None);
      damageTypeSet.insert(Unknown);

      for(std::set<DamageType>::iterator i=damageTypeSet.begin();i!=damageTypeSet.end();i++)
      {
        damageTypeMap.insert(std::make_pair((*i).getName(),*i));
      }
      initializingDamageType=false;
    }
  }
  DamageType::DamageType()
  {
    this->id=DamageTypes::None.id;
  }
  DamageType::DamageType(int id)
  {
    this->id=id;
    if (!initializingDamageType)
    {
      if (id<0 || id>=7)
      {
        this->id=DamageTypes::Unknown.id;
      }
    }
  }
  DamageType::DamageType(const DamageType &other)
  {
    this->id=other.id;
  }
  DamageType& DamageType::operator=(const DamageType &other)
  {
    this->id=other.id;
    return *this;
  }
  bool DamageType::operator==(const DamageType &other) const
  {
    return this->id==other.id;
  }
  bool DamageType::operator!=(const DamageType &other) const
  {
    return this->id!=other.id;
  }
  bool DamageType::operator<(const DamageType &other) const
  {
    return this->id<other.id;
  }
  int DamageType::getID() const
  {
    return this->id;
  }
  std::string DamageType::getName() const
  {
    return damageTypeName[this->id];
  }
  
  DamageType DamageTypes::getDamageType(std::string &name)
  {
    std::map<std::string, DamageType>::iterator i=damageTypeMap.find(name);
    if (i==damageTypeMap.end()) return DamageTypes::Unknown;
    return (*i).second;
  }
  std::set<DamageType>& DamageTypes::allDamageTypes()
  {
    return damageTypeSet;
  }
}
