#include "RegionImpl.h"

#include <BW/Offsets.h>
#include <BW/Pathing.h>

#include <BWAPI/Game.h>

#include "../../../Debug.h"

namespace BWAPI
{
  RegionImpl::RegionImpl(int id)
  {
    // Assuming this is called via GameInternals, so no checks are made
    const BW::region * const r = &BW::BWDATA::SAIPathing->regions[id];

    // Assign common region properties
    self->islandID        = r->groupIndex;
    self->center_x        = r->getCenter().x;
    self->center_y        = r->getCenter().y;

    self->isAccessible    = r->accessabilityFlags != 0x1FFD;
    self->isHigherGround  = r->accessabilityFlags == 0x1FF9;
    self->priority        = r->defencePriority & 0x7F;
    self->leftMost        = r->rgnBox.left;
    self->rightMost       = r->rgnBox.right;
    self->topMost         = r->rgnBox.top;
    self->bottomMost      = r->rgnBox.bottom;

    // Connect the BWAPI Region and BW Region two ways
    self->id  = id;
    
    this->closestAccessibleRgn    = nullptr;
    this->closestInaccessibleRgn  = nullptr;
  }
  void RegionImpl::UpdateRegionRelations()
  {
    // Assuming this is called via GameInternals, so no checks are made
    const BW::region * const r = &BW::BWDATA::SAIPathing->regions[self->id];

    // Assign region neighbors
    this->neighbors.clear();

    int accessibleBestDist    = 99999;
    int inaccessibleBestDist  = 99999;
    for ( int n = 0; n < r->neighborCount; ++n )
    {
      BW::region *neighbor = r->getNeighbor(static_cast<u8>(n));
      BWAPI::Region bwapiNeighbor = Broodwar->getRegion(neighbor->getIndex());

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
      ++self->neighborCount;
      self->neighbors[n] = neighbor->getIndex();
    }
  }
  RegionData *RegionImpl::getData()
  {
    return self;
  }
};
