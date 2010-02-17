#pragma once
#include "BWSLInterface.h"

#include <string>
#include <set>

namespace BWSL
{
  class BWSL_EXPORT UnitSizeType
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
    BWSL_EXPORT extern const UnitSizeType Independent;
    BWSL_EXPORT extern const UnitSizeType Small;
    BWSL_EXPORT extern const UnitSizeType Medium;
    BWSL_EXPORT extern const UnitSizeType Large;
    BWSL_EXPORT extern const UnitSizeType None;
    BWSL_EXPORT extern const UnitSizeType Unknown;
  }
}
