#pragma once
#include <BWAPI/Position.h>
#include <set>
#include <BWAPI/Client/RegionData.h>

namespace BWAPI
{
  class RegionImpl
  {
  public:
    int getID() const;
    int getRegionGroupID() const;
    BWAPI::Position getCenter() const;
    bool  isHigherGround() const;
    int   getDefensePriority() const;
    bool  isAccessible() const;

    const std::set<RegionImpl*> &getNeighbors() const;

    int getBoundsLeft() const;
    int getBoundsTop() const;
    int getBoundsRight() const;
    int getBoundsBottom() const;

    RegionImpl* getClosestAccessibleRegion() const;
    RegionImpl* getClosestInaccessibleRegion() const;

    RegionImpl(int id);
    void UpdateRegionRelations();
    RegionData* getData();
  private:
    RegionData  data = RegionData();
    RegionData* self = &data;
    
    std::set<RegionImpl*> neighbors;
    RegionImpl* closestAccessibleRgn;
    RegionImpl* closestInaccessibleRgn;
  };

};

