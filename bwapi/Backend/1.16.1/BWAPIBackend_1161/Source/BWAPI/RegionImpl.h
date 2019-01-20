#pragma once
#include <BWAPI/Regionset.h>
#include <BWAPI/Position.h>

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

    const Regionset &getNeighbors() const;

    int getBoundsLeft() const;
    int getBoundsTop() const;
    int getBoundsRight() const;
    int getBoundsBottom() const;

    BWAPI::Region getClosestAccessibleRegion() const;
    BWAPI::Region getClosestInaccessibleRegion() const;

    RegionImpl(int id);
    void UpdateRegionRelations();
    RegionData &getData();
  private:
    RegionData data = RegionData();
    
    Regionset neighbors;
    BWAPI::Region closestAccessibleRgn;
    BWAPI::Region closestInaccessibleRgn;
  };

};

