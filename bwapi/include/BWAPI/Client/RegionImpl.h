#pragma once
#include <BWAPI/Region.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class RegionImpl : public Region
  {
  public:
    virtual int getRegionGroupID() const;
    virtual BWAPI::Position getCenter() const;
  };

};

