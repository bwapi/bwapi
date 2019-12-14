#pragma once

#include <set>

#include <BWAPI/Unit.h>

namespace BWAPI
{
  class Unitset;

  class UnitFinder
  {
  public:
    void update(const Unitset& units);
    Unitset find(int left, int top, int right, int bottom) const;
  private:
    struct UnitCompare
    {
      UnitCompare(int value, Unit unit) : value(value), unit(unit) {}

      int value;
      Unit unit;

      bool operator <(UnitCompare const &other) const
      {
        return value < other.value;
      }
    };

    std::multiset<UnitCompare> unitFinderX;
    std::multiset<UnitCompare> unitFinderY;
  };
}
