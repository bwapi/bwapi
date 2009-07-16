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
    UnitSizeType(const UnitSizeType &other);
    UnitSizeType& operator=(const UnitSizeType &other);
    bool operator==(const UnitSizeType &other) const;
    bool operator!=(const UnitSizeType &other) const;
    bool operator<(const UnitSizeType &other) const;
    int getID() const;
    std::string getName() const;
  private:
    int id;
  };
  namespace UnitSizeTypes
  {
    UnitSizeType getUnitSizeType(std::string &name);
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
