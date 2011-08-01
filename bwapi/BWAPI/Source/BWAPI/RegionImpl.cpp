#include "RegionImpl.h"

#include <BW/Offsets.h>
#include <Util/Foreach.h>

namespace BWAPI
{
  RegionImpl::RegionImpl(int id)
  {
    // Assuming this is called via GameInternals, so no checks are made
    BW::region *r         = &BW::BWDATA_SAIPathing->regions[id];

    this->islandID        = r->groupIndex;
    this->center          = BWAPI::Position(r->getCenter().x, r->getCenter().y);
    this->isWalkable      = r->accessabilityFlags != 0x1FFD;
    this->isHigherGround  = r->accessabilityFlags == 0x1FF9;
    foreach ( BW::region *n, r->getAccessibleNeighbours() )
    {
      //this->neighbors.insert(n); // Do something to convert BW to BWAPI region
    }
    this->priority = r->properties & 0x7F;
  }
  RegionImpl::~RegionImpl()
  {
  }
  int RegionImpl::getRegionGroupID() const
  {
    return this->islandID;
  }
  BWAPI::Position RegionImpl::getCenter() const
  {
    return this->center;
  }
};
