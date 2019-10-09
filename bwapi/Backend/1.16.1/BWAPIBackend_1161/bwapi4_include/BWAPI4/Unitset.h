#pragma once
#include <BWAPI/Position.h>
#include <iterator>
#include <set>

namespace BWAPI
{
  class UnitType;
  class TechType;
}
namespace BWAPI4
{
  // Forward declarations
  class UnitCommand;
  class UnitImpl;

  /// <summary>The Unitset is a container for a set of pointers to Unit objects. It is typically
  /// used for groups of units instead of having to manage each Unit individually.</summary>
  ///
  /// @see Unit
  class Unitset : public std::set<BWAPI4::UnitImpl*>
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
    /// @see UnitImpl::getPosition
    BWAPI::Position getPosition() const;
  };
}

