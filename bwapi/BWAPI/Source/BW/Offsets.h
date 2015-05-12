#pragma once
#include <Util/Types.h>
#include <storm.h>
#include <array>
#include <type_traits>

#include "Constants.h"

#include "TriggerEngine.h"
#include "Position.h"

#include "Dialog.h"

#pragma pack(1)

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */

namespace BW
{
  class  CUnit;
  class  CSprite;
  class  CImage;
  class  CThingy;
  class  CBullet;
  class  TileType;
  class  dialog;
  class  Bitmap;
  struct dlgEvent;
  class  Font;
  struct SAI_Paths;
  struct grpHead;

  /*

  //----------------------------------------------- DATA TABLES ----------------------------------------------
  struct DatLoad
  {
  u32   address;
  u32   length;
  u32   entries;
  };

  BW_DATA(DatLoad*, Upgrades_Table, 0x005136E0, 0x0015AFCC);   // Mac: for 1.16.1
  BW_DATA(DatLoad*, TechData_Table, 0x005137D8, 0x0015A6F4);   // PC: all for 1.15.3, 1.16, and 1.16.1
  BW_DATA(DatLoad*, Weapons_Table, 0x00513868, 0x0015C19C);
  BW_DATA(DatLoad*, Units_Table, 0x00513C30, 0x0015AD38);
  BW_DATA(DatLoad*, Flingy_Table, 0x00515A38, 0x0014BC08);
  */
  //----------------------------------------------- PLAYER DATA ----------------------------------------------
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
  


#define IS_REF(name,addr) (& name) = *reinterpret_cast<std::remove_reference<decltype(name)>::type*>(addr);

  namespace BWDATA
  {
    namespace
    {
      // Player Information
      PlayerResourcesStruct IS_REF(PlayerResources, 0x0057F0F0);
      std::array<u8, PLAYABLE_PLAYER_COUNT> IS_REF(PlayerVictory, 0x00650974);
      std::array<BW::Position, PLAYABLE_PLAYER_COUNT> IS_REF(startPositions, 0x0058D720);

      std::array<std::array<char, 30>, 4> IS_REF(ForceNames, 0x0058D5BC);
      std::array<PlayerInfo, PLAYER_COUNT> IS_REF(Players, 0x0057EEE0);
      std::array<u32, PLAYABLE_PLAYER_COUNT> IS_REF(PlayerDownloadStatus, 0x0068F4FC);
      std::array<u32, PLAYABLE_PLAYER_COUNT> IS_REF(playerStatusArray, 0x0057F0B8);

      // Alliance and vision
      std::array<std::array<u8, PLAYER_COUNT>, PLAYER_COUNT> IS_REF(Alliance, 0x0058D634);
      std::array<u32, PLAYER_COUNT> IS_REF(PlayerVision, 0x0057F1EC);
      u32 IS_REF(ReplayVision, 0x006D0F18);
      u32 IS_REF(ReplayRevealAll, 0x006D0F1C);

      // Code Patches
      const u32 ServerMenuIn = 0x004DD5A2;
      const u32 ServerMenuOut = 0x004DD5C9;
      const u32 OpponentStartHack = 0x004B995D;
      const u32 SingleSpeedHack = 0x004D99FB;

      struct swishTimer
      {
        u16 wIndex;
        u16 wType;
      };
      static_assert(sizeof(swishTimer) == 4, "Expected swishTimer structure to be 4 bytes.");

      std::array<swishTimer, 43> IS_REF(commonSwishControllers, 0x005129EC);
      std::array<swishTimer, 5> IS_REF(gluCustmSwishController, 0x0051A9F0);
      std::array<swishTimer, 2> IS_REF(gluCmpgnSwishController, 0x00512B10);
      std::array<swishTimer, 1> IS_REF(gluScoreSwishController, 0x0051A844);
      std::array<swishTimer, 5> IS_REF(gluChatSwishController, 0x0051A490);

      // Command queue data
      u32 IS_REF(sgdwBytesInCmdQueue, 0x00654AA0);
      std::array<u8, TURN_BUFFER_SIZE> IS_REF(TurnBuffer, 0x00654880);

      static void(*const BWFXN_sendTurn)() = (void(*)()) 0x00485A40;
      const u32 BWFXN_QueueCommand = 0x00485BD0;

      // Speed and Latency
      struct GameSpeeds
      {
        std::array<u32, NUM_SPEEDS> gameSpeedModifiers;
        std::array<u32, NUM_SPEEDS> altSpeedModifiers;
      };
      GameSpeeds IS_REF(GameSpeedModifiers, 0x005124D8);   // mac: 0x0014BF3C
      u32 IS_REF(GameSpeed, 0x006CDFD4);  // mac: 0x00228458

      std::array<u32, NUM_SPEEDS> IS_REF(LatencyFrames, 0x0051CE70);
      u32 IS_REF(FramesUntilNextTurn, 0x0051CEA0);
      s32 IS_REF(FrameSkip, 0x005124D4);
      u8 IS_REF(Latency, 0x006556e4);   // A value between 0 and 2 indicating latency setting

