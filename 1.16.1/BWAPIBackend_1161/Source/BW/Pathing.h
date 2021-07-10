#pragma once
#include "Position.h"
#include "../Types.h"
#include <vector>

#pragma pack(1)

#define getRegionFromId(x) (&(BW::BWDATA::SAIPathing->regions[(x)&0x1FFF]))

namespace BW
{
  struct pathPoint
  {
    u16 x;
    u16 y;
  };

  struct pathBox
  {
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
  };

  struct pathRect
  {
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
  };

  // cppcheck-suppress noConstructor
  class region
  {
  public:
    bool                  isConnectedTo(region *target) const;
    bool                  isConnectedTo(u16 index) const;
    std::vector<region*>  &getAccessibleNeighbours() const;
    int                   getAirDistance(region *dst) const;
    region                *getNeighbor(u8 index) const;
    Position              getCenter() const;
    u16                   getIndex() const;

    /*0x00*/u16       accessabilityFlags;
                // 0x1FF9 = High ground    1001
                // 0x1FFB = Low ground     1011
                // 0x1FFD = Inaccessible   1101
    /*0x02*/u16       groupIndex; // Identifies connected regions
    /*0x04*/u16       tileCount;
    /*0x06*/u8        pathCount;
    /*0x07*/u8        neighborCount;
    /*0x08*/union
            {
              void  *node;      // pointer to associated path node structure in path generation
              int   relation;   // used in string-pulling/area-fixup routines
            } user;             // Data in user is cleared after its use in a subroutine
    /*0x0C*/u16       *neighbors; // allocated array of IDs for neighbors
    /*0x10*/u32       rgnCenterX; // must >> 8; in pixels
    /*0x14*/u32       rgnCenterY; // must >> 8; in pixels
    /*0x18*/pathRect  rgnBox; // in pixels
    /*0x20*/u8        defencePriority;    /*
                                            0x001 = Key Point
                                            0x002 = Choke Point/Corridor
                                          */
    /*0x21*/u8        neighborProperty; // 0x01 for map bottom, subtracted from neighborCount in some place
  private:
    /*0x22*/u16       unk_22;
    /*0x24*/u32       unk_24;
    /*0x28*/u32       unk_28;
    /*0x2C*/u16       localBuffer[10]; // local array of IDs for neighbors
  };

  static_assert( sizeof(region) == 64, "BW::Region is incorrect." );

  /* Contour IDs and values:
     0: BOTTOM: y1, x1, x2
     1: LEFT;   x1, y1, y2
     2: TOP;    y1, x1, x2
     3: RIGHT:  x1, y1, y2

    In Starcraft Beta they are labelled opposite: TOP, RIGHT, BOTTOM, LEFT
    Additional Beta Notes:

    Involving a 16-byte const array:
      LEFT_TURN  = true;
      RIGHT_TURN = false;
  */
  struct contour
  {
    s16 v[3];
    u8  type;
    u8  unk_relation;   // Relation to neighboring contours. The highest 2 bits are unused.
                        // bitwise:   YY XXXX
                        // Where YY is the neighboring contours direction; 
                        //    1 = in, 0 = out; One bit for each neighbor
                        // And XXXX is a unique ID for the combination, used for an unknown array.
  };

  struct contourHub
  {
    contour *contours[4];
    u16     contourCount[4];
    u16     contourMax[4];
    s16     searchInner[4];
    s16     unk_28[4];
    s16     searchOuter[4];
  };

  struct split
  {
    u16 minitileMask; // 16 minitiles per tile corresponds to 16 bits;
    u16 rgn1;         // region with unset bits
    u16 rgn2;         // region with set   bits
  };

  struct SAI_Paths
  {
    u32         regionCount;
    void        *globalBuffer_ptr;
    void        *splitTiles_end;
    u16         mapTileRegionId[256][256];   // rgnId is &0x1FFF; split = &0xE000 >>1
    split       splitTiles[25000];     // 0x2000C
    region      regions[5000];         // 0x449FC
    u16         globalBuffer[10000];   // 0x92BFC; extra buffer used for large neighbor ID arrays
    contourHub  *contours;             // 0x97A1C
  };

  const BW::region *getRegionAt(int x, int y);
  const BW::region *getRegionAt(Position pos);
}
#pragma pack()
