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

    void init()
    {
      unitSizeTypeName[Independent.getID()] = "Independent";
      unitSizeTypeName[Small.getID()]       = "Small";
      unitSizeTypeName[Medium.getID()]      = "Medium";
      unitSizeTypeName[Large.getID()]       = "Large";
      unitSizeTypeName[None.getID()]        = "None";
      unitSizeTypeName[Unknown.getID()]     = "Unknown";

      unitSizeTypeSet.insert(Independent);
      unitSizeTypeSet.insert(Small);
      unitSizeTypeSet.insert(Medium);
      unitSizeTypeSet.insert(Large);
      unitSizeTypeSet.insert(None);
      unitSizeTypeSet.insert(Unknown);

      foreach(UnitSizeType i, unitSizeTypeSet)
      {
        std::string name = i.getName();
        fixName(&name);
        unitSizeTypeMap.insert(std::make_pair(name, i));
      }
      initializingUnitSizeType = false;
    }
  }
  UnitSizeType::UnitSizeType()
  {
    this->id = UnitSizeTypes::None.id;
  }
  UnitSizeType::UnitSizeType(int id)
  {
    this->id = id;
    if (!initializingUnitSizeType && (id < 0 || id >= 6))
      this->id = UnitSizeTypes::Unknown.id;
  }
  UnitSizeType::UnitSizeType(const UnitSizeType& other)
  {
    this->id = other.id;
  }
  UnitSizeType& UnitSizeType::operator=(const UnitSizeType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool UnitSizeType::operator==(const UnitSizeType& other) const
  {
    return this->id == other.id;
  }
  bool UnitSizeType::operator!=(const UnitSizeType& other) const
  {
    return this->id != other.id;
  }
  bool UnitSizeType::operator<(const UnitSizeType& other) const
  {
    return this->id < other.id;
  }
  int UnitSizeType::getID() const
  {
    return this->id;
  }
  std::string UnitSizeType::getName() const
  {
    return unitSizeTypeName[this->id];
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