      // Dialog and drawing
      std::array<dialog::FnInteract*, ctrls::max> IS_REF(GenericDlgInteractFxns, 0x005014AC);
      std::array<dialog::FnUpdate*, ctrls::max> IS_REF(GenericDlgUpdateFxns, 0x00501504);
      dialog* IS_REF(DialogList, 0x006D5E34);

      std::array<Font*, 4> IS_REF(FontBase, 0x006CE0F4);
      Bitmap IS_REF(GameScreenBuffer, 0x006CEFF0);

      std::array<PALETTEENTRY, 256> IS_REF(GamePalette, 0x006CE320);
      LPDIRECTDRAW IS_REF(DDInterface, 0x006D5E08);
      LPDIRECTDRAWPALETTE IS_REF(PrimaryPalette, 0x006D5E0C);
      LPDIRECTDRAWSURFACE IS_REF(PrimarySurface, 0x006D5E00);
      LPDIRECTDRAWSURFACE IS_REF(BackSurface, 0x006D5E10);

      static void(__cdecl * const BWFXN_DDrawDestroy)() = (void(__cdecl*)())0x0041D8B0;
      const u32 DDrawInitCallPatch = 0x004DB0A2;

      std::array<u8, PLAYER_COUNT> IS_REF(PlayerColors, 0x00581DD6);

      std::array<layer, 8> IS_REF(ScreenLayers, 0x006CEF50);
      RECT IS_REF(ScrLimit, 0x0051A15C);
      RECT IS_REF(ScrSize, 0x0051A16C);

      // Unit and CList offsets
      CUnit* IS_REF(UnitNodeList_VisibleUnit_First, 0x00628430);
      CUnit* IS_REF(UnitNodeList_HiddenUnit_First, 0x006283EC);
      CUnit* IS_REF(UnitNodeList_ScannerSweep_First, 0x006283F4);

      std::array<CUnit, UNIT_ARRAY_MAX_LENGTH> IS_REF(UnitNodeTable, 0x0059CCA8);

      CBullet* IS_REF(BulletNodeTable_FirstElement, 0x0064DEC4);
      std::array<CBullet, BULLET_ARRAY_MAX_LENGTH> IS_REF(BulletNodeTable, 0x0064B2E8);

      // Unit ordering system (fast units in region calculations)
      std::array<unitFinder, UNIT_ARRAY_MAX_LENGTH * 2> IS_REF(UnitOrderingX, 0x0066FF78);
      std::array<unitFinder, UNIT_ARRAY_MAX_LENGTH * 2> IS_REF(UnitOrderingY, 0x006769B8);
      int IS_REF(UnitOrderingCount, 0x0066FF74);

      // Mode stuff
      u8 IS_REF(gameType, 0x00596820);
      u32 IS_REF(InReplay, 0x006D0F14);
      int IS_REF(NetMode, 0x0059688C);
      int IS_REF(CountdownTimer, 0x0058D6F4);  // Countdown Timer (in seconds)
      int IS_REF(ElapsedTime, 0x0058D6F8); // Elapsed Game Time (in seconds)

      int IS_REF(ReplayFrames, 0x006D0F31); // note: unaligned because it's part of a structure that we're not using

      u8 IS_REF(GameState, 0x006D11EC);
      u16 IS_REF(gwNextGameMode, 0x0051CE90);
      u16 IS_REF(gwGameMode, 0x00596904);
      u32 IS_REF(glGluesMode, 0x006D11BC);

      int IS_REF(g_LocalHumanID, 0x00512688);

      // For fixing BW-level memory leak
      std::array<void*,2> IS_REF(customList_UIDlgData, 0x0051A350);

      // Selected units
      std::array<CUnit*, MAX_SELECTION_COUNT> IS_REF(ClientSelectionGroup, 0x00597208);
      u8 IS_REF(ClientSelectionCount, 0x0059723D);

      // Inputs
      std::array<void(__fastcall *)(dlgEvent*), BW_EVN_MAX> IS_REF(InputProcedures, 0x005968A0);
      std::array<dialog*, BW_EVN_MAX> IS_REF(EventDialogs, 0x006D5E40);
      u8 IS_REF(InputFlags, 0x006CDDC0);

      //
      u32 IS_REF(isGamePaused, 0x006509C4);

      // Positions (mouse/screen)
      u32 IS_REF(MoveToX, 0x0062848C);
      u32 IS_REF(MoveToY, 0x006284A8);
      TilePosition IS_REF(MoveToTile, 0x0057F1D0);

      POINT IS_REF(Mouse, 0x006CDDC4);
      u32 IS_REF(ScreenX, 0x00628448);
      u32 IS_REF(ScreenY, 0x00628470);

      TilePosition IS_REF(MapSize, 0x0057F1D4);

