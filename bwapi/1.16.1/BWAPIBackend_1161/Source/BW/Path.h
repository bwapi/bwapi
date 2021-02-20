#pragma once
#include "Position.h"

#pragma pack(1)


namespace BW
{
  enum UnitMovementState : u8
  {
    // Names are official
    UM_Init          = 0x00,
    UM_InitSeq       = 0x01,
    UM_Lump          = 0x02,
    UM_Turret        = 0x03,
    UM_Bunker        = 0x04,
    UM_BldgTurret    = 0x05,
    UM_Hidden        = 0x06,
    UM_Flyer         = 0x07,
    UM_FakeFlyer     = 0x08,
    UM_AtRest        = 0x09,
    UM_Dormant       = 0x0A,
    UM_AtMoveTarget  = 0x0B,
    UM_CheckIllegal  = 0x0C,
    UM_MoveToLegal   = 0x0D,
    UM_LumpWannabe   = 0x0E,
    UM_FailedPath    = 0x0F,
    UM_RetryPath     = 0x10,
    UM_StartPath     = 0x11,
    UM_UIOrderDelay  = 0x12,
    UM_TurnAndStart  = 0x13,
    UM_FaceTarget    = 0x14,
    UM_NewMoveTarget = 0x15,
    UM_AnotherPath   = 0x16,
    UM_Repath        = 0x17,
    UM_RepathMovers  = 0x18,
    UM_FollowPath    = 0x19,
    UM_ScoutPath     = 0x1A,
    UM_ScoutFree     = 0x1B,
    UM_FixCollision  = 0x1C,
    UM_WaitFree      = 0x1D,
    UM_GetFree       = 0x1E,
    UM_SlidePrep     = 0x1F,
    UM_SlideFree     = 0x20,
    UM_ForceMoveFree = 0x21,
    UM_FixTerrain    = 0x22,
    UM_TerrainSlide  = 0x23
  };
  struct Path
  {
    /*0x00*/ Position start;   // 0
    /*0x04*/ Position next;    // 4
    /*0x08*/ Position finish;  // 8
    /*0x0C*/ u32 timeStarted;  // 12
    /*0x10*/ u32 unitID;       // 16
    /*0x14*/ u32 unitUnknown_0x38_0x3C_copy;   /* Starcraft copies field 0x38 of the unit    // 20
                                                  if the unit's flingy movement type is 0,
                                                  otherwise it will copy field 0x3C
                                               */
    /*0x18*/ u8  isCalculating;  // 24   1 if the path is being calculated and not yet ready
    /*0x19*/ u8  pathDelay;      // 25   Delay before a new path is created
    /*0x1A*/ u8  unk_1A;         // 26   unknown flag?
    /*0x1B*/ u8  pathCount;      // 27   dup of num_areas?
    /*0x1C*/ u8  num_areas;      // 28   Official name, number of regions following the segments
    /*0x1D*/ u8  cur_area;       // 29   The current region index
    /*0x1E*/ u8  num_segments;   // 30   Official name, number of Position segments
    /*0x1F*/ u8  cur_segment;    // 31   The current position segment
    /*0x20*/ Position steps[1];  // 32
  }; 
  
  // note: u16 areas[num_areas] follow after steps[num_segments]
  // The last entry of the above struct can be viewed as follows:
  // Position steps[num_segments];
  // u16      regions[num_areas];  // region ID for SAI_Paths->regions; typically fills the rest of the struct with region IDs to the destination, and cut off if there isn't enough room

  class CUnit;
  struct pathCreate
  {
    /* 0x000 */BW::CUnit *pUnit;
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

  const char *getMoveStateName(u8 bMoveState);
};
#pragma pack()
