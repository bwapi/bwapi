#include <string>
#include <map>
#include <set>
#include <BWAPI/Error.h>

namespace BWAPI
{
  bool initializingError=true;
  std::string errorName[16];
  std::map<std::string, Error> errorMap;
  std::set< Error > errorSet;
  namespace Errors
  {
    const Error Unit_Not_Visible(0);
    const Error Unit_Not_Owned(1);
    const Error Incompatible_UnitType(2);
    const Error Incompatible_TechType(3);
    const Error Incompatible_UpgradeType(4);
    const Error Insufficient_Minerals(5);
    const Error Insufficient_Gas(6);
    const Error Insufficient_Supply(7);
    const Error Insufficient_Energy(8);
    const Error Insufficient_Tech(9);
    const Error Insufficient_Ammo(10);
    const Error Unbuildable_Location(11);
    const Error Out_Of_Range(12);
    const Error Unable_To_Hit(13);
    const Error None(14);
    const Error Unknown(15);
    void init()
    {
      errorName[Unit_Not_Visible.getID()]="Unit Not Visible";
      errorName[Unit_Not_Owned.getID()]="Unit Not Owned";
      errorName[Incompatible_UnitType.getID()]="Incompatible UnitType";
      errorName[Incompatible_TechType.getID()]="Incompatible TechType";
      errorName[Incompatible_UpgradeType.getID()]="Incompatible UpgradeType";
      errorName[Insufficient_Minerals.getID()]="Insufficient Minerals";
      errorName[Insufficient_Gas.getID()]="Insufficient Gas";
      errorName[Insufficient_Supply.getID()]="Insufficient Supply";
      errorName[Insufficient_Energy.getID()]="Insufficient Energy";
      errorName[Insufficient_Tech.getID()]="Insufficient Tech";
      errorName[Insufficient_Ammo.getID()]="Insufficient Ammo";
      errorName[Unbuildable_Location.getID()]="Unbuildable Location";
      errorName[Out_Of_Range.getID()]="Out Of Range";
      errorName[Unable_To_Hit.getID()]="Unable To Hit";
      errorName[None.getID()]="None";
      errorName[Unknown.getID()]="Unknown";
      errorSet.insert(Unit_Not_Visible);
      errorSet.insert(Unit_Not_Owned);
      errorSet.insert(Incompatible_UnitType);
      errorSet.insert(Incompatible_TechType);
      errorSet.insert(Incompatible_UpgradeType);
      errorSet.insert(Insufficient_Minerals);
      errorSet.insert(Insufficient_Gas);
      errorSet.insert(Insufficient_Supply);
      errorSet.insert(Insufficient_Energy);
      errorSet.insert(Insufficient_Tech);
      errorSet.insert(Insufficient_Ammo);
      errorSet.insert(Unbuildable_Location);
      errorSet.insert(Out_Of_Range);
      errorSet.insert(Unable_To_Hit);
      errorSet.insert(None);
      errorSet.insert(Unknown);
      for(std::set<Error>::iterator i=errorSet.begin();i!=errorSet.end();i++)
      {
        errorMap.insert(std::make_pair((*i).toString(),*i));
      }
      initializingError=false;
    }
  }

  Error::Error()
  {
    this->id=Errors::None.id;
  }
  Error::Error(int id)
  {
    this->id=id;
    if (!initializingError)
    {
      if (id<0 || id>=16)
      {
        this->id=Errors::Unknown.id;
      }
    }
  }
  Error::Error(const Error &other)
  {
    this->id=other.id;
  }
  Error& Error::operator=(const Error &other)
  {
    this->id=other.id;
    return *this;
  }
  bool Error::operator==(const Error &other) const
  {
    return this->id==other.id;
  }
  bool Error::operator!=(const Error &other) const
  {
    return this->id!=other.id;
  }
  bool Error::operator<(const Error &other) const
  {
    return this->id<other.id;
  }
  int Error::getID() const
  {
    return this->id;
  }
  std::string Error::toString() const
  {
    return errorName[this->id];
  }
  Error Errors::getError(std::string &name)
  {
    std::map<std::string, Error>::iterator i=errorMap.find(name);
    if (i==errorMap.end()) return Errors::Unknown;
    return (*i).second;
  }
  std::set<Error>& Errors::allErrors()
  {
    return errorSet;
  }
}
