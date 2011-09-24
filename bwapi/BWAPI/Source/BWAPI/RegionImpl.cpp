#include "RegionImpl.h"

#include <set>
#include <vector>

#include <BW/Offsets.h>
#include <Util/Foreach.h>

#include "../../Debug.h"

namespace BWAPI
{
  RegionImpl::RegionImpl(int id)
  {
    // Set client data
    this->self = &this->data;
    MemZero(this->data);

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
    self->id  = id;
    r->unk_28 = (u32)this;
    
    this->polygon.clear();
    this->closestAccessibleRgn    = NULL;
    this->closestInaccessibleRgn  = NULL;
  }
  void RegionImpl::UpdateRegionRelations()
  {
    // Assuming this is called via GameInternals, so no checks are made
    BW::region *r = &BW::BWDATA_SAIPathing->regions[self->id];

    // Assign region neighbors
    this->neighbors.clear();

    int accessibleBestDist    = 99999;
    int inaccessibleBestDist  = 99999;
    for ( int n = 0; n < r->neighborCount; ++n )
    {
      BW::region *neighbor = r->getNeighbor((unsigned char)n);
      BWAPI::Region *bwapiNeighbor = (BWAPI::Region*)neighbor->unk_28;

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
  void RegionImpl::AddPoint(int x, int y)
  {
    this->polygon.push_back(BWAPI::Position(x,y));
  }
  std::vector<BWAPI::Position> &RegionImpl::getSimplePolygon()
  {
    return this->polygon;
  }
  void RegionImpl::SimplifyPolygon()
  {
    BWAPI::Position first;
    if ( this->polygon.size() < 3 )
      return;

    int type = 0;
    std::vector<BWAPI::Position>::iterator i = this->polygon.begin();
    while ( i != this->polygon.end() )
    {
      if ( i == this->polygon.begin() )
      {
        first = *i;
        ++i;
        continue;
      }
      if ( i + 1 != this->polygon.end() )
      {
        if ( (type == 0 || type == 1) && first.x() == (i+1)->x() && first.x() == i->x() )
        {
          i = this->polygon.erase(i);
          type = 1;
        }
        else if ( (type == 0 || type == 2) && first.y() == (i+1)->y() && first.y() == i->y() )
        {
          i = this->polygon.erase(i);
          type = 2;
        }
        else
        {
          type = 0;
          first = *i;
          ++i;
        }
      }
      else
        break;
    }
  }
  bool RegionImpl::isHigherGround() const
  {
    return self->isHigherGround;
  }
  int RegionImpl::getDefensePriority() const
  {
    return self->priority;
  }
  bool RegionImpl::isWalkable() const
  {
    return self->isWalkable;
  }
  const std::set<BWAPI::Region*> &RegionImpl::getNeighbors() const
  {
    return this->neighbors;
  }
  int RegionImpl::getID() const
  {
    return self->id;
  }
  int RegionImpl::getBoundsLeft() const
  {
    return self->leftMost;
  }
  int RegionImpl::getBoundsTop() const
  {
    return self->topMost;
  }
  int RegionImpl::getBoundsRight() const
  {
    return self->rightMost;
  }
  int RegionImpl::getBoundsBottom() const
  {
    return self->bottomMost;
  }
  RegionData *RegionImpl::getData()
  {
    return self;
  }
  BWAPI::Region *RegionImpl::getClosestAccessibleRegion() const
  {
    return this->closestAccessibleRgn;
  }
  BWAPI::Region *RegionImpl::getClosestInaccessibleRegion() const
  {
    return this->closestInaccessibleRgn;
  }
  int RegionImpl::getDistance(BWAPI::Region *other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }
};
