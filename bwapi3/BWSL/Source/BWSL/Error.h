#pragma once

#include "BWSLInterface.h"

IP_STRIP

#include <string>
#include <set>

namespace BWSL
{
  class UnitType;
  class Error
  {
    private:
      int id;
    public:
      BWSL_EXPORT Error();
      BWSL_EXPORT Error(int id);
      BWSL_EXPORT Error(const Error& other);
      BWSL_EXPORT Error& operator=(const Error& other);
      BWSL_EXPORT bool operator==(const Error& other) const;
      BWSL_EXPORT bool operator!=(const Error& other) const;
      BWSL_EXPORT bool operator<(const Error& other) const;
      BWSL_EXPORT int getID() const;
      BWSL_EXPORT std::string toString() const;
  };
  namespace Errors
  {
    Error getError(std::string& name);
    std::set<Error>& allErrors();
    void init();
    BWSL_EXPORT extern const Error Connection_Failure;
    BWSL_EXPORT extern const Error Unit_Does_Not_Exist;
    BWSL_EXPORT extern const Error Unit_Not_Visible;
    BWSL_EXPORT extern const Error Unit_Not_Owned;
    BWSL_EXPORT extern const Error Unit_Busy;
    BWSL_EXPORT extern const Error Incompatible_UnitType;
    BWSL_EXPORT extern const Error Incompatible_TechType;
    BWSL_EXPORT extern const Error Already_Researched;
    BWSL_EXPORT extern const Error Fully_Upgraded;
    BWSL_EXPORT extern const Error Insufficient_Minerals;
    BWSL_EXPORT extern const Error Insufficient_Gas;
    BWSL_EXPORT extern const Error Insufficient_Supply;
    BWSL_EXPORT extern const Error Insufficient_Energy;
    BWSL_EXPORT extern const Error Insufficient_Tech;
    BWSL_EXPORT extern const Error Insufficient_Ammo;
    BWSL_EXPORT extern const Error Insufficient_Space;
    BWSL_EXPORT extern const Error Unbuildable_Location;
    BWSL_EXPORT extern const Error Out_Of_Range;
    BWSL_EXPORT extern const Error Unable_To_Hit;
    BWSL_EXPORT extern const Error Access_Denied;
    BWSL_EXPORT extern const Error None;
    BWSL_EXPORT extern const Error Unknown;
  }
}
