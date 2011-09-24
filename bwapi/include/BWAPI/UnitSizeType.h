#pragma once
#include <string>
#include <set>
#include "Type.h"
namespace BWAPI
{
  class UnitSizeType : public Type
  {
  public:
    UnitSizeType();
    UnitSizeType(int id);
    /** Returns the string corresponding to the UnitSizeType object. For example,
     * UnitSizeTypes::Medium.getName() returns std::string("Medium")*/
    const std::string &getName() const;
    const char *c_str() const;
  };
  namespace UnitSizeTypes
  {
    /** Given a string, this function returns the size type it refers to. For example,
     * UnitSizeTypes::getUnitSizeType("Small") returns UnitSizeTypes::Small. */
    UnitSizeType getUnitSizeType(std::string name);

    /** Returns the set of all the sizes, which are listed below: */
    const std::set<UnitSizeType>& allUnitSizeTypes();
    void init();
    extern const UnitSizeType Independent;
    extern const UnitSizeType Small;
    extern const UnitSizeType Medium;
    extern const UnitSizeType Large;
    extern const UnitSizeType None;
    extern const UnitSizeType Unknown;
  }
}
