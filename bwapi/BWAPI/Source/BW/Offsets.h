#pragma once
#pragma pack(1)

#include <windows.h>
#include <Util/Types.h>
#include "../Storm/storm.h"

#include "TriggerEngine.h"
#include "Position.h"

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */

#define PLAYER_COUNT             12
#define PLAYABLE_PLAYER_COUNT     8
#define RACE_COUNT                3
#define UNIT_TYPE_COUNT         228
#define TECH_TYPE_COUNT          44
#define UPGRADE_TYPE_COUNT       61
#define WEAPON_TYPE_COUNT       130
#define DAMAGE_TYPE_COUNT         5
#define EXPLOSION_TYPE_COUNT     25
#define FLINGY_TYPE_COUNT       209
#define UNIT_ARRAY_MAX_LENGTH  1700
#define BULLET_ARRAY_MAX_LENGTH 100
#define MAX_SEARCH             3400

namespace BW
{
  class  Unit;
  class  Sprite;
  class  Image;
  struct Bullet;
  class  TileType;
  class  dialog;
  struct bitmap;
  struct dlgEvent;
  struct fntHead;
  struct SAI_Paths;

  struct DatLoad
  {
    u32   address;
    u32   length;
    u32   entries;
  };

// Define a value for mac version
#ifdef _MAC
#define BWMAC 1
#else
#define BWMAC 0
#endif

// Used for defining BW data without having to write redundant code
#define BWDATA(type,name,pc,mac) static type const BWDATA_##name = BWMAC ? (type)mac : (type)pc

  /*
  //----------------------------------------------- DATA TABLES ----------------------------------------------
#ifdef _MAC
  static const DatLoad * const upgradesDat = (const DatLoad*)0x0015AFCC;  // 1.16.1
  static const DatLoad * const techdataDat = (const DatLoad*)0x0015A6F4;
  static const DatLoad * const weaponsDat  = (const DatLoad*)0x0015C19C;
  static const DatLoad * const unitsDat    = (const DatLoad*)0x0015AD38;
  static const DatLoad * const flingyDat   = (const DatLoad*)0x0014BC08;
#else
  static const DatLoad * const upgradesDat = (const DatLoad*)0x005136E0;  // 1.15.3, 1.16, 1.16.1
  static const DatLoad * const techdataDat = (const DatLoad*)0x005137D8;
  static const DatLoad * const weaponsDat  = (const DatLoad*)0x00513868;
  static const DatLoad * const unitsDat    = (const DatLoad*)0x00513C30;
  static const DatLoad * const flingyDat   = (const DatLoad*)0x00515A38;
#endif
  */
  //----------------------------------------------- PLAYER DATA ----------------------------------------------
  /** Player resource counts */
  struct PlayerResources
  {
    s32 minerals[PLAYER_COUNT];
    s32 gas[PLAYER_COUNT];
    s32 cumulativeGas[PLAYER_COUNT];
    s32 cumulativeMinerals[PLAYER_COUNT];
  };
  BWDATA(PlayerResources*, PlayerResources, 0x0057F0F0, 0);

  BWDATA(u8*, PlayerVictory, 0x00650974, 0);
  BWDATA(BW::Position*, startPositions, 0x0058D720, 0);
  
  /** Force Names */
  struct ForceName
  {
    char  name[30];
  };
  BWDATA(ForceName*, ForceNames, 0x0058D5BC, 0);

  /** Player Information */
  struct PlayerInfo
  {
    int  dwPlayerID;
    int  dwStormId;
    u8   nType;
    u8   nRace;
    u8   nTeam;
    char szName[25];
  };
  BWDATA(PlayerInfo*, Players, 0x0057EEE0, 0);

  BWDATA(u32*, PlayerDownloadStatus, 0x0068F4FC, 0);

  /** Player Alliances */
  struct PlayerAlliance
  {
    u8 player[PLAYER_COUNT];
  };
  BWDATA(PlayerAlliance*, Alliance, 0x0058D634, 0);
  BWDATA(u32*, PlayerVision, 0x0057F1EC, 0);

  // Replay Vision
  BWDATA(u32*, ReplayVision, 0x006D0F18, 0);
  BWDATA(u32*, ReplayRevealAll, 0x006D0F1C, 0);

  /** Code Patches */
  BWDATA(u32, ServerMenuIn, 0x004DD5A2, 0);
  BWDATA(u32, ServerMenuOut, 0x004DD5C9, 0);
  BWDATA(u32, OpponentStartHack, 0x004B995D, 0);
  BWDATA(u32, SingleSpeedHack, 0x004D99FB, 0);

