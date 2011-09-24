#pragma once
#include <string>
#include <set>
#include "Type.h"
namespace BWAPI
{
  class DamageType : public Type
  {
    public:
      DamageType();
      DamageType(int id);

      /** Returns the name of this damage type. For example DamageTypes::Explosive.getName() will return
       * std::string("Explosive"). */
      const std::string &getName() const;
      const char *c_str() const;
  };
  namespace DamageTypes
  {
    /** Given the name of a damage type, this will return a corresponding DamageType object. For example,
     * DamageTypes::getDamageType("Concussive") will return DamageTypes::Concussive. */
    DamageType getDamageType(std::string name);

    /** Returns the set of all the DamageTypes. */
    const std::set<DamageType>& allDamageTypes();

    void init();
    extern const DamageType Independent;
    extern const DamageType Explosive;
    extern const DamageType Concussive;
    extern const DamageType Normal;
    extern const DamageType Ignore_Armor;
    extern const DamageType None;
    extern const DamageType Unknown;
  }
}
