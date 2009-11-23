#include <string>
#include <map>
#include <set>
#include <BWAPI/AttackType.h>

namespace BWAPI
{
  bool initializingAttackType = true;
  std::string attackTypeName[2];
  std::map<std::string, AttackType> attackTypeMap;
  std::set< AttackType > attackTypeSet;
  namespace AttackTypes
  {
    const AttackType None(189);
    const AttackType Unknown(190);
    void init()
    {
      attackTypeName[None.getID()] = "None";
      attackTypeName[Unknown.getID()] = "Unknown";
      attackTypeSet.insert(None);
      attackTypeSet.insert(Unknown);
      for(std::set<AttackType>::iterator i = attackTypeSet.begin(); i != attackTypeSet.end(); i++)
      {
        attackTypeMap.insert(std::make_pair((*i).getName(), *i));
      }
      initializingAttackType = false;
    }
  }

  AttackType::AttackType()
  {
    this->id = AttackTypes::None.id;
  }
  AttackType::AttackType(int id)
  {
    this->id = id;
    if (!initializingAttackType)
    {
      if (id < 0 || id >= 2 || attackTypeName[id].length() == 0)
      {
        this->id = AttackTypes::Unknown.id;
      }
    }
  }
  AttackType::AttackType(const AttackType& other)
  {
    this->id = other.id;
  }
  AttackType& AttackType::operator=(const AttackType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool AttackType::operator==(const AttackType& other) const
  {
    return this->id == other.id;
  }
  bool AttackType::operator!=(const AttackType& other) const
  {
    return this->id != other.id;
  }
  bool AttackType::operator<(const AttackType& other) const
  {
    return this->id < other.id;
  }
  int AttackType::getID() const
  {
    return this->id;
  }
  std::string AttackType::getName() const
  {
    return attackTypeName[this->id];
  }
  AttackType AttackTypes::getAttackType(std::string& name)
  {
    std::map<std::string, AttackType>::iterator i = attackTypeMap.find(name);
    if (i == attackTypeMap.end()) return AttackTypes::Unknown;
    return (*i).second;
  }
  std::set<AttackType>& AttackTypes::allAttackTypes()
  {
    return attackTypeSet;
  }
}