  struct swishTimer
  {
    WORD wIndex;
    WORD wType;
  };
  BWDATA(swishTimer*, commonSwishControllers, 0x005129EC, 0);   // count: 43
  BWDATA(swishTimer*, gluCustmSwishController, 0x0051A9F0, 0);   // count: 5
  BWDATA(swishTimer*, gluCmpgnSwishController, 0x00512B10, 0);   // count: 2
  BWDATA(swishTimer*, gluScoreSwishController, 0x0051A844, 0);   // count: 1
  BWDATA(swishTimer*, gluChatSwishController, 0x0051A490, 0);  // count: 5

  BWDATA(u32*, playerStatusArray, 0x0057F0B8, 0);

  // QueueGameCommand data
  BWDATA(u32*, sgdwBytesInCmdQueue, 0x00654AA0, 0);
  BWDATA(u8*, TurnBuffer, 0x00654880, 0);
  static void  (* const BWFXN_sendTurn)()          = (void(*)()) 0x00485A40;
  const u32 BWFXN_QueueCommand = 0x00485BD0;

  /** Speed & Latency */
  static const u32     OriginalSpeedModifiers[7]  =        { 167, 111, 83, 67, 56, 48, 42};
  BWDATA(u32*, GameSpeedModifiers, 0x005124D8, 0x0014BF3C);
  BWDATA(u32*, GameSpeed, 0x006CDFD4, 0x00228458);

  BWDATA(u32*, LatencyFrames, 0x0051CE70, 0);
  BWDATA(u32*, FramesUntilNextTurn, 0x0051CEA0, 0);
  BWDATA(int*, FrameSkip, 0x005124D4, 0);
  BWDATA(u8*, Latency, 0x006556e4, 0);
  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  /** Dialog and drawing offsets */
  static bool (__fastcall ** const BWDATA_GenericDlgInteractFxns)(dialog*,dlgEvent*)   = (bool (__fastcall**)(dialog*,dlgEvent*))    0x005014AC;
  static void (__fastcall ** const BWDATA_GenericDlgUpdateFxns)(dialog*,int,int,rect*) = (void (__fastcall**)(dialog*,int,int,rect*))0x00501504;

  BWDATA(DWORD*, LastControlID, 0x006D051C, 0);
  BWDATA(dialog**, DialogList, 0x006D5E34, 0);
  BWDATA(fntHead**, FontBase, 0x006CE0F4, 0);
  BWDATA(bitmap*, GameScreenBuffer, 0x006CEFF0, 0);
  BWDATA(bitmap*, GameScreenConsole, 0x00597240, 0);
  
  BWDATA(PALETTEENTRY*, GamePalette, 0x006CE320, 0);
  BWDATA(LPDIRECTDRAWSURFACE*, PrimarySurface, 0x006D5E00, 0);
  BWDATA(LPDIRECTDRAW*, DDInterface, 0x006D5E08, 0);
  BWDATA(LPDIRECTDRAWPALETTE*, PrimaryPalette, 0x006D5E0C, 0);
  BWDATA(LPDIRECTDRAWSURFACE*, BackSurface, 0x006D5E10, 0);

  static void (__cdecl * const BWFXN_DDrawDestroy)()   = (void(__cdecl*)())0x0041D8B0;
  BWDATA(u32, DDrawInitCallPatch, 0x004DB0A2, 0);
  static void (__cdecl * const BWFXN_UpdateBltMasks)() = (void(__cdecl*)())0x0041D470;

