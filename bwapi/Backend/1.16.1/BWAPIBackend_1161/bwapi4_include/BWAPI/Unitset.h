#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>
#include <iterator>
#include <set>

namespace BWAPI
{
  // Forward declarations
  class UnitType;
  class UnitCommand;
  class TechType;

  /// <summary>The Unitset is a container for a set of pointers to Unit objects. It is typically
  /// used for groups of units instead of having to manage each Unit individually.</summary>
  ///
  /// @see Unit
  class Unitset : public std::set<BWAPI::Unit>
  {
  public:
    /// <summary>A blank Unitset containing no elements.</summary> This is typically used as a
    /// return value for BWAPI interface functions that have encountered an error.
    static const Unitset none;

    Unitset& operator = (const Unitset& other) {
        clear();
        std::copy(other.begin(), other.end(), std::inserter(*this, begin()));
        return *this;
    }

    /// <summary>Calculates the average of all valid Unit positions in this set.</summary>
    ///
    /// @returns Average Position of all units in the set.
    ///
    /// @see UnitInterface::getPosition
    Position getPosition() const;
  };
}

