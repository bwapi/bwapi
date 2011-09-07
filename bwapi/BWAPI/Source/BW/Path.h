#pragma once
#pragma pack(1)
#include "Position.h"

/* Defines are official */

#define UM_Init          0x00
#define UM_InitSeq       0x01
#define UM_Lump          0x02
#define UM_Turret        0x03
#define UM_Bunker        0x04
#define UM_BldgTurret    0x05
#define UM_Hidden        0x06
#define UM_Flyer         0x07
#define UM_FakeFlyer     0x08
#define UM_AtRest        0x09
#define UM_Dormant       0x0A
#define UM_AtMoveTarget  0x0B
#define UM_CheckIllegal  0x0C
#define UM_MoveToLegal   0x0D
#define UM_LumpWannabe   0x0E
#define UM_FailedPath    0x0F
#define UM_RetryPath     0x10
#define UM_StartPath     0x11
#define UM_UIOrderDelay  0x12
#define UM_TurnAndStart  0x13
#define UM_FaceTarget    0x14
#define UM_NewMoveTarget 0x15
#define UM_AnotherPath   0x16
#define UM_Repath        0x17
#define UM_RepathMovers  0x18
#define UM_FollowPath    0x19
#define UM_ScoutPath     0x1A
#define UM_ScoutFree     0x1B
#define UM_FixCollision  0x1C
#define UM_WaitFree      0x1D
#define UM_GetFree       0x1E
#define UM_SlidePrep     0x1F
#define UM_SlideFree     0x20
#define UM_ForceMoveFree 0x21
#define UM_FixTerrain    0x22
#define UM_TerrainSlide  0x23

namespace BW
{
  struct Path
  {
    /* 0x00 */Position start;   // 0
    /* 0x04 */Position next;    // 4
    /* 0x08 */Position finish;  // 8
    /* 0x0C */u32 timeStarted;  // 12
    /* 0x10 */u32 unitID;       // 16
    /* 0x14 */u32 unitUnknown_0x38_0x3C_copy;   /* Starcraft copies field 0x38 of the unit    // 20
                                                   if the unit's flingy movement type is 0,
                                                   otherwise it will copy field 0x3C
                                                */
    /* 0x18 */u8  isCalculating;  // 24   1 if the path is being calculated and not yet ready
    /* 0x19 */u8  pathDelay;      // 25   Delay before a new path is created
    /* 0x1A */u8  unk_1A;         // 26   unknown flag?
    /* 0x1B */u8  pathCount;      // 27   dup of num_areas?
    /* 0x1C */u8  num_areas;      // 28   Official name, number of regions following the segments
    /* 0x1D */u8  cur_area;       // 29   The current region index
    /* 0x1E */u8  num_segments;   // 30   Official name, number of Position segments
    /* 0x1F */u8  cur_segment;    // 31   The current position segment
    /* 0x20 */Position steps[1];  // 32
  }; 
  
  // note: u16 areas[num_areas] follow after steps[num_segments]
  // The last entry of the above struct can be viewed as follows:
  // Position steps[num_segments];
  // u16      regions[num_areas];  // region ID for SAI_Paths->regions; typically fills the rest of the struct with region IDs to the destination, and cut off if there isn't enough room

  class Unit;
  struct pathCreate
  {
    /* 0x000 */BW::Unit *pUnit;
    /* 0x004 */Position src;
    /* 0x008 */Position dst;
    /* 0x00C */DWORD    dw__0C;
    /* 0x010 */DWORD    dw__10;
    /* 0x014 */DWORD    dw__14;
    /* 0x018 */DWORD    dw__18;
    /* 0x01C */BYTE     b__1C;
    /* 0x01D */BYTE     b__1D;
    /* 0x01E */BYTE     b__1E;
    /* 0x01F */BYTE     b__1F;
    /* 0x020 */WORD     wAreas[50];
    /* 0x084 */WORD     cur_area;
    /* 0x086 */WORD     num_areas;
    /* 0x088 */Position segments[50];
    /* 0x150 */WORD     cur_segment;
    /* 0x152 */WORD     num_segments;
    /* 0x154 */BYTE     b__154;
    /* 0x155 */BYTE     b__155;
    /* 0x156 */BYTE     b__156;
    /* 0x157 */BYTE     b__157;
    /* 0x158 */Position boundsTopLeft;
    /* 0x15C */Position boundsBotRight;
    /* 0x160 */WORD     w__160;
    /* 0x162 */WORD     w__162;
    /* 0x164 */WORD     w__164;
    /* 0x166 */WORD     w__166;
    /* 0x168 */WORD     w__168;
    /* 0x16A */WORD     w__16A;
  };

  char *getMoveStateName(u8 bMoveState);
};
#pragma pack()
