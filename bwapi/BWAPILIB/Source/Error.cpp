#include <string>
#include <map>
#include <set>
#include <BWAPI/Error.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

#define BWAPI_ERROR_MAX_COUNT 28

namespace BWAPI
{
  bool initializingError = true;
  std::string errorName[BWAPI_ERROR_MAX_COUNT];
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
    const Error Incompatible_State(6);
    const Error Already_Researched(7);
    const Error Fully_Upgraded(8);
    const Error Currently_Researching(9);
    const Error Currently_Upgrading(10);
    const Error Insufficient_Minerals(11);
    const Error Insufficient_Gas(12);
    const Error Insufficient_Supply(13);
    const Error Insufficient_Energy(14);
    const Error Insufficient_Tech(15);
    const Error Insufficient_Ammo(16);
    const Error Insufficient_Space(17);
    const Error Invalid_Tile_Position(18);
    const Error Unbuildable_Location(19);
    const Error Unreachable_Location(20);
    const Error Out_Of_Range(21);
    const Error Unable_To_Hit(22);
    const Error Access_Denied(23);
    const Error File_Not_Found(24);
    const Error Invalid_Parameter(25);
    const Error None(26);
    const Error Unknown(27);

#define SET_ERROR(x) errorName[x] = underscoresToSpaces(#x); errorSet.insert(x);
    void init()
    {
      SET_ERROR(Unit_Does_Not_Exist);
      SET_ERROR(Unit_Not_Visible);
      SET_ERROR(Unit_Not_Owned);
      SET_ERROR(Unit_Busy);
      SET_ERROR(Incompatible_UnitType);
      SET_ERROR(Incompatible_TechType);
      SET_ERROR(Incompatible_State);
      SET_ERROR(Already_Researched);
      SET_ERROR(Fully_Upgraded);
      SET_ERROR(Currently_Researching);
      SET_ERROR(Currently_Upgrading);
      SET_ERROR(Insufficient_Minerals);
      SET_ERROR(Insufficient_Gas);
      SET_ERROR(Insufficient_Supply);
      SET_ERROR(Insufficient_Energy);
      SET_ERROR(Insufficient_Tech);
      SET_ERROR(Insufficient_Ammo);
      SET_ERROR(Insufficient_Space);
      SET_ERROR(Invalid_Tile_Position);
      SET_ERROR(Unbuildable_Location);
      SET_ERROR(Unreachable_Location);
      SET_ERROR(Out_Of_Range);
      SET_ERROR(Unable_To_Hit);
      SET_ERROR(Access_Denied);
      SET_ERROR(File_Not_Found);
      SET_ERROR(Invalid_Parameter);
      SET_ERROR(None);
      SET_ERROR(Unknown);

      foreach(Error i, errorSet)
      {
        std::string name(i.toString());
        fixName(&name);
        errorMap.insert(std::make_pair(name, i));
      }
      initializingError = false;
    }
  }

  Error::Error()
  {
    this->id = Errors::None;
  }
  Error::Error(int id)
  {
    this->id = id;
    if (!initializingError && (id < 0 || id >= BWAPI_ERROR_MAX_COUNT))
      this->id = Errors::Unknown;
  }
  Error::Error(const Error& other)
  {
    this->id = other;
  }
  Error& Error::operator=(const Error& other)
  {
    this->id = other;
    return *this;
  }
  Error::operator int() const
  {
    return id;
  }
  int Error::getID() const
  {
    return this->id;
  }
  std::string Error::toString() const
  {
    return errorName[this->id];
  }
  const char *Error::c_str() const
  {
    return errorName[this->id].c_str();
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
