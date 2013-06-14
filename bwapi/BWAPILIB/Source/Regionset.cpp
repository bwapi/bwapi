#include <BWAPI/Vectorset.h>
#include <BWAPI/Regionset.h>
#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>

#include <utility>

namespace BWAPI
{
  Regionset::Regionset(size_t initialSize) : Vectorset(initialSize) { };
  Regionset::Regionset(const Regionset &other) : Vectorset(other) { };
  Regionset::Regionset(Regionset &&other) : Vectorset( std::forward<Regionset>(other) ) { };

  Position Regionset::getCenter() const
  {
    Position sum; // The sum of all positions
    int count = 0; // The number of valid positions

    for ( auto r = this->begin(); r != this->end(); ++r )
    {
      Position p = r->getCenter();
      if ( p )  // Only use if position is valid/known
      {
        sum += p;
        ++count;
      }
    } // for
    return sum /= count;
  }

  Unitset Regionset::getUnits(const UnitFilter &pred) const
  {
    Unitset units;
    for ( auto r = this->begin(); r != this->end(); ++r )
      units += r->getUnits(pred);
    return units;
  }
}

