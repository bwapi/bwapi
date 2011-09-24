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
      unitSizeTypeName[Independent] = "Independent";
      unitSizeTypeName[Small]       = "Small";
      unitSizeTypeName[Medium]      = "Medium";
      unitSizeTypeName[Large]       = "Large";
      unitSizeTypeName[None]        = "None";
      unitSizeTypeName[Unknown]     = "Unknown";

      unitSizeTypeSet.insert(Independent);
      unitSizeTypeSet.insert(Small);
      unitSizeTypeSet.insert(Medium);
      unitSizeTypeSet.insert(Large);
      unitSizeTypeSet.insert(None);
      unitSizeTypeSet.insert(Unknown);

      foreach(UnitSizeType i, unitSizeTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        unitSizeTypeMap.insert(std::make_pair(name, i));
      }
      initializingUnitSizeType = false;
    }
  }
  UnitSizeType::UnitSizeType() : Type(UnitSizeTypes::None)
  {
  }
  int getValidUnitSizeTypeID(int id)
  {
    if ( !initializingUnitSizeType && (id < 0 || id >= 6) )
      return UnitSizeTypes::Unknown;
    return id;
  }
  UnitSizeType::UnitSizeType(int id) : Type( getValidUnitSizeTypeID(id) )
  {
  }
  const std::string &UnitSizeType::getName() const
  {
    return unitSizeTypeName[this->getID()];
  }
  const char *UnitSizeType::c_str() const
  {
    return unitSizeTypeName[this->getID()].c_str();
  }
  UnitSizeType UnitSizeTypes::getUnitSizeType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UnitSizeType>::iterator i = unitSizeTypeMap.find(name);
    if (i == unitSizeTypeMap.end())
      return UnitSizeTypes::Unknown;
    return (*i).second;
  }
  const std::set<UnitSizeType>& UnitSizeTypes::allUnitSizeTypes()
  {
    return unitSizeTypeSet;
  }
}

