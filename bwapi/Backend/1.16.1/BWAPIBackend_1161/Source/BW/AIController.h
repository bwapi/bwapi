#pragma once
#include <Util/Types.h>

#include "Position.h"
#include "CUnit.h"

#pragma pack(1)

namespace BW
{
  // forwards
  class CUnit;

  class Captain;
  class Town;
  class CAIController;

  class Captain
  {
  public:
    /* 0x00 */ WORD  wRegionId;
    /* 0x02 */ WORD  wRegionId2_unk;
    /* 0x04 */ BYTE bPlayerId;
    /* 0x05 */ BYTE bCaptainType;
      /*  0  - none
          1  - military (attack group)
          2  - military (attack group)
          3
          4  - implode/help_iftrouble (aiscript)
          5  - guard
          6  - campaign guard
          7
          8  - military (attack group, clear)
          9  - military (attack group)
      */
    /* 0x06 */ BYTE bUnk_06;  // priority?
    /* 0x07 */ BYTE bUnk_07;  // similar to 0x06?? Some counter down?
    /* 0x08 */ BYTE bCaptainFlags;
      /*  
          0x01 - want build something?
          0x02 - build bunkers
          0x10 - build turrets
          0x40 - No Campaign
      */
    /* 0x09 */ BYTE bUnk_0x09;
    /* 0x0A */ WORD wUnk_0x0A;
    /* 0x0C */ WORD wUnk_0x0C;    // default_min, default_min * 6, or 1500, compared with max_force
    /* 0x0E */ WORD wUnk_0x0E;    // default_min, default_min * 6, or 1000, compared with max_force
    /* 0x10 */ WORD wUnk_0x10;    // region ground strength
    /* 0x12 */ WORD wUnk_0x12;    // region air strength
    /* 0x14 */ WORD wUnk_0x14;    // full ground strength
    /* 0x16 */ WORD wUnk_0x16;    // full air strength
    /* 0x18 */ WORD  wUnk_0x18;
    /* 0x1A */ WORD  wUnk_0x1A;
    /* 0x1C */ CUnit *unk_unit_0x1C; // unknown unit, expected primary order attack target?
    /* 0x20 */ CUnit *unk_unit_0x20; // unknown unit, related to 0x1C
    /* 0x24 */ CUnit *slowestUnit;    // slowest unit
    /* 0x28 */ CUnit *followTarget;    // target to follow
    /* 0x2C */ DWORD dwUnk_0x2C;
    /* 0x30 */ DWORD dwUnk_0x30;    // Town*
  };

  /* NOTES:  BYTE rgnData[2500];
          // 6 = neighbor of high-priority rgn contains nothing
          // 5 = contains own structures, or units if no completed structures owned
          // 4 = neighbor of high-priority enemy rgn contains nothing
          // 3 = neighbor of high-priority rgn containing enemies was acknowledged
          // 2 = neighbor of high-priority rgn contains enemies
          // 1 = contains visible enemy unit

  */

  template <class T>
  struct Pool
  {
    T *first;
    T *last;
  };

  const int MAX_BUILD_LIST = 100;

  struct townQueueEntry
  {
    unsigned char flags;
    unsigned char priority;
    unsigned short type;
  };

  struct Town
  {
    Town *next;
    Town *prev;
    Pool<void*> workerPool;
    Pool<void*> taskPool;
    u8  player;
    u8  isInitialized;
    u8  desiredWorkers;
    u8  peons;
    u8  baseId;
    u8  isFunctional;
    u8  _unk1;
    u8  _unk2;
    Position position;
    CUnit *town_center;
    CUnit *unk;
    CUnit *resource;
    CUnit *gas[3];
    townQueueEntry townQueue[MAX_BUILD_LIST];
  };

  enum ControllerType : u8
  {
    AI_NONE = 0,
    AI_GUARD,
    AI_PEON,
    AI_BLDG,
    AI_SOLDIER
  };

/*
TYPE_CAPTAIN  1
TYPE_GUARD    2
TYPE_BLDG     3
TYPE_PEON     4
TYPE_ADDON    7
TYPE_SPECIAL  8
*/

/*
DO_TECH   2

*/

  class CAIController
  {
  public:
    /* 0x00 */ CAIController *prev;
    /* 0x04 */ CAIController *next;
    
    /* 0x08 */ ControllerType bType;
    /*    guard = 1,
          worker = 2,
          task = 3,

          unknown (additional structure) = 4
    */
    union
    {
      struct
      {
        /* 0x09 */ BYTE counter;
        /* 0x0A */ BYTE unk_0A;
        /* 0x0B */ BYTE unk_0B;
        /* 0x0C */ CUnit *pUnit;
        /* 0x10 */ WORD unitType;
        /* 0x12 */ struct { short x,y; } desiredLocation;
        /* 0x16 */ struct { short x,y; } currentLocation;
        /* 0x18 */ WORD unk_18;
        /* 0x1A */ DWORD dwCount;
      } guard;  // 01

      // Used for workers
      struct
      {
        /* 0x09 */ BYTE   bWorkerTask;  //  1 = harvest minerals
                                        //  2 = harvest gas#2
                                        //  3 = harvest gas #3
                                        //  default = harvest gas #1
        /* 0x0A */ BYTE   unk_0A;
        /* 0x0B */ BYTE   bWaitTimer;  // wait timer before using this unit
        /* 0x0C */ DWORD  dwLastUpdateTime;
        /* 0x10 */ DWORD  unk_10;
        /* 0x14 */ Town   *pTown; // ptr to town struct
      } worker; // 02

      struct 
      {
        /* 0x09 */ BYTE unitQueue[5];
        /* 0x0E */ WORD unk_0E;
        /* 0x10 */ DWORD unk_10;
        /* 0x14 */ DWORD unkCurrentQueue;
        /* 0x18 */ DWORD unkQueue[5];
      } task; // ID: 03

      struct
      {
        /* 0x09 */ BYTE    unk_09;
        /* 0x0A */ BYTE    unk_0A;
        /* 0x0B */ BYTE    unk_0B;
        /* 0x0C */ CUnit    *pUnit;
        /* 0x10 */ Captain *captain;
      } type4;
    } u;
  };
}

#pragma pack()
