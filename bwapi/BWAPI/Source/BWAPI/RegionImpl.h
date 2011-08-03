#pragma once
#include <set>
#include <vector>
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
    void AddPoint(int x, int y);
    std::vector<BWAPI::Position> &getPolygon();
  private:
    RegionData data;
    RegionData *self;
    
    std::set<Region*> accessableNeighbors;
    std::set<Region*> inaccessableNeighbors;
    std::vector<BWAPI::Position> polygon;
    int regionID;
  };

};

