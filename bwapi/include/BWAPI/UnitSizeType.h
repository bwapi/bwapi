#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class UnitSizeType
  {
    public:
      UnitSizeType();
      UnitSizeType(int id);
      UnitSizeType(const UnitSizeType& other);
      UnitSizeType& __cdecl operator=(const UnitSizeType& other);
      bool __cdecl operator==(const UnitSizeType& other) const;
      bool __cdecl operator!=(const UnitSizeType& other) const;
      bool __cdecl operator<(const UnitSizeType& other) const;
      int __cdecl getID() const;
      std::string __cdecl getName() const;
    private:
      int id;
  };
  namespace UnitSizeTypes
  {
    UnitSizeType __cdecl getUnitSizeType(std::string& name);
    std::set<UnitSizeType>& __cdecl allUnitSizeTypes();
    void __cdecl init();
    extern const UnitSizeType Independent;
    extern const UnitSizeType Small;
    extern const UnitSizeType Medium;
    extern const UnitSizeType Large;
    extern const UnitSizeType None;
    extern const UnitSizeType Unknown;
  }
}
