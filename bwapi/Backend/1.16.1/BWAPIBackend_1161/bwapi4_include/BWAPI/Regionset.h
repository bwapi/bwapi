#pragma once
#include "SetContainer.h"
#include <BWAPI/Position.h>

namespace BWAPI
{
  // Forward Declarations
  class RegionInterface;
  typedef RegionInterface *Region;

  class Unitset;

  /// <summary>A container that holds a set of Region objects.</summary>
  class Regionset : public SetContainer<BWAPI::Region, std::hash<void*>>
  {
  public:

  };
}

