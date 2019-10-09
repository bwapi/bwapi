#pragma once
#include <BWAPI/Position.h>
#include <set>
#include <BWAPI4/Client/RegionData.h>

namespace BWAPI4
{
  class RegionImpl
  {
  public:
    int getID() const;
    int getRegionGroupID() const;
    BWAPI::Position getCenter() const;

    RegionImpl(int id);
    void UpdateRegionRelations();
    RegionData* getData();
  private:
    RegionData  data = RegionData();
    
    std::set<RegionImpl*> neighbors;
  };

};

