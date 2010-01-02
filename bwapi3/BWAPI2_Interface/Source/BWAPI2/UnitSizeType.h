#pragma once
#include "BWAPI2Interface.h"

#include <string>
#include <set>

namespace BWAPI2
{
  class UnitSizeType
  {
    public:
      UnitSizeType();
      UnitSizeType(int id);
      UnitSizeType(const UnitSizeType& other);
      UnitSizeType& operator=(const UnitSizeType& other);
      bool operator==(const UnitSizeType& other) const;
      bool operator!=(const UnitSizeType& other) const;
      bool operator<(const UnitSizeType& other) const;
      int getID() const;
      std::string getName() const;
    private:
      int id;
  };
  namespace UnitSizeTypes
  {
    UnitSizeType getUnitSizeType(std::string& name);
    std::set<UnitSizeType>& allUnitSizeTypes();
    void init();
    BWAPI2_EXPORT extern const UnitSizeType Independent;
    BWAPI2_EXPORT extern const UnitSizeType Small;
    BWAPI2_EXPORT extern const UnitSizeType Medium;
    BWAPI2_EXPORT extern const UnitSizeType Large;
    BWAPI2_EXPORT extern const UnitSizeType None;
    BWAPI2_EXPORT extern const UnitSizeType Unknown;
  }
}
