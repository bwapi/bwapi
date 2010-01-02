#include <string>
#include <map>
#include <set>
#include "UnitSizeType.h"

namespace BWAPI2
{
  bool initializingUnitSizeType = true;
  std::string unitSizeTypeName[6];
  std::map<std::string, UnitSizeType> unitSizeTypeMap;
  std::set< UnitSizeType > unitSizeTypeSet;
  namespace UnitSizeTypes
  {
    BWAPI2_EXPORT const UnitSizeType Independent(0);
    BWAPI2_EXPORT const UnitSizeType Small(1);
    BWAPI2_EXPORT const UnitSizeType Medium(2);
    BWAPI2_EXPORT const UnitSizeType Large(3);
    BWAPI2_EXPORT const UnitSizeType None(4);
    BWAPI2_EXPORT const UnitSizeType Unknown(5);
    void init()
    {
      unitSizeTypeName[Independent.getID()] = "Independent";
      unitSizeTypeName[Small.getID()] = "Small";
      unitSizeTypeName[Medium.getID()] = "Medium";
      unitSizeTypeName[Large.getID()] = "Large";
      unitSizeTypeName[None.getID()] = "None";
      unitSizeTypeName[Unknown.getID()] = "Unknown";

      unitSizeTypeSet.insert(Independent);
      unitSizeTypeSet.insert(Small);
      unitSizeTypeSet.insert(Medium);
      unitSizeTypeSet.insert(Large);
      unitSizeTypeSet.insert(None);
      unitSizeTypeSet.insert(Unknown);

      for(std::set<UnitSizeType>::iterator i = unitSizeTypeSet.begin(); i != unitSizeTypeSet.end(); i++)
      {
        unitSizeTypeMap.insert(std::make_pair((*i).getName(), *i));
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
    if (!initializingUnitSizeType)
    {
      if (id < 0 || id >= 6)
      {
        this->id = UnitSizeTypes::Unknown.id;
      }
    }
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

  UnitSizeType UnitSizeTypes::getUnitSizeType(std::string& name)
  {
    std::map<std::string, UnitSizeType>::iterator i = unitSizeTypeMap.find(name);
    if (i == unitSizeTypeMap.end()) return UnitSizeTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitSizeType>& UnitSizeTypes::allUnitSizeTypes()
  {
    return unitSizeTypeSet;
  }
}

