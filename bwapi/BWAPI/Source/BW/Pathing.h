#pragma once
#pragma pack(1)
#include "Position.h"
#include <Util/Types.h>


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

  struct unkPath4
  {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8  unk_06;
    u8  unk_07;
    u32 unk_08;
    u32 unk_0C;
    u32 x;
    u32 y;
  };

  struct unkPath3
  {
    u8       unk_00;
    u8       unk_01;
    u16      unk_02;
    unkPath4 *unk_04;
  };

  struct region
  {
    u16       accessabilityFlags;
              // 0x1FF9 = High ground    1001
              // 0x1FFB = Low ground     1011
              // 0x1FFD = Inaccessable   1101
    u16       groupIndex; // Identifies connected regions
    u16       tileCount;
    u8        unk_6;
    u8        neighborCount;
    unkPath3  *unk_8;
    WORD      *neighborsLg; // allocated array of IDs for neighbors
    u32       rgnCenterX; // must >> 8; in pixels
    u32       rgnCenterY; // must >> 8; in pixels
    pathRect  rgnBox; // in pixels
    u32       unk_20;
    u32       unk_24;
    u32       unk_28;
    u16       neighborsSm[10]; // local array of IDs for neighbors
  };

  // Contour IDs and values:
  // 0: BOTTOM: y1, x1, x2, ?
  // 1: LEFT;   x1, y1, y2, ?
  // 2: TOP;    y1, x1, x2, ?
  // 3: RIGHT:  x1, y1, y2, ?

  struct contour
  {
    pathBox *pathList[4];
    u16     pathCount[4];
    u16     pathMax[4];
    u16     unk_20[4];
    u16     unk_28[4];
    u16     unk_30[4];
  };

  struct SAI_Paths
  {
    u32       regionCount;
    u32       unk_92BFC_offset;
    u32       unk_2000C_offset;
    u16       mapTileRegionId[256][256];
    u8        unk_2000C[150000];
    region    regions[5000];
    u8        unk_92BFC[20000];
    contour   *contours; // 0x97A1C
  };
}

#pragma pack()