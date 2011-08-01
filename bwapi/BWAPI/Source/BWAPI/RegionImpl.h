#pragma once
#include <set>
#include <BWAPI/Region.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class RegionImpl : public Region
  {
  public:
    virtual int getRegionGroupID() const;
    virtual BWAPI::Position getCenter() const;

    RegionImpl(int id);
    ~RegionImpl();
  private:
    int islandID;
    BWAPI::Position center;
    bool isWalkable;
    bool isHigherGround;
    int  priority;
    
    std::set<Region*> neighbors;
  };

};

