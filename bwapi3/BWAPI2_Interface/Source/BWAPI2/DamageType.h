#pragma once

#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class DamageType
  {
    public:
      DamageType();
      DamageType(int id);
      DamageType(const DamageType& other);
      DamageType& operator=(const DamageType& other);
      bool operator==(const DamageType& other) const;
      bool operator!=(const DamageType& other) const;
      bool operator<(const DamageType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace DamageTypes
  {
    DamageType getDamageType(std::string& name);
    std::set<DamageType>& allDamageTypes();
    void init();
    BWAPI2_EXPORT extern const DamageType Independent;
    BWAPI2_EXPORT extern const DamageType Explosive;
    BWAPI2_EXPORT extern const DamageType Concussive;
    BWAPI2_EXPORT extern const DamageType Normal;
    BWAPI2_EXPORT extern const DamageType Ignore_Armor;
    BWAPI2_EXPORT extern const DamageType None;
    BWAPI2_EXPORT extern const DamageType Unknown;
  }
}
