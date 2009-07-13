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
    DamageType(const DamageType &other);
    DamageType& operator=(const DamageType &other);
    bool operator==(const DamageType &other) const;
    bool operator!=(const DamageType &other) const;
    bool operator<(const DamageType &other) const;
    int getID() const;
    std::string getName() const;
  private:
    int id;
  };
  namespace DamageTypes
  {
    DamageType getDamageType(std::string &name);
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
