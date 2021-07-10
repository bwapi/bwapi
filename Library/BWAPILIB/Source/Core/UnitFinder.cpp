#include <BWAPI/UnitFinder.h>

#include <BWAPI/Unitset.h>

#include <iterator>
#include <algorithm>
#include <set>
#include <unordered_set>

using namespace BWAPI;

Unitset UnitFinder::find(int left, int top, int right, int bottom) const
{
  int searchRight = right, searchBottom = bottom;

  // Extension is to take into account units whose dimensions completely eclipse the location being searched.
  // The unit with the largest width/height can't eclipse the search 
  if (right - left <= UnitTypes::maxUnitWidth())
    searchRight = left + UnitTypes::maxUnitWidth();
  if (bottom - top <= UnitTypes::maxUnitHeight())
    searchBottom = top + UnitTypes::maxUnitHeight();

  std::unordered_set<Unit> foundUnitsX;
  auto lowerXIter = unitFinderX.lower_bound(UnitCompare{ left, 0 });
  // We add 1 to right and bottom search because upper_bound is not inclusive - we want it to be
  auto upperXIter = unitFinderX.upper_bound(UnitCompare{ searchRight + 1, 0 });
  for (auto it = lowerXIter; it != upperXIter; ++it)
  {
    if (it->unit.getLeft() <= right)
      foundUnitsX.insert(it->unit);
  }
  
  Unitset result;

  auto lowerYIter = unitFinderY.lower_bound(UnitCompare{ top, 0 });
  auto upperYIter = unitFinderY.upper_bound(UnitCompare{ searchBottom + 1, 0 });
  for (auto it = lowerYIter; it != upperYIter; ++it)
  {
    // Ignore if the unit isn't within the X-bound
    if (foundUnitsX.count(it->unit) == 0) continue;

    if (it->unit.getTop() <= bottom && it->unit.exists())
      result.insert(it->unit);
  }
  return result;
}

void UnitFinder::update(const Unitset& units)
{
  unitFinderX.clear();
  unitFinderY.clear();
  for (const Unit& u : units)
  {
    unitFinderX.emplace(u.getLeft(), u);
    unitFinderX.emplace(u.getRight(), u);
    unitFinderY.emplace(u.getTop(), u);
    unitFinderY.emplace(u.getBottom(), u);
  }
}
