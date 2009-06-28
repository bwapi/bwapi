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
    Error(const Error &other);
    Error& operator=(const Error &other);
    bool operator==(const Error &other) const;
    bool operator!=(const Error &other) const;
    bool operator<(const Error &other) const;
    int getID() const;
    std::string toString() const;
  private:
    int id;
  };
  namespace Errors
  {
    Error getError(std::string &name);
    std::set<Error>& allErrors();
    void init();
    extern const Error Unit_Not_Visible;
    extern const Error Unit_Not_Owned;
    extern const Error Incompatible_Race;
    extern const Error Incompatible_UnitType;
    extern const Error Incompatible_TechType;
    extern const Error Incompatible_UpgradeType;
    extern const Error Insufficient_Minerals;
    extern const Error Insufficient_Gas;
    extern const Error Insufficient_Supply;
    extern const Error Insufficient_Energy;
    extern const Error Insufficient_Tech;
    extern const Error Insufficient_Ammo;
    extern const Error Unbuildable_Location;
    extern const Error Out_Of_Range;
    extern const Error Unable_To_Hit;
    extern const Error None;
    extern const Error Unknown;
  }
}
