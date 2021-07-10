#pragma once
#include "../Types.h"

#pragma pack(1)
struct ReplayHeader
{
  bool isBroodwar;
  u32  frameCount;
  u16  campaignID;
  struct _gameSeed
  {
    u8  commandID;
    u32 randSeed;
    u8  playerbytes[8];
  } gameSeed;
  struct _netGame
  {
    u32  unknown_0;
    char currentPlayerName[24];
    u32  gameFlags;
    u16  mapWidth;
    u16  mapHeight;
    u8   activePlayerCount;
    u8   availableSlots;
    u8   gameSpeed;
    u8   gameState;
    u8   gameType;
    u8   unknown_1;
    u16  gameSubtype;
    u32  seed; // unused
    u16  tileset;
    u8   autosaved;
    u8   computerPlayerCount;
    char gameName[25];
    char mapName[32];
    struct _gameTemplate
    {
      u8   gameType;
      u8   unknown;
      u16  subType;
      u16  subTypeDisplay;
      u16  subTypeLabel;
      u8   victoryCondition;
      u8   resourceType;
      bool useStandardUnitStats;
      u8   fogOfWar; // unused
      u8   startingUnits;
      bool useFixedPosition;
      u8   usageRestrictionFlags;
      bool alliesAllowed;
      u8   teams;
      bool cheats;
      u8   tournamentMode;
      u32  victoryConditionVal;
      u32  resourcesVal;
      u32  unusedVal;
      u8   extra;
    } gameTemplateInfo;
  } networkGameHeader;
  struct _playerEntry
  {
    u32  slot;
    u32  stormId;
    u8   type;
    u8   race;
    u8   team;
    char playerName[25];
  } players[12];
  u32 playerColors[8];
  u8  playerForceData[8];
};
#pragma pack()

static_assert(sizeof(ReplayHeader) == 633, "Replay header size is incorrect.");
