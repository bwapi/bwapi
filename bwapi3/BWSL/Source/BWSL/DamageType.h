#pragma once

#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
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
    BWSL_EXPORT extern const DamageType Independent;
    BWSL_EXPORT extern const DamageType Explosive;
    BWSL_EXPORT extern const DamageType Concussive;
    BWSL_EXPORT extern const DamageType Normal;
    BWSL_EXPORT extern const DamageType Ignore_Armor;
    BWSL_EXPORT extern const DamageType None;
    BWSL_EXPORT extern const DamageType Unknown;
  }
}
