#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  class UnitType;
  class Error
  {
    public:
      Error();
      Error(int id);
      Error(const Error& other);
      Error& __cdecl operator=(const Error& other);
      bool __cdecl operator==(const Error& other) const;
      bool __cdecl operator!=(const Error& other) const;
      bool __cdecl operator<(const Error& other) const;
      int __cdecl getID() const;
      std::string __cdecl toString() const;
    private:
      int id;
  };
  namespace Errors
  {
    Error __cdecl getError(std::string& name);
    std::set<Error>& __cdecl allErrors();
    void __cdecl init();
    extern const Error Unit_Does_Not_Exist;
    extern const Error Unit_Not_Visible;
    extern const Error Unit_Not_Owned;
    extern const Error Unit_Busy;
    extern const Error Incompatible_UnitType;
    extern const Error Incompatible_TechType;
    extern const Error Already_Researched;
    extern const Error Fully_Upgraded;
    extern const Error Insufficient_Minerals;
    extern const Error Insufficient_Gas;
    extern const Error Insufficient_Supply;
    extern const Error Insufficient_Energy;
    extern const Error Insufficient_Tech;
    extern const Error Insufficient_Ammo;
    extern const Error Insufficient_Space;
    extern const Error Unbuildable_Location;
    extern const Error Out_Of_Range;
    extern const Error Unable_To_Hit;
    extern const Error Access_Denied;
    extern const Error None;
    extern const Error Unknown;
  }
}
