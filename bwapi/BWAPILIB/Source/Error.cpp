#include <string>
#include <map>
#include <set>
#include <BWAPI/Error.h>
#include <Util/Foreach.h>

#include "Common.h"

namespace BWAPI
{
  bool initializingError = true;
  std::string errorName[23];
  std::map<std::string, Error> errorMap;
  std::set< Error > errorSet;
  namespace Errors
  {
    const Error Unit_Does_Not_Exist(0);
    const Error Unit_Not_Visible(1);
    const Error Unit_Not_Owned(2);
    const Error Unit_Busy(3);
    const Error Incompatible_UnitType(4);
    const Error Incompatible_TechType(5);
    const Error Already_Researched(6);
    const Error Fully_Upgraded(7);
    const Error Currently_Researching(8);
    const Error Currently_Upgrading(9);
    const Error Insufficient_Minerals(10);
    const Error Insufficient_Gas(11);
    const Error Insufficient_Supply(12);
    const Error Insufficient_Energy(13);
    const Error Insufficient_Tech(14);
    const Error Insufficient_Ammo(15);
    const Error Insufficient_Space(16);
    const Error Unbuildable_Location(17);
    const Error Out_Of_Range(18);
    const Error Unable_To_Hit(19);
    const Error Access_Denied(20);
    const Error None(21);
    const Error Unknown(22);

    void init()
    {
      errorName[Unit_Does_Not_Exist.getID()]   = "Unit Does Not Exist";
      errorName[Unit_Not_Visible.getID()]      = "Unit Not Visible";
      errorName[Unit_Not_Owned.getID()]        = "Unit Not Owned";
      errorName[Unit_Busy.getID()]             = "Unit Busy";
      errorName[Incompatible_UnitType.getID()] = "Incompatible UnitType";
      errorName[Incompatible_TechType.getID()] = "Incompatible TechType";
      errorName[Already_Researched.getID()]    = "Already Researched";
      errorName[Fully_Upgraded.getID()]        = "Fully Upgraded";
      errorName[Currently_Researching.getID()] = "Currently Researching";
      errorName[Currently_Upgrading.getID()]   = "Currently Upgrading";
      errorName[Insufficient_Minerals.getID()] = "Insufficient Minerals";
      errorName[Insufficient_Gas.getID()]      = "Insufficient Gas";
      errorName[Insufficient_Supply.getID()]   = "Insufficient Supply";
      errorName[Insufficient_Energy.getID()]   = "Insufficient Energy";
      errorName[Insufficient_Tech.getID()]     = "Insufficient Tech";
      errorName[Insufficient_Ammo.getID()]     = "Insufficient Ammo";
      errorName[Insufficient_Space.getID()]    = "Insufficient Space";
      errorName[Unbuildable_Location.getID()]  = "Unbuildable Location";
      errorName[Out_Of_Range.getID()]          = "Out Of Range";
      errorName[Unable_To_Hit.getID()]         = "Unable To Hit";
      errorName[Access_Denied.getID()]         = "Access Denied";
      errorName[None.getID()]                  = "None";
      errorName[Unknown.getID()]               = "Unknown";

      errorSet.insert(Unit_Does_Not_Exist);
      errorSet.insert(Unit_Not_Visible);
      errorSet.insert(Unit_Not_Owned);
      errorSet.insert(Unit_Busy);
      errorSet.insert(Incompatible_UnitType);
      errorSet.insert(Incompatible_TechType);
      errorSet.insert(Already_Researched);
      errorSet.insert(Fully_Upgraded);
      errorSet.insert(Currently_Researching);
      errorSet.insert(Currently_Upgrading);
      errorSet.insert(Insufficient_Minerals);
      errorSet.insert(Insufficient_Gas);
      errorSet.insert(Insufficient_Supply);
      errorSet.insert(Insufficient_Energy);
      errorSet.insert(Insufficient_Tech);
      errorSet.insert(Insufficient_Ammo);
      errorSet.insert(Insufficient_Space);
      errorSet.insert(Unbuildable_Location);
      errorSet.insert(Out_Of_Range);
      errorSet.insert(Unable_To_Hit);
      errorSet.insert(Access_Denied);
      errorSet.insert(None);
      errorSet.insert(Unknown);

      foreach(Error i, errorSet)
      {
        std::string name = i.toString();
        fixName(&name);
        errorMap.insert(std::make_pair(name, i));
      }
      initializingError = false;
    }
  }

  Error::Error()
  {
    this->id = Errors::None.id;
  }
  Error::Error(int id)
  {
    this->id = id;
    if (!initializingError && (id < 0 || id >= 23))
      this->id = Errors::Unknown.id;
  }
  Error::Error(const Error& other)
  {
    this->id = other.id;
  }
  Error& Error::operator=(const Error& other)
  {
    this->id = other.id;
    return *this;
  }
  bool Error::operator==(const Error& other) const
  {
    return this->id == other.id;
  }
  bool Error::operator!=(const Error& other) const
  {
    return this->id != other.id;
  }
  bool Error::operator<(const Error& other) const
  {
    return this->id < other.id;
  }
  int Error::getID() const
  {
    return this->id;
  }
  std::string Error::toString() const
  {
    return errorName[this->id];
  }
  Error Errors::getError(std::string name)
  {
    fixName(&name);
    std::map<std::string, Error>::iterator i = errorMap.find(name);
    if (i == errorMap.end())
      return Errors::Unknown;
    return (*i).second;
  }
  std::set<Error>& Errors::allErrors()
  {
    return errorSet;
  }
}
