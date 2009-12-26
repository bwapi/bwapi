#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  struct UnitSizeType
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
}
