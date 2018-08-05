#include "RegionImpl.h"

#include <BW/Offsets.h>
#include <BW/Pathing.h>

#include <BWAPI/Game.h>
#include <BWAPI/GameImpl.h>

#include "../../../Debug.h"

namespace BWAPI
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
    
    this->closestAccessibleRgn    = nullptr;
    this->closestInaccessibleRgn  = nullptr;
  }
  void RegionImpl::UpdateRegionRelations()
  {
    // Assuming this is called via GameInternals, so no checks are made
    const BW::region * const r = &BW::BWDATA::SAIPathing->regions[data.id];

    // Assign region neighbors
    this->neighbors.clear();

    int accessibleBestDist    = 99999;
    int inaccessibleBestDist  = 99999;
    for ( int n = 0; n < r->neighborCount; ++n )
    {
      BW::region *neighbor = r->getNeighbor(static_cast<u8>(n));
      BWAPI::Region bwapiNeighbor = BroodwarImpl.getRegion(neighbor->getIndex());

      // continue if this is null (but it shouldn't be)
      if ( !bwapiNeighbor )
        continue;

      // add our neighbor
      this->neighbors.insert(bwapiNeighbor);

      // Obtain the closest accessible and inaccessible Regions from their Region center
      int dst = r->getCenter().getApproxDistance(neighbor->getCenter());
      if ( r->isConnectedTo( neighbor ) )
      {
        if ( dst < accessibleBestDist )
        {
          accessibleBestDist = dst;
          this->closestAccessibleRgn = bwapiNeighbor;
        }
      }
      else if ( dst < inaccessibleBestDist )
      {
        inaccessibleBestDist = dst;
        this->closestInaccessibleRgn = bwapiNeighbor;
      }

      // Client compatibility for neighbors
      ++data.neighborCount;
      data.neighbors[n] = neighbor->getIndex();
    }
  }
  RegionData &RegionImpl::getData()
  {
    return data;
  }
};
