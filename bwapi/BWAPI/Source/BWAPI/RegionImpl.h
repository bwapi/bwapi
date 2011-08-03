#pragma once
#include <set>
#include <BWAPI/Region.h>
#include <BWAPI/Position.h>

#include <BWAPI/Client/RegionData.h>

namespace BWAPI
{
  class RegionImpl : public Region
  {
  public:
    virtual int getRegionGroupID() const;
    virtual BWAPI::Position getCenter() const;

    RegionImpl(int id);
    ~RegionImpl();
    void UpdateRegionRelations();
  private:
    RegionData data;
    RegionData *self;
    
    std::set<Region*> accessableNeighbors;
    std::set<Region*> inaccessableNeighbors;
    int regionID;
  };

};

