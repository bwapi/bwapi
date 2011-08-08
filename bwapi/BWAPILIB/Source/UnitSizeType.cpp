#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitSizeType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingUnitSizeType = true;
  std::string unitSizeTypeName[6];
  std::map<std::string, UnitSizeType> unitSizeTypeMap;
  std::set< UnitSizeType > unitSizeTypeSet;
  namespace UnitSizeTypes
  {
    const UnitSizeType Independent(0);
    const UnitSizeType Small(1);
    const UnitSizeType Medium(2);
    const UnitSizeType Large(3);
    const UnitSizeType None(4);
    const UnitSizeType Unknown(5);

#define SET_SIZE_TYPE(x) unitSizeTypeName[x] = underscoresToSpaces(#x); unitSizeTypeSet.insert(x);
    void init()
    {
      SET_SIZE_TYPE(Independent);
      SET_SIZE_TYPE(Small);
      SET_SIZE_TYPE(Medium);
      SET_SIZE_TYPE(Large);
      SET_SIZE_TYPE(None);
      SET_SIZE_TYPE(Unknown);

      foreach(UnitSizeType i, unitSizeTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        unitSizeTypeMap.insert(std::make_pair(name, i));
      }
      initializingUnitSizeType = false;
    }
  }
  UnitSizeType::UnitSizeType()
  {
    this->id = UnitSizeTypes::None;
  }
  UnitSizeType::UnitSizeType(int id)
  {
    this->id = id;
    if (!initializingUnitSizeType && (id < 0 || id >= 6))
      this->id = UnitSizeTypes::Unknown;
  }
  UnitSizeType::UnitSizeType(const UnitSizeType& other)
  {
    this->id = other;
  }
  UnitSizeType& UnitSizeType::operator=(const UnitSizeType& other)
  {
    this->id = other;
    return *this;
  }
  UnitSizeType::operator int() const
  {
    return id;
  }
  int UnitSizeType::getID() const
  {
    return this->id;
  }
  std::string UnitSizeType::getName() const
  {
    return unitSizeTypeName[this->id];
  }
  const char *UnitSizeType::c_str() const
  {
    return unitSizeTypeName[this->id].c_str();
  }
  UnitSizeType UnitSizeTypes::getUnitSizeType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UnitSizeType>::iterator i = unitSizeTypeMap.find(name);
    if (i == unitSizeTypeMap.end())
      return UnitSizeTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitSizeType>& UnitSizeTypes::allUnitSizeTypes()
  {
    return unitSizeTypeSet;
  }
}

