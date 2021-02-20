#include "RegionImpl.h"

#include "GameImpl.h"

#include "../BW/Offsets.h"
#include "../BW/Pathing.h"

namespace BWAPI4
{
  RegionImpl::RegionImpl(int id)
  {
    // Assuming this is called via GameInternals, so no checks are made
    const BW::region * const r = &BW::BWDATA::SAIPathing->regions[id];

    // Assign common region properties
    data.islandID        = r->groupIndex;
    data.center_x        = r->getCenter().x;
    data.center_y        = r->getCenter().y;

    data.isAccessible    = r->accessabilityFlags != 0x1FFD;
    data.isHigherGround  = r->accessabilityFlags == 0x1FF9;
    data.priority        = r->defencePriority & 0x7F;
    data.leftMost        = r->rgnBox.left;
    data.rightMost       = r->rgnBox.right;
    data.topMost         = r->rgnBox.top;
    data.bottomMost      = r->rgnBox.bottom;

    // Connect the BWAPI Region and BW Region two ways
    data.id  = id;
  }
  void RegionImpl::UpdateRegionRelations()
  {
    // Assuming this is called via GameInternals, so no checks are made
    const BW::region * const r = &BW::BWDATA::SAIPathing->regions[data.id];

    // Assign region neighbors
    this->neighbors.clear();

    for ( int n = 0; n < r->neighborCount; ++n )
    {
      BW::region *neighbor = r->getNeighbor(static_cast<u8>(n));
      BWAPI4::RegionImpl* bwapiNeighbor = BroodwarImpl.getRegion(neighbor->getIndex());

      // continue if this is null (but it shouldn't be)
      if ( !bwapiNeighbor )
        continue;

      // add our neighbor
      this->neighbors.insert(bwapiNeighbor);

      // Client compatibility for neighbors
      data.neighborCount++;
      data.neighbors[n] = neighbor->getIndex();
    }
  }
  RegionData *RegionImpl::getData()
  {
    return &data;
  }
  // -------------------------------- GET GROUP ID -----------------------------------------------------------
  int RegionImpl::getRegionGroupID() const
  {
    return data.islandID;
  }
  // --------------------------------- GET CENTER ------------------------------------------------------------
  BWAPI::Position RegionImpl::getCenter() const
  {
    return BWAPI::Position(data.center_x, data.center_y);
  }
  // ------------------------------------ GET ID -------------------------------------------------------------
  int RegionImpl::getID() const
  {
    return data.id;
  }
};
