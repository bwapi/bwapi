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
    Position start;   // 0
    Position next;    // 4
    Position finish;  // 8
    u32 timeStarted;  // 12
    u32 unitID;       // 16
    u32 unitUnknown_0x38_0x3C_copy;   /* Starcraft copies field 0x38 of the unit    // 20
                                         if the unit's flingy movement type is 0,
                                         otherwise it will copy field 0x3C
                                      */
    u8  isCalculating;  // 24   1 if the path is being calculated and not yet ready
    u8  pathDelay;      // 25   Delay before a new path is created
    u8  unk_1A;         // 26   unknown flag?
    u8  pathCount;      // 27   dup of num_areas?
    u8  num_areas;      // 28   Official name
    u8  cur_area;       // 29
    u8  num_segments;   // 30   Official name
    u8  cur_segment;    // 31
    Position steps[1];  // 32   sc allocates 96 bytes here? 
  }; 
  
  // note: u16 areas[num_areas] follow after steps[num_segments]
  // The last entry of the above struct can be viewed as follows:
  // Position steps[num_segments];
  // u16      regions[num_areas];  // region ID for SAI_Paths->regions; typically fills the rest of the struct with region IDs to the destination, and cut off if there isn't enough room

  char *getMoveStateName(u8 bMoveState);
};
#pragma pack()