      // Strings
      std::array<char, MAX_PATH> IS_REF(CurrentMapFileName, 0x0057FD3C);
      std::array<char, 32> IS_REF(CurrentMapName, 0x0057FE40);
      std::array<char, MAX_PATH> IS_REF(CurrentMapFolder, 0x0059BB70);
      std::array<char, 28> IS_REF(SaveGameFile, 0x00685148);
    }
  }

  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  extern void (__stdcall *pOldDrawGameProc)(BW::Bitmap *pSurface, BW::bounds *pBounds);
  extern void (__stdcall *pOldDrawDialogProc)(BW::Bitmap *pSurface, BW::bounds *pBounds);
  
  //----------------------------------------- FUNCTION LEVEL -------------------------------------------------
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

  static void (* const BWFXN_DSoundDestroy)()  = (void (*)()) 0x004BC180;

  static const u32 BWFXN_NetSelectReturnMenu   = 0x004DC5B0;

  static void (* const BWFXN_RandomizePlayerRaces)() = (void (*)()) 0x004A9A30;
  static const u32 BWFXN_RandomizeRacePatch = 0x004EF20E;

  static void (* const BWFXN_InitializePlayerConsole)() = (void (*)()) 0x004EE180;
  static const u32 BWFXN_InitPlayerConsolePatch = 0x004EF244;

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl * const BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;

  //--------------------------------------------- STRINGS ----------------------------------------------------
  // TODO: Get rid of this
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
    DWORD dwUnknown_260;          // 608
    DWORD dwUnknown_264;          // 612
    BYTE  bEntryFlags;            // 616
              //  0x01: Game
              //  0x04: Standard
              //  0x08: Folder
              //  0x80: Replay
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
  namespace BWDATA
  {
    namespace
    {
      BlizzVectorController<MapVectorEntry> IS_REF(MapListVector, 0x0051A274);
      std::array<BlizzVectorController<Triggers::Trigger>, PLAYABLE_PLAYER_COUNT> IS_REF(TriggerVectors, 0x0051A280);
    }
  }

  static bool (__fastcall ** const TriggerActionCallbacks)(BW::Triggers::Action*) = (bool(__fastcall**)(BW::Triggers::Action*))0x00512800;

  static void (__stdcall * const BWFXN_ExecuteGameTriggers)(DWORD dwMillisecondsPerFrame) = (void(__stdcall*)(DWORD))0x0048A460;
  static const u32 BWFXN_ExecuteGameTrigsCallPatch = 0x004D9798;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
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
  namespace BWDATA
  {
    namespace
    {
      BW::AllScoresStruct IS_REF(AllScores, 0x00581DE4);
      std::array< std::array<u8, BW::UNIT_TYPE_COUNT>, BW::PLAYER_COUNT> IS_REF(UnitAvailability, 0x0057F27C);

      //------------------------------------------- UPGRADE DATA -------------------------------------------------
      std::array< std::array<u8, 46>, PLAYER_COUNT> IS_REF(UpgradeMaxSC, 0x0058D088);
      std::array< std::array<u8, 46>, PLAYER_COUNT> IS_REF(UpgradeLevelSC, 0x0058D2B0);
      std::array< std::array<u8, 15>, PLAYER_COUNT> IS_REF(UpgradeMaxBW, 0x0058F278);
      std::array< std::array<u8, 15>, PLAYER_COUNT> IS_REF(UpgradeLevelBW, 0x0058F32C);

      static const u32 UpgradeProgress = 0x0058F3E0;

      //--------------------------------------------- TECH DATA --------------------------------------------------
      std::array< std::array<u8, 24>, PLAYER_COUNT> IS_REF(TechAvailableSC, 0x0058CE24);
      std::array< std::array<u8, 24>, PLAYER_COUNT> IS_REF(TechResearchSC, 0x0058CF44);
      std::array< std::array<u8, 20>, PLAYER_COUNT> IS_REF(TechAvailableBW, 0x0058F050);
      std::array< std::array<u8, 20>, PLAYER_COUNT> IS_REF(TechResearchBW, 0x0058F140);

      static const u32 ResearchProgress = 0x0058F230;
    }
  }

  //------------------------------------------------ MAPPING -------------------------------------------------
  // Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group.
  typedef u16 TileID;
  namespace BWDATA
  {
    namespace
    {
      TileID* IS_REF(MapTileArray, 0x005993C4); // MTXM (Matrix Map) -- gfpMIMap
      TileType* IS_REF(TileSetMap, 0x006D5EC8);  // cv5
    }
  }

  /// Direct mapping of minitile flags array
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      u16 miniTile[16];
    };
    MiniTileFlagArray tile[0x10000];
  };
  namespace BWDATA
  {
    namespace
    {
      MiniTileMaps_type* IS_REF(MiniTileFlags, 0x005993D0);   // vf4
    }
  }

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
    u8 bTemporaryCreep    : 1; // Set when the creep occupying the area was created. Not set if creep tiles were preplaced. Used in drawing routine.
    u8 bUnknown3          : 1; // Unused?
  };

  namespace BWDATA
  {
    namespace
    {
      activeTile* IS_REF(ActiveTileArray, 0x006D1260);
      SAI_Paths* IS_REF(SAIPathing, 0x006D5BFC);
    }
  }

};

#undef IS_REF

#pragma pack()