  BWDATA(u8*, RefreshRegions, 0x006CEFF8, 0);
  BWDATA(u8*, PlayerColors, 0x00581DD6, 0);

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
    bitmap  *pSurface;
    void (__stdcall *pUpdate)(bitmap *pSurface, bounds *pBounds);
  };
  BWDATA(layer*, ScreenLayers, 0x006CEF50, 0);
  extern void (__stdcall *pOldDrawGameProc)(BW::bitmap *pSurface, BW::bounds *pBounds);
  extern void (__stdcall *pOldDrawDialogProc)(BW::bitmap *pSurface, BW::bounds *pBounds);

  BWDATA(RECT*, ScrLimit, 0x0051A15C, 0);
  BWDATA(RECT*, ScrSize, 0x0051A16C, 0);

  //------------------------------------------- CLIST DATA ---------------------------------------------------
  BWDATA(Unit**, UnitNodeList_VisibleUnit_First, 0x00628430, 0);
  BWDATA(Unit**, UnitNodeList_HiddenUnit_First, 0x006283EC, 0);
  BWDATA(Unit**, UnitNodeList_ScannerSweep_First, 0x006283F4, 0);
  BWDATA(Unit*, UnitNodeTable, 0x0059CCA8, 0);

  BWDATA(Bullet**, BulletNodeTable_FirstElement, 0x0064DEC4, 0);
  BWDATA(Bullet*, BulletNodeTable, 0x0064B2E8, 0);

  struct unitFinder
  {
    int unitIndex;
    int searchValue;
  };
  // array size 3400 (1700 * 2) for each
  BWDATA(unitFinder*, UnitOrderingX, 0x0066FF78, 0);
  BWDATA(unitFinder*, UnitOrderingY, &BWDATA_UnitOrderingX[MAX_SEARCH], 0);

  //------------------------------------------- DATA LEVEL ---------------------------------------------------
  /* Mode Stuff */
  BWDATA(u8*, gameType, 0x00596820, 0);
  BWDATA(u32*, InGame, 0x006556E0, 0);
  BWDATA(u32*, InReplay, 0x006D0F14, 0);
  BWDATA(int*, NetMode, 0x0059688C, 0);
  BWDATA(u32*, CountdownTimer, 0x0058D6F4, 0);  // Countdown Timer (in seconds)
  BWDATA(u32*, ElapsedTime, 0x0058D6F8, 0); // Elapsed Game Time (in seconds)

  // note: unaligned because it's part of a structure that we're not using
  BWDATA(u32*, ReplayFrames, 0x006D0F31, 0);

  BWDATA(u8*, GameState, 0x006D11EC, 0);
  BWDATA(u16*, gwNextGameMode, 0x0051CE90, 0);
  BWDATA(u16*, gwGameMode, 0x00596904, 0);
  BWDATA(u32*, glGluesMode, 0x006D11BC, 0);

  BWDATA(int*, g_LocalHumanID, 0x00512688, 0);

  BWDATA(void**, customList_UIDlgData, 0x0051A350, 0);
  //--------------------------------------- FOR RESOLUTION HACK ----------------------------------------------
  BWDATA(BW::Sprite**, spriteGroups, 0x00629688, 0);

  struct _gametext
  {
    char txt[218];
  };
  BWDATA(_gametext*, Chat_GameText, 0x00640B60, 0);
  BWDATA(u8*, Chat_NextLine, 0x00640B58, 0);
  BWDATA(u8*, Chat_ColorBytes, 0x00641674, 0);
  BWDATA(u32*, Chat_IncrementY, 0x00640B20, 0);

  static void (__cdecl * const BWFXN_drawDragSelBox)()           = (void (__cdecl*)()) 0x00470040;
  static void (__cdecl * const BWFXN_drawAllThingys)()           = (void (__cdecl*)()) 0x00488180;
  static void (__cdecl * const BWFXN_drawMapTiles)()             = (void (__cdecl*)()) 0x0049C780;
  static void (__cdecl * const BWFXN_blitMapTiles)()             = (void (__cdecl*)()) 0x0040C253;
  static void (__cdecl * const BWFXN_drawAllSprites)()           = (void (__cdecl*)()) 0x00498D40;
  //static void (__cdecl *BWFXN_updateImageData)()          = (void (__cdecl*)()) 0x00498CF0;


  static void (__fastcall ** const BWDATA_InputProcs)(dlgEvent*) = (void (__fastcall**)(dlgEvent*))0x005968A0;
  BWDATA(dialog**, EventDialogs, 0x006D5E40, 0);

  BWDATA(u8*, InputFlags, 0x006CDDC0, 0);

  //----------------------------------------- FUNCTION LEVEL -------------------------------------------------
  BWDATA(Unit**, ClientSelectionGroup, 0x00597208, 0);
  BWDATA(u8*, ClientSelectionCount, 0x0059723D, 0);
  BWDATA(u32, PlayerSelection, 0x006284E0, 0);

  BWDATA(u32*, isGamePaused, 0x006509C4, 0);
  static const u32 BWFXN_P_IsGamePaused           = 0x004D974E;

  // These 2 can be merged;
  // void __usercall refundBuildingCost(unsigned __int16 unitType<ax>, unsigned __int8 playerID<cl>)
  // Function takes cost of unit type, multiplies it by 3/4, and adds it to the player's resources.
  static const u32 BWFXN_RefundMinerals                 = 0x0042ce97;//building cancel
  static const u32 BWFXN_RefundMineralsReturnAddress    = 0x0042ce9e;//building cancel
  static const u32 BWFXN_RefundGas                      = 0x0042ceb8;//building cancel
  static const u32 BWFXN_RefundGasReturnAddress         = 0x0042cebe;//building cancel

  static const u32 BWFXN_RefundMinAndGas2               = 0x0042ced9;//morphing
  static const u32 BWFXN_RefundMinAndGas2ReturnAddress  = 0x0042cedf;//morphing

  static const u32 BWFXN_RefundMinAndGas5               = 0x00453E62;//research cancel
  static const u32 BWFXN_RefundMinAndGas5ReturnAddress  = 0x00453E68;//research cancel

  // These 2 can be merged;
  // void __userpurge refundUpgrade(unsigned __int8 upgradeType<al>, unsigned __int8 playerID)
  // Function calculates the cost of the upgrade by taking the upgrade level, multiplying 
  // with cost factor, and adding base cost, then adding that to player's resources.
  static const u32 BWFXN_RefundMin4                     = 0x004541C2;//upgrade cancel
  static const u32 BWFXN_RefundMin4ReturnAddress        = 0x004541CD;//upgrade cancel
  static const u32 BWFXN_RefundGas4                     = 0x004541FD;//upgrade cancel
  static const u32 BWFXN_RefundGas4ReturnAddress        = 0x00454204;//upgrade cancel

  // A little trickier, but can also be merged
  static const u32 BWFXN_RefundMin3                     = 0x0045D44D;//morphing
  static const u32 BWFXN_RefundMin3ReturnAddress        = 0x0045D454;//morphing
  static const u32 BWFXN_RefundGas3                     = 0x0045D46e;//morphing
  static const u32 BWFXN_RefundGas3ReturnAddress        = 0x0045D474;//morphing

  static const u32 BWFXN_SpendRepair                    = 0x00467006;
  static const u32 BWFXN_SpendRepairReturnAddress       = 0x0046700D;

  static void (* const BWDATA_DSoundDestroy)()  = (void (*)()) 0x004BC180;

  static const u32 BWFXN_NetSelectReturnMenu   = 0x004DC5B0;

  static void (* const BWFXN_RandomizePlayerRaces)() = (void (*)()) 0x004A9A30;
  BWDATA(u32, RandomizeRacePatch, 0x004EF20E, 0);

  static void (* const BWFXN_InitializePlayerConsole)() = (void (*)()) 0x004EE180;
  BWDATA(u32, InitPlayerConsolePatch, 0x004EF244, 0);

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl * const BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;
  BWDATA(u32*, MoveToX, 0x0062848C, 0);
  BWDATA(u32*, MoveToY, 0x006284A8, 0);
  BWDATA(TilePosition*, MoveToTile, 0x0057F1D0, 0);

  BWDATA(POINT*, Mouse, 0x006CDDC4, 0);
  BWDATA(u32*, ScreenX, 0x00628448, 0);
  BWDATA(u32*, ScreenY, 0x00628470, 0);

  BWDATA(TilePosition*, MapSize, 0x0057F1D4, 0);

  //--------------------------------------------- STRINGS ----------------------------------------------------
  BWDATA(char*, CurrentMapFileName, 0x0057FD3C, 0);
  BWDATA(char*, CurrentMapName, 0x0057FE40, 0);
  BWDATA(char*, CurrentMapFolder, 0x0059BB70, 0);
  BWDATA(char*, SaveGameFile, 0x00685148, 0);

  struct MapVectorEntry   // sizeof 1348
  {
    char  szEntryName[64];        // fixed entry name // 8
    BYTE  bUnknown_48;            // 72
    char  szMapName[32];          // 73
    char  szMapDescription[316];  // 105
    char  szTotalPlayers[35];     // 421
    char  szComputerSlots[35];    // 456
    char  szHumanSlots[35];       // 491
    char  szMapDimensions[35];    // 526
    char  szMapTileset[35];       // 561
    DWORD dwListEntryIndex;       // 596
    DWORD dwUnknown_258;          // 600  Computer slots?
    DWORD dwUnknown_25C;          // 604
    BYTE  bGameState;             // 608
    BYTE  bUnk_609;
    WORD  wUnk_610;
    DWORD dwUnknown_264;          // 612
    BYTE  bEntryFlags;            // 616
    char  szFullPath[MAX_PATH];   // 617
    char  szFileName[MAX_PATH];   // 877
    BYTE  bUnknown_471;           // 1137
    WORD  wMapWidth;              // 1138
    WORD  wMapHeight;             // 1140
    WORD  wMapTileset;            // 1142
    BYTE  bUnknown_478;           // 1144
    BYTE  bTotalPlayers;          // 1145
    BYTE  bComputerSlots;         // 1146
    BYTE  bHumanSlots;            // 1147
    BYTE  bUnknown_47C;           // 1148
    WORD  wUnknown_47D;           // 1149
    struct 
    {
      struct
      {
        BYTE  bCmd;             //  1151
        DWORD dwSeed;           //  1152
        BYTE  bPlayerBytes[8];  //  1156
      } gameInitCommand;
      struct
      {
        DWORD dwUnknownAlways0;   // 1164
        char  szPlayerName[24];   // 1168
        DWORD dwGameFlags;        // 1192
        WORD  wMapWidth;          // 1196
        WORD  wMapHeight;         // 1198
        BYTE  bActivePlayerCount; // 1200
        BYTE  bAvailableSlots;    // 1201
        BYTE  bGameSpeed;         // 1202
        BYTE  bGameState;         // 1203
        WORD  wGameType;          // 1204
        WORD  wGameSubType;       // 1206
        DWORD dwSeed;             // 1208
        WORD  wTileset;           // 1212
        BYTE  bAutosaved;         // set if it was saved as LastReplay    // 1214
        BYTE  bComputerPlayerCount; // 1215
      } gameData;
      char szGameName[25];  // 1216
      char szMapName[32];   // 1241
      struct
      {
        WORD  wGameType;              // 1273
        WORD  wGameSubType;           // 1275
        WORD  wSubTypeDisplay;        // 1277
        WORD  wSubTypeLabel;          // 1279
        BYTE  bVictoryCondition;      // 1281
        BYTE  bResourceType;          // 1282
        BYTE  bUseStandardUnitStats;  // 1283
        BYTE  bFogOfWar_Unused;       // 1284
        BYTE  bStartingUnits;         // 1285
        BYTE  bUseFixedPosition;      // 1286
        BYTE  bRestrictionFlags;      // 1287
        BYTE  bAlliesEnabled;         // 1288
        BYTE  bTeamsEnabled;          // 1289
        BYTE  bCheatsEnabled;         // 1290
        BYTE  bTournamentModeEnabled; // 1291
        DWORD dwVictoryConditionValue;  // 1292
        DWORD dwResourceMineralsValue;  // 1296
        DWORD dwResourceGasValue; // unused // 1300
        BYTE  __align_1; // 1304
      } gameTemplate;
    } partialReplayGameHeader;
    DWORD dwUnknown_519[7]; // 1305
    BYTE  bPlayerSlotEnabled[12];  // 1333
    BYTE  __align_3[3];   // 1345
  };  // 1348

  struct TransVectorEntry
  {
    HANDLE            hTrans;
    RECT              info;
    DWORD             dwReserved;
  };


  template <class _T>
  struct BlizzVectorEntry
  {
    BlizzVectorEntry<_T> *prev; // First entry points to begin in controller
    BlizzVectorEntry<_T> *next; // negative value indicates vector::end and points to &end in controller
    _T container;
  };

  template <class _T>
  struct BlizzVectorController // sizeof 12
  {
    int unknown_00;
    BlizzVectorEntry<_T> *end;
    BlizzVectorEntry<_T> *begin;
  };

  BWDATA(BlizzVectorController<MapVectorEntry>*, MapListVector, 0x0051A274, 0);
  BWDATA(BlizzVectorController<TransVectorEntry>*, TransMaskVector, 0x0051A334, 0);
  BWDATA(BlizzVectorController<Triggers::Trigger>*, TriggerVectors, 0x0051A280, 0);

  static bool (__fastcall ** const BWDATA_TriggerActionFxnTable)(BW::Triggers::Action*) = (bool(__fastcall**)(BW::Triggers::Action*))0x00512800;

  static void (__stdcall * const BWFXN_ExecuteGameTriggers)(DWORD dwMillisecondsPerFrame) = (void(__stdcall*)(DWORD))0x0048A460;
  BWDATA(u32, ExecuteGameTrigsCallPatch, 0x004D9798, 0);

  const char            *GetStatString(int index);
  extern char           *BWDATA_StringTableOff;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct AllScores
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

    /** Supply available, used, and maximum for all players and every race */
    struct SuppliesPerRace
    {
      s32 available[PLAYER_COUNT];
      s32 used[PLAYER_COUNT];
      s32 max[PLAYER_COUNT];
    };
    SuppliesPerRace supplies[RACE_COUNT];
    s32 customScore[PLAYER_COUNT];

    /** Unit counts: all, completed, killed, dead */
    struct Counts
    {
      s32 all[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 completed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 killed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 dead[UNIT_TYPE_COUNT][PLAYER_COUNT];
    };
    Counts unitCounts;
  };
  BWDATA(AllScores*, AllScores, 0x00581DE4, 0);
  /*
  //---------------------------------------------- UNIT DATA -------------------------------------------------
  static const u8  *BWDATA_Unit_Graphic             = (const u8*)  unitsDat[0].address;
  static const u16 *BWDATA_Unit_SubUnit             = (const u16*) unitsDat[1].address;
  static const u32 *BWDATA_Unit_ConstructionGraphic = (const u32*) unitsDat[4].address;
  static const u8  *BWDATA_Unit_ShieldsEnabled      = (const u8*)  unitsDat[6].address;
  static const u16 *BWDATA_Unit_MaxShieldPoints     = (const u16*) unitsDat[7].address;
  static const s32 *BWDATA_Unit_MaxHitPoints        = (const s32*) unitsDat[8].address;
  static const u8  *BWDATA_Unit_Elevation           = (const u8*)  unitsDat[9].address;
  static const u8  *BWDATA_Unit_GroundWeapon        = (const u8*)  unitsDat[17].address;
  static const u8  *BWDATA_Unit_MaxGroundHits       = (const u8*)  unitsDat[18].address;
  static const u8  *BWDATA_Unit_AirWeapon           = (const u8*)  unitsDat[19].address;
  static const u8  *BWDATA_Unit_MaxAirHits          = (const u8*)  unitsDat[20].address;
  static const u32 *BWDATA_Unit_PrototypeFlags      = (const u32*) unitsDat[22].address;
  static const u8  *BWDATA_Unit_SeekRange           = (const u8*)  unitsDat[23].address;
  static const u8  *BWDATA_Unit_SightRange          = (const u8*)  unitsDat[24].address;
  static const u8  *BWDATA_Unit_ArmorUpgrade        = (const u8*)  unitsDat[25].address;
  static const u8  *BWDATA_Unit_SizeType            = (const u8*)  unitsDat[26].address;
  static const u8  *BWDATA_Unit_ArmorAmount         = (const u8*)  unitsDat[27].address;
  static const u16 *BWDATA_Unit_MineralCost         = (const u16*) unitsDat[40].address;
  static const u16 *BWDATA_Unit_GasCost             = (const u16*) unitsDat[41].address;
  static const u16 *BWDATA_Unit_TimeCost            = (const u16*) unitsDat[42].address;
  static const u8  *BWDATA_Unit_GroupFlags          = (const u8*)  unitsDat[44].address;
  static const u8  *BWDATA_Unit_SupplyProvided      = (const u8*)  unitsDat[45].address;
  static const u8  *BWDATA_Unit_SupplyRequired      = (const u8*)  unitsDat[46].address;
  static const u8  *BWDATA_Unit_SpaceRequired       = (const u8*)  unitsDat[47].address;
  static const u8  *BWDATA_Unit_SpaceProvided       = (const u8*)  unitsDat[48].address;
  static const u16 *BWDATA_Unit_BuildScore          = (const u16*) unitsDat[49].address;
  static const u16 *BWDATA_Unit_DestroyScore        = (const u16*) unitsDat[50].address;
  static const u16 *BWDATA_Unit_MapStringID         = (const u16*) unitsDat[51].address;
  static const u8  *BWDATA_Unit_BroodwarOnly        = (const u8*)  unitsDat[52].address;

  /// Unit Placement Size
  struct UnitPlacement_type
  {
    struct Placement_Internal_type
    {
      u16 height;
      u16 width;
    } unitType[UNIT_TYPE_COUNT];
  };
  static const UnitPlacement_type *BWDATA_Unit_Placement = (const UnitPlacement_type*) unitsDat[36].address;

  // Unit Dimensions; The distance from the 1px 'center' of unit to each border
  struct UnitsDimensions_type
  {
    struct UnitDimensions
    {
      u16 left;
      u16 up;
      u16 right;
      u16 down;
    } units[UNIT_TYPE_COUNT];
  };
  static const UnitsDimensions_type *BWDATA_Unit_Dimensions = (const UnitsDimensions_type*) unitsDat[38].address;
  */
  struct _uavail
  {
    u8 available[12][228];
  };
  BWDATA(_uavail*, UnitAvailability, 0x0057F27C, 0);
  /*
  //-------------------------------------------- FLINGY DATA -------------------------------------------------
  static const u16 *BWDATA_Flingy_SpriteID        = (const u16*) flingyDat[0].address;
  static const u32 *BWDATA_Flingy_TopSpeed        = (const u32*) flingyDat[1].address;
  static const u16 *BWDATA_Flingy_Acceleration    = (const u16*) flingyDat[2].address;
  static const u32 *BWDATA_Flingy_HaltDistance    = (const u32*) flingyDat[3].address;
  static const u8  *BWDATA_Flingy_TurnRadius      = (const u8*)  flingyDat[4].address;
  static const u8  *BWDATA_Flingy_MovementControl = (const u8*)  flingyDat[6].address;

  //-------------------------------------------- WEAPON DATA -------------------------------------------------
  static const u16 *BWDATA_Weapon_Label              = (const u16*) weaponsDat[0].address;
  static const u32 *BWDATA_Weapon_Graphic            = (const u32*) weaponsDat[1].address;
  static const u16 *BWDATA_Weapon_TargetFlags        = (const u16*) weaponsDat[3].address;
  static const u32 *BWDATA_Weapon_MinRange           = (const u32*) weaponsDat[4].address;
  static const u32 *BWDATA_Weapon_MaxRange           = (const u32*) weaponsDat[5].address;
  static const u8  *BWDATA_Weapon_Upgrade            = (const u8*)  weaponsDat[6].address;
  static const u8  *BWDATA_Weapon_DamageType         = (const u8*)  weaponsDat[7].address;
  static const u8  *BWDATA_Weapon_Behavior           = (const u8*)  weaponsDat[8].address;
  static const u8  *BWDATA_Weapon_RemoveAfter        = (const u8*)  weaponsDat[9].address;
  static const u8  *BWDATA_Weapon_ExplosionType      = (const u8*)  weaponsDat[10].address;
  static const u16 *BWDATA_Weapon_InnerSplashRadius  = (const u16*) weaponsDat[11].address;
  static const u16 *BWDATA_Weapon_MedianSplashRadius = (const u16*) weaponsDat[12].address;
  static const u16 *BWDATA_Weapon_OuterSplashRadius  = (const u16*) weaponsDat[13].address;
  static const u16 *BWDATA_Weapon_DamageAmount       = (const u16*) weaponsDat[14].address;
  static const u16 *BWDATA_Weapon_DamageBonus        = (const u16*) weaponsDat[15].address;
  static const u8  *BWDATA_Weapon_DamageCooldown     = (const u8*)  weaponsDat[16].address;
  static const u8  *BWDATA_Weapon_DamageFactor       = (const u8*)  weaponsDat[17].address;
  static const u8  *BWDATA_Weapon_AttackDirection    = (const u8*)  weaponsDat[18].address;
  static const u8  *BWDATA_Weapon_LaunchSpin         = (const u8*)  weaponsDat[19].address;
  static const u8  *BWDATA_Weapon_XOffset            = (const u8*)  weaponsDat[20].address;
  static const u8  *BWDATA_Weapon_YOffset            = (const u8*)  weaponsDat[21].address;

  //------------------------------------------- UPGRADE DATA -------------------------------------------------
  static const u16            *BWDATA_Upgrade_MineralCostBase   = (const u16*)  upgradesDat[0].address;
  static const u16            *BWDATA_Upgrade_MineralCostFactor = (const u16*)  upgradesDat[1].address;
  static const u16            *BWDATA_Upgrade_GasCostBase       = (const u16*)  upgradesDat[2].address;
  static const u16            *BWDATA_Upgrade_GasCostFactor     = (const u16*)  upgradesDat[3].address;
  static const u16            *BWDATA_Upgrade_TimeCostBase      = (const u16*)  upgradesDat[4].address;
  static const u16            *BWDATA_Upgrade_TimeCostFactor    = (const u16*)  upgradesDat[5].address;
  static const u16            *BWDATA_Upgrade_Label             = (const u16*)  upgradesDat[8].address;
  static const u8             *BWDATA_Upgrade_Race              = (const u8*)   upgradesDat[9].address;
  static const u8             *BWDATA_Upgrade_MaxRepeats        = (const u8*)   upgradesDat[10].address;
  */
  struct _scUpgrs {    u8 level[PLAYER_COUNT][46];  };
  struct _bwUpgrs {    u8 level[PLAYER_COUNT][15];  };
  BWDATA(_scUpgrs*, UpgradeMaxSC, 0x0058D088, 0);
  BWDATA(_scUpgrs*, UpgradeLevelSC, 0x0058D2B0, 0);
  BWDATA(_bwUpgrs*, UpgradeMaxBW, 0x0058F24A, 0);
  BWDATA(_bwUpgrs*, UpgradeLevelBW, 0x0058F32C, 0);

  BWDATA(u32, UpgradeProgress, 0x0058F3E0, 0);

  //--------------------------------------------- TECH DATA --------------------------------------------------
  /*
  static const u16 *BWDATA_Tech_MineralCost  = (const u16*) techdataDat[0].address;
  static const u16 *BWDATA_Tech_GasCost      = (const u16*) techdataDat[1].address;
  static const u16 *BWDATA_Tech_TimeCost     = (const u16*) techdataDat[2].address;
  static const u16 *BWDATA_Tech_EnergyCost   = (const u16*) techdataDat[3].address;
  static const u16 *BWDATA_Tech_LabelIndex   = (const u16*) techdataDat[7].address;
  */

  struct _scTechs {    u8 enabled[PLAYER_COUNT][24];  };
  struct _bwTechs {    u8 enabled[PLAYER_COUNT][20];  };
  BWDATA(_scTechs*, TechAvailableSC, 0x0058CE24, 0);
  BWDATA(_scTechs*, TechResearchSC, 0x0058CF44, 0);
  BWDATA(_bwTechs*, TechAvailableBW, 0x0058F050, 0);
  BWDATA(_bwTechs*, TechResearchBW, 0x0058F140, 0);

  BWDATA(u32, ResearchProgress, 0x0058F230, 0);

  //------------------------------------------------ MAPPING -------------------------------------------------
  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  extern TileID   *BWDATA_MapTileArray;
  extern TileType *BWDATA_TileSet;

  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      u16 miniTile[16];
    };
    MiniTileFlagArray tile[0x10000];
  };
  extern MiniTileMaps_type *BWDATA_MiniTileFlags;

  struct activeTile
  {
    u8 bVisibilityFlags;
    u8 bExploredFlags;
    u8 bWalkable          : 1; // Set on tiles that can be walked on
    u8 bUnknown1          : 1; // Unused?
    u8 bUnwalkable        : 1; // Set on tiles that can't be walked on
    u8 bUnknown2          : 3; // Unused?
    u8 bHasCreep          : 1; // Set when creep occupies the area
    u8 bAlwaysUnbuildable : 1; // always unbuildable, like water
    u8 bGroundHeight      : 3; // ground height
    u8 bCurrentlyOccupied : 1; // unbuildable but can be made buildable
    u8 bCreepReceeding    : 1; // Set when the nearby structure supporting the creep is destroyed
    u8 bCliffEdge         : 1; // Set if the tile is a cliff edge
    u8 bTemporaryCreep    : 1; // Set when the creep occupying the area was created. Not set if creep tiles were preplaced.
    u8 bUnknown3          : 1; // Unused?
  };
  extern activeTile        *BWDATA_ActiveTileArray;
  extern SAI_Paths         *BWDATA_SAIPathing;

  // EXPERIMENTAL
  //static int (__stdcall *BWFXN_getTileDistance)(int x, int y, int x2, int y2) = (int (__stdcall*)(int,int,int,int))0x00444100;
  
  struct baseLocation
  {
    /* 0x00 */BW::Position position;
    /* 0x04 */BYTE  mineralClusters;
    /* 0x05 */BYTE  gasGeysers;
    /* 0x06 */BYTE  isStartLocation; // The "base" is actually a Start Location
    /* 0x07 */BYTE  bFlags;
                    /*  0x01 - This base is currently occupied.
                        0x02 - An AI script has acquired this base as an expansion.
                    */
    /* 0x08 */DWORD remainingMinerals;
    /* 0x0C */DWORD remainingGas;
    /* 0x10 */DWORD unk_10[8];
  };

  // 250 entries max
  //static baseLocation *BWDATA_Bases = (baseLocation*)0x00692688;
};
#pragma pack()
