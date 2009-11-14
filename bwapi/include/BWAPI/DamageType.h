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
      DamageType& __cdecl operator=(const DamageType& other);
      bool __cdecl operator==(const DamageType& other) const;
      bool __cdecl operator!=(const DamageType& other) const;
      bool __cdecl operator<(const DamageType& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
    private:
      int id;
  };
  namespace DamageTypes
  {
    DamageType __cdecl getDamageType(std::string& name);
    std::set<DamageType>& __cdecl allDamageTypes();
    void __cdecl init();
    extern const DamageType Independent;
    extern const DamageType Explosive;
    extern const DamageType Concussive;
    extern const DamageType Normal;
    extern const DamageType Ignore_Armor;
    extern const DamageType None;
    extern const DamageType Unknown;
  }
}
