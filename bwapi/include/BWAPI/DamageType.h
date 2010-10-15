#pragma once
#include <string>
#include <set>
namespace BWAPI
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

      /** Returns a unique ID for this damage type. */
      int getID() const;

      /** Returns the name of this damage type. For example DamageTypes::Explosive.getName() will return
       * std::string("Explosive"). */
      std::string getName() const;
    private:
      int id;
  };
  namespace DamageTypes
  {
    /** Given the name of a damage type, this will return a corresponding DamageType object. For example,
     * DamageTypes::getDamageType("Concussive") will return DamageTypes::Concussive. */
    DamageType getDamageType(std::string name);

    /** Returns the set of all the DamageTypes. */
    std::set<DamageType>& allDamageTypes();

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
