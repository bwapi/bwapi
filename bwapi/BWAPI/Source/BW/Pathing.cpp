#include <set>
#include <list>
#include <bitset>
#include <vector>

#include "util/foreach.h"

#include "Pathing.h"
#include "Offsets.h"

#include "../../Debug.h"

#define BOTTOM 0
#define LEFT 1
#define TOP 2
#define RIGHT 3

namespace BW
{
  bool region::isConnectedTo(region *target)
  {
    if ( target )
      return this->groupIndex == target->groupIndex;
    return false;
  }

  bool region::isConnectedTo(u16 index)
  {
    if ( index < BW::BWDATA_SAIPathing->regionCount )
      return this->groupIndex == getRegionFromId(index)->groupIndex;
    return false;
  }

  std::vector<region*> &region::getAccessibleNeighbours()
  {
    static std::vector<region*> neighbors;
    neighbors.clear();
    for ( u8 i = 0; i < this->neighborCount; ++i )
    {
      region *neighbor = this->getNeighbor(i);
      if ( this->isConnectedTo( neighbor ) )
        neighbors.push_back(neighbor);
    }
    return neighbors;
  }

  int region::getAirDistance(region *dst)
  {
    return dst->getCenter().getApproxDistance(this->getCenter());
  }

  region *region::getNeighbor(u8 index)
  {
    if ( index <= this->neighborCount )
      return getRegionFromId(this->neighbors[index]);
    return NULL;
  }

  Position region::getCenter()
  {
    return BW::Position((u16)(this->rgnCenterX >> 8), (u16)(this->rgnCenterY >> 8));
  }

  u16 region::getIndex()
  {
    return (u16)( ((u32)this - (u32)(&BW::BWDATA_SAIPathing->regions)) / sizeof(BW::region) );
  }

/*
   bool isContourSpaceAvailable(contourBase *this, int x, int y)
    {
        // worry about declarations later
        
      int v3 = this->contourCount[BOTTOM]; //upperBound
      int lowerBound = 0;
      int height = 2 * y - 1;
      // Binary search to find smallest y coordinate where
      // height<(signed __int16)(2 * this->contours[BOTTOM][midpoint].v[0])
      while ( lowerBound < v3)
      {
        //average of v3 and lowerBound
        int midpoint = (v3 + lowerBound) /2;
        if ( height>= 2 * this->contours[BOTTOM][midpoint].v[0] )
          lowerBound = midpoint + 1;
        else
          v3 = midpoint;
      }
      //v3 is smallest value where height<2 * this->contours[BOTTOM][v3].v[0]
      
      // The following for loop returns false if it finds a value for v7 where
      //   this->searchInner[LEFT] + v7->v[1] <= x <= this->searchInner[RIGHT] + v7->v[2]
      
      for( v7 = v3 - 1; this->contours[BOTTOM][v7].v[0] + this->searchInner[BOTTOM] >= y && v7 >= 0; --v7)
      {
        if ( this->searchInner[LEFT] + this->contours[BOTTOM][v7].v[1] <= x
           &&  x <= this->searchInner[RIGHT] + this->contours[BOTTOM][v7].v[2])
          return false;
      }
      
      lowerBound = 0;
      width= 2 * x + 1;
      v9 = this->contourCount[LEFT];
      // Binary search to find the smallest x coordinate where
      // width < 2 * this->contours[LEFT][midpoint].v[0]
      while ( lowerBound < v9 )
      {
        //average of v9 and lowerBound
        int midpoint = (v9 + lowerBound) /2;
        if (width >= 2 * this->contours[LEFT][midpoint].v[0] )
          lowerBound = midpoint + 1;
        else
          v9 = midpoint;
      }
      //v9 is smallest x coordinate where width < 2*this->contours[LEFT][midpoint].v[0]
      // The following for loop returns false if it finds a value for v12 where
      // (this->searchInner[TOP] + v12->v[1]) <=y <= (this->searchInner[BOTTOM] + v12->v[2])
      for( int v12 = v9; this->contours[LEFT][v12].v[0] + this->searchInner[LEFT] <= x && v12 < this->contourCount[LEFT]; ++v12)
      {
        if ( this->searchInner[TOP] + this->contours[LEFT][v12].v[1] <= y
          && y <= this->searchInner[BOTTOM] + this->contours[LEFT][v12].v[2] )
          return false;
      }
      lowerBound = 0;
      height = 2 * y + 1;
      v15 = this->contourCount[TOP];
      //binary search to find the smallest y coordiante where
      //height < 2 * this->contours[TOP][midpoint].v[0]
      while ( lowerBound < v15 )
      {
        //average of v15 and lowerBound 
        int midpoint = (v15 + lowerBound ) /2;
        if ( height >= 2 * this->contours[TOP][midpoint].v[0] )
          lowerBound = midpoint + 1;
        else
          v15 = midpoint;
      }
      // The following for loop returns false if it finds a value for v18 where
      //  (this->searchInner[LEFT] + v18->v[1]) <=x <=(this->searchInner[RIGHT] + v18->v[2]) + v12->v[2])
      for ( int v18 = v15; (this->contours[TOP][v18].v[0] + this->searchInner[TOP]) <= y && v18 < this->contourCount[TOP]; ++v18)
      {
        if ( this->searchInner[LEFT] + this->contours[TOP][v18].v[1] <= x
          && x<= this->searchInner[RIGHT] + this->contours[TOP][v18].v[2] )
          return false;
      }
      v21 = this->contourCount[RIGHT];
      lowerBound = 0;
      width = 2 * x - 1;
      // binary search to find the smallest x coordinate where
      // width < (signed __int16)(2 * this->contours[RIGHT][midpoint].v[0])
      while ( lowerBound < v21 )
      {
        //average of v21 and lowerBound 
        int midpoint = (v21 + lowerBound ) /2;
        if (width >= 2 * this->contours[RIGHT][midpoint].v[0] )
          lowerBound = midpoint + 1;
        else
          v21 = midpoint;
      }
     
      // The following for loop returns false if it finds a value for v24 where
      // this->searchInner[TOP] + v24->v[1] <=  y<= this->searchInner[BOTTOM] + v24->v[2]
      
      for( int v24 =  v21 - 1; this->contours[RIGHT][v24].v[0] + this->searchInner[RIGHT] >= x &&  v24 >= 0; --v24)
      {
          if ( this->searchInner[TOP] + this->contours[RIGHT][v24].v[1] <= y
             && y<= this->searchInner[BOTTOM] + this->contours[RIGHT][v24].v[2] )
              return false;
      }
      return true;
    }
*/

};

