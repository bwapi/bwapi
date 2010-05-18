#pragma once
#include <string>
#include <set>
namespace BWAPI
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

      /** Returns a unique ID for this UnitSizeType. */
      int getID() const;

      /** Returns the string corresponding to the UnitSizeType object. For example,
       * UnitSizeTypes::Medium.getName() returns std::string("Medium")*/
      std::string getName() const;
    private:
      int id;
  };
  namespace UnitSizeTypes
  {
    /** Given a string, this function returns the size type it refers to. For example,
     * UnitSizeTypes::getUnitSizeType("Small") returns UnitSizeTypes::Small. */
    UnitSizeType getUnitSizeType(std::string& name);

    /** Returns the set of all the sizes, which are listed below: */
    std::set<UnitSizeType>& allUnitSizeTypes();
    void init();
    extern const UnitSizeType Independent;
    extern const UnitSizeType Small;
    extern const UnitSizeType Medium;
    extern const UnitSizeType Large;
    extern const UnitSizeType None;
    extern const UnitSizeType Unknown;
  }
}
