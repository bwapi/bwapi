#pragma once
#include "SetContainer.h"
#include <BWAPI/Position.h>
#include <BWAPI/Filters.h>
#include <BWAPI/Region.h>

namespace BWAPI
{
  // Forward Declarations
  class Unitset;

  /// <summary>A container that holds a set of Region objects.</summary>
  class Regionset : public SetContainer<BWAPI::Region, std::hash<void*>>
  {
  public:

    /// @copydoc Region::getCenter
    Position getCenter() const;

    /// @copydoc Region::getUnits
    Unitset getUnits(const UnitFilter &pred = nullptr) const;
  };
}

