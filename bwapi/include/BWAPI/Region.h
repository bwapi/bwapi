#pragma once

#include <set>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Region
  {
  public:
    /** Returns an ID for the body of land that this region belongs to. */
    virtual int getRegionGroupID() const = 0;

    /** Retrieves the center of the region. */
    virtual BWAPI::Position getCenter() const = 0;
  };
};
