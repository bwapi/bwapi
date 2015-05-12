#pragma once
#include <Util/Types.h>

#include "Constants.h"

namespace BW
{
  class  Bitmap;

  // Player resource counts
  struct PlayerResourcesStruct
  {
    s32 minerals[PLAYER_COUNT];
    s32 gas[PLAYER_COUNT];
    s32 cumulativeGas[PLAYER_COUNT];
    s32 cumulativeMinerals[PLAYER_COUNT];
  };

  struct PlayerInfo
  {
    int  dwPlayerID;
    int  dwStormId;
    u8   nType;
    u8   nRace;
    u8   nTeam;
    char szName[25];
  };

  struct bounds
  {
    WORD  left;
    WORD  top;
    WORD  right;
    WORD  bottom;
    WORD  width;
    WORD  height;
  };

  struct layer
  {
    BYTE    buffers;
    BYTE    bits;
    WORD    left;
    WORD    top;
    WORD    width;
    WORD    height;
    WORD    alignment;
    Bitmap  *pSurface;
    void(__stdcall *pUpdate)(Bitmap *pSurface, bounds *pBounds);
  };

  struct unitFinder
  {
    int unitIndex;
    int searchValue;
  };


  template <class _T>
  struct VCListEntry
  {
    VCListEntry<_T> *prev; // First entry points to begin in controller
    VCListEntry<_T> *next; // negative value indicates vector::end and points to &end in controller
    _T container;
  };

  template <class _T>
  struct VCList // sizeof 12
  {
    int unknown_00;
    VCListEntry<_T> *end;
    VCListEntry<_T> *begin;
  };

  struct AllScoresStruct
  {
    s32 allUnitsTotal[PLAYER_COUNT];
    s32 allUnitsProduced[PLAYER_COUNT];
    s32 allUnitsOwned[PLAYER_COUNT];
    s32 allUnitsLost[PLAYER_COUNT];
    s32 allUnitsKilled[PLAYER_COUNT];
    s32 allUnitScore[PLAYER_COUNT];
    s32 allKillScore[PLAYER_COUNT];
    s32 allBuildingsTotal[PLAYER_COUNT];
    s32 allBuildingsConstructed[PLAYER_COUNT];
    s32 allBuildingsOwned[PLAYER_COUNT];
    s32 allBuildingsLost[PLAYER_COUNT];
    s32 allBuildingsRazed[PLAYER_COUNT];
    s32 allBuildingScore[PLAYER_COUNT];
    s32 allRazingScore[PLAYER_COUNT];
    s32 allFactoriesConstructed[PLAYER_COUNT];
    s32 allFactoriesOwned[PLAYER_COUNT];
    s32 allFactoriesLost[PLAYER_COUNT];
    s32 allFactoriesRazed[PLAYER_COUNT];

    // Supply available, used, and maximum for all players and every race
    struct SuppliesPerRace
    {
      s32 available[PLAYER_COUNT];
      s32 used[PLAYER_COUNT];
      s32 max[PLAYER_COUNT];
    };
    SuppliesPerRace supplies[RACE_COUNT];
    s32 customScore[PLAYER_COUNT];

    // Unit counts: all, completed, killed, dead
    struct Counts
    {
      s32 all[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 completed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 killed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 dead[UNIT_TYPE_COUNT][PLAYER_COUNT];
    };
    Counts unitCounts;
  };

  // Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group.
  typedef u16 TileID;


  /// Direct mapping of minitile flags array
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      u16 miniTile[16];
    };
    MiniTileFlagArray tile[0x10000];
  };

  struct activeTile
  {
    u8 bVisibilityFlags;
    u8 bExploredFlags;
    u8 bWalkable : 1; // Set on tiles that can be walked on
    u8 bUnknown1 : 1; // Unused?
    u8 bUnwalkable : 1; // Set on tiles that can't be walked on
    u8 bUnknown2 : 3; // Unused?
    u8 bHasCreep : 1; // Set when creep occupies the area
    u8 bAlwaysUnbuildable : 1; // always unbuildable, like water
    u8 bGroundHeight : 3; // ground height
    u8 bCurrentlyOccupied : 1; // unbuildable but can be made buildable
    u8 bCreepReceeding : 1; // Set when the nearby structure supporting the creep is destroyed
    u8 bCliffEdge : 1; // Set if the tile is a cliff edge
    u8 bTemporaryCreep : 1; // Set when the creep occupying the area was created. Not set if creep tiles were preplaced. Used in drawing routine.
    u8 bUnknown3 : 1; // Unused?
  };

  struct swishTimer
  {
    u16 wIndex;
    u16 wType;
  };
  static_assert(sizeof(swishTimer) == 4, "Expected swishTimer structure to be 4 bytes.");

}
