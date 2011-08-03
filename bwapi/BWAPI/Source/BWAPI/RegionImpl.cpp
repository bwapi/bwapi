#include "RegionImpl.h"

#include <BW/Offsets.h>
#include <Util/Foreach.h>

namespace BWAPI
{
  RegionImpl::RegionImpl(int id)
  {
    // Set client data
    this->self = &this->data;
    memset(&this->data, 0, sizeof(this->data));

    // Assuming this is called via GameInternals, so no checks are made
    BW::region *r         = &BW::BWDATA_SAIPathing->regions[id];

    // Assign common region properties
    self->islandID        = r->groupIndex;
    self->center_x        = r->getCenter().x;
    self->center_y        = r->getCenter().y;

    self->isWalkable      = r->accessabilityFlags != 0x1FFD;
    self->isHigherGround  = r->accessabilityFlags == 0x1FF9;
    self->priority        = r->properties & 0x7F;
    self->leftMost        = r->rgnBox.left;
    self->rightMost       = r->rgnBox.right;
    self->topMost         = r->rgnBox.top;
    self->bottomMost      = r->rgnBox.bottom;

    // Connect the BWAPI Region and BW Region two ways
    this->regionID  = id;
    r->unk_28       = (u32)this;
  }
  void RegionImpl::UpdateRegionRelations()
  {
    // Assuming this is called via GameInternals, so no checks are made
    BW::region *r = &BW::BWDATA_SAIPathing->regions[this->regionID];

    // Assign region neighbors
    this->accessableNeighbors.clear();
    this->inaccessableNeighbors.clear();
    for ( int n = 0; n < r->neighborCount; ++n )
    {
      BW::region *neighbor = r->getNeighbor((unsigned char)n);

      // Separate region relationships
      if ( r->isConnectedTo( neighbor ) )
        this->accessableNeighbors.insert((BWAPI::Region*)neighbor->unk_28);
      else
        this->inaccessableNeighbors.insert((BWAPI::Region*)neighbor->unk_28);

      // Client compatibility for neighbors
      ++self->neighborCount;
      self->neighbors[n] = neighbor->getIndex();
    }
  }
  RegionImpl::~RegionImpl()
  {
  }
  int RegionImpl::getRegionGroupID() const
  {
    return self->islandID;
  }
  BWAPI::Position RegionImpl::getCenter() const
  {
    return BWAPI::Position(self->center_x, self->center_y);
  }
};
