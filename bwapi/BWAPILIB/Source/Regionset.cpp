#include <BWAPI/Regionset.h>
#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>

#include <utility>

namespace BWAPI
{
  Position Regionset::getCenter() const
  {
    Position sum(0,0); // The sum of all positions
    int count = 0; // The number of valid positions

    for ( auto &r : *this )
    {
      Position p = r->getCenter();
      if ( p )  // Only use if position is valid/known
      {
        sum += p;
        ++count;
      }
    }
    
    if (count > 0)
      sum /= count;

    return sum;
  }

  Unitset Regionset::getUnits(const UnitFilter &pred) const
  {
    Unitset units;
    for (auto &r : *this)
    {
      auto units = r->getUnits(pred);
      units.insert(units.begin(), units.end());
    }
    return units;
  }
}

