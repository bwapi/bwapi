#pragma once
#pragma pack(1)

#include <windows.h>
#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"
#include "../Storm/storm.h"

#include "UnitPrototypeFlags.h"
#include "WeaponTargetFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"
#include "PlayerType.h"
#include "Race.h"
#include "Sprite.h"
#include "Dialog.h"
#include "Pathing.h"

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
#define TILE_SIZE                32

namespace BW
{
  struct Unit;
  struct UnitArray;
  struct Bullet;
  struct BulletArray;
  class  TileType;
  class  dialog;
  struct bitmap;
  struct rect;
  struct dlgEvent;
  struct fntHead;

  struct DatLoad
  {
    u32   address;
    u32   length;
    u32   entries;
  };

  //----------------------------------------------- DATA TABLES ----------------------------------------------
  static DatLoad *upgradesDat = (DatLoad*) 0x005136E0;  /** 1.15.3, 1.16, 1.16.1 */
  static DatLoad *techdataDat = (DatLoad*) 0x005137D8;
  static DatLoad *weaponsDat  = (DatLoad*) 0x00513868;
  static DatLoad *unitsDat    = (DatLoad*) 0x00513C30;
  static DatLoad *flingyDat   = (DatLoad*) 0x00515A38;

  //----------------------------------------------- PLAYER DATA ----------------------------------------------
  /** Player resource counts */
  struct PlayerResources
  {
    s32 minerals[PLAYER_COUNT];
    s32 gas[PLAYER_COUNT];
    s32 cumulativeGas[PLAYER_COUNT];
    s32 cumulativeMinerals[PLAYER_COUNT];
  };
  static PlayerResources *BWDATA_PlayerResources = (PlayerResources*) 0x0057F0F0;

  static u8        *BWDATA_PlayerVictory  = (u8*)       0x00650974;
  static Position  *BWDATA_startPositions = (Position*) 0x0058D720;

  /** Force Names */
  struct ForceName
  {
    char  name[30];
  };
  static ForceName *BWDATA_ForceNames = (ForceName*) 0x0058D5BC;

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
  static PlayerInfo *BWDATA_Players = (PlayerInfo*) 0x0057EEE0;

  /** Player Alliances */
  struct PlayerAlliance
  {
    u8 player[PLAYER_COUNT];
  };
  static PlayerAlliance *BWDATA_Alliance = (PlayerAlliance*) 0x0058D634;

  /** Unit counts: all, completed, killed, dead */
  struct Counts
  {
    s32 all[UNIT_TYPE_COUNT][PLAYER_COUNT];
    s32 completed[UNIT_TYPE_COUNT][PLAYER_COUNT];
    s32 killed[UNIT_TYPE_COUNT][PLAYER_COUNT];
    s32 dead[UNIT_TYPE_COUNT][PLAYER_COUNT];
  };
  static Counts *BWDATA_Counts             = (Counts*) 0x00582324;

  /** Code Patches */
  static u32 BWDATA_MenuLoadHack      = 0x004DE392;
  static u32 BWDATA_MenuInHack        = 0x004DD76E;
  static u32 BWDATA_MenuOutHack       = 0x004DD162;
  static u32 BWDATA_MultiplayerHack   = 0x004DD5A2;
  static u32 BWDATA_MultiplayerHack2  = 0x004DD5C9;
  static u32 BWDATA_OpponentStartHack = 0x004B995D;

  /** QueueGameCommand data */
  static u32    *BWDATA_sgdwBytesInCmdQueue = (u32*)      0x00654AA0;
  static u32    *BWDATA_MaxTurnSize         = (u32*)      0x0057F0D8;
  static u8     *BWDATA_TurnBuffer          = (u8*)       0x00654880;
  static s32    *BWDATA_LatencyCalls        = (s32*)      0x0057F090;
  static void  (*BWFXN_sendTurn)()          = (void(*)()) 0x00485A40;
  static u32    BWFXN_QueueCommand          =             0x00485BD0;

  /** Speed & Latency */
  static u32     OriginalSpeedModifiers[7]  =        { 167, 111, 83, 67, 56, 48, 42};
  static u32     *BWDATA_GameSpeedModifiers = (u32*) 0x005124D8;
  static u32     *BWDATA_GameSpeed          = (u32*) 0x006CDFD4;
  static u32     *BWDATA_LatencyFrames      = (u32*) 0x0051CE70;

  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  /** Dialog and drawing offsets */
  static bool (__fastcall **BWDATA_GenericDlgInteractFxns)(dialog*,dlgEvent*)   = (bool (__fastcall**)(dialog*,dlgEvent*))    0x005014AC;
  static void (__fastcall **BWDATA_GenericDlgUpdateFxns)(dialog*,int,int,rect*) = (void (__fastcall**)(dialog*,int,int,rect*))0x00501504;
  static dialog           **BWDATA_DialogList                                   = (dialog**)  0x006D5E34;
  static fntHead          **BWDATA_FontBase                                     = (fntHead**) 0x006CE0F4;
  
  static bitmap           *BWDATA_GameScreenBuffer                              = (bitmap*)   0x006CEFF0;
  static bitmap           *BWDATA_GameScreenConsole                             = (bitmap*)   0x00597240;
  
  struct bltMask
  {
    bltMask *prev;
    bltMask *next;
    HANDLE  hTrans;
    RECT    info;
    DWORD   dwReserved;
  };
  static bltMask *BWDATA_MainBltMask = (bltMask*)0x00597238;

  static void (__cdecl *BWFXN_DDrawDestroy)()       = (void(__cdecl*)())0x0041D8B0;
  static void (__cdecl *BWFXN_DDrawInitialize)()    = (void(__cdecl*)())0x0041D930;
  static IDirectDrawSurface *BWDATA_PrimarySurface  = (IDirectDrawSurface*)0x006D5E00;
  static PALETTEENTRY *BWDATA_StormPalette = (PALETTEENTRY*)0x006CE320;

  static u8               *BWDATA_RefreshRegions                                = (u8*)       0x006CEFF8;
  static u8               *BWDATA_PlayerColors                                  = (u8*)       0x00581DD6;

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
  static layer   *BWDATA_ScreenLayers = (layer*)0x006CEF50;
  extern void (__stdcall *pOldDrawGameProc)(BW::bitmap *pSurface, BW::bounds *pBounds);
  extern void (__stdcall *pOldDrawDialogProc)(BW::bitmap *pSurface, BW::bounds *pBounds);

  static RECT *BWDATA_ScrLimit = (RECT*)0x0051A15C;
  static RECT *BWDATA_ScrSize  = (RECT*)0x0051A16C;

  
  //------------------------------------------- CLIST DATA ---------------------------------------------------
  static Unit      **BWDATA_UnitNodeList_VisibleUnit_First  = (Unit**)     0x00628430;
  static Unit      **BWDATA_UnitNodeList_HiddenUnit_First   = (Unit**)     0x006283EC;
  static Unit      **BWDATA_UnitNodeList_ScannerSweep_First = (Unit**)     0x006283F4;
  //static Unit      **BWDATA_UnitNodeList_UnusedUnit_First   = (Unit**)     0x00628438;
  //static Unit      **BWDATA_UnitNodeTable_PlayerFirstUnit   = (Unit**)     0x0062843C;  // Haven't found the right offset yet. Should point to the first unit of the first player (player 1).
  static UnitArray *BWDATA_UnitNodeTable                    = (UnitArray*) 0x0059CCA8;

  static Bullet      **BWDATA_BulletNodeTable_FirstElement = (Bullet**)     0x0064DEAC;
  static Bullet      **BWDATA_BulletNodeTable_LastElement  = (Bullet**)     0x0064DEC4;
  static BulletArray *BWDATA_BulletNodeTable               = (BulletArray*) 0x0064B2E8;

  //------------------------------------------- DATA LEVEL ---------------------------------------------------
  static u8             *BWDATA_gameType       = (u8*)  0x00596820;
  static u8             *BWDATA_Latency        = (u8*)  0x006556e4;
  static u32            *BWDATA_InGame         = (u32*) 0x006556E0;
  static u32            *BWDATA_InReplay       = (u32*) 0x006D0F14;
  static int            *BWDATA_GameModule     = (int*) 0x0059688C;
  static u8             *BWDATA_IsRunning      = (u8*)  0x0051CE6C;

  // note: unaligned because it's part of a structure that we're not using
  static u32            *BWDATA_ReplayFrames   = (u32*) 0x006D0F31;

  static u32            *BWDATA_Ophelia        = (u32*) 0x0051BFF8;
  static u8             *BWDATA_GameState      = (u8*)  0x006D11EC;  // 1 if loading?
  static u16            *BWDATA_gwNextGameMode = (u16*) 0x0051CE90;
  static u16            *BWDATA_gwGameMode     = (u16*) 0x00596904;
  /*
    GAME_INTRO          = 0,  // guessed
    GAME_RUNINIT        = 1,  // guessed; seems to be set between menu and game
    GAME_EXIT           = 2,  // guessed; exit starcraft
    GAME_RUN            = 3,  // official -- begins game
    GAME_GLUES          = 4,  // official -- uses glGluesMode
    GAME_RESTART        = 5,  // guessed
    GAME_WIN?           = 6,  // guessed
    GAME_LOSE?          = 7,  // guessed
    GAME_CREDITS        = 8,  // guessed
    GAME_EPILOG         = 9,  // guessed
    GAME_CINEMATIC      = 10  // guessed
  */
  static u32            *BWDATA_glGluesMode    = (u32*) 0x006D11BC;
  /*
    GLUE_MAIN_MENU,         // guess
    GLUE_SIMULATE,          // guess
    GLUE_CONNECT,           // guess
    GLUE_CHAT,              // official
    GLUE_BATTLE,            // guess
    GLUE_LOGIN,             // guess
    GLUE_CAMPAIGN,          // guess
    GLUE_READY_T,           // guess
    GLUE_READY_Z,           // guess
    GLUE_READY_P,           // guess
    GLUE_GAME_SELECT,       // guess
    GLUE_CREATE,            // guess
    GLUE_CREATE2,           // guess
    GLUE_LOAD,              // guess
    GLUE_SCORE_Z_DEFEAT,    // guess
    GLUE_SCORE_Z_VICTORY,   // guess
    GLUE_SCORE_T_DEFEAT,    // guess
    GLUE_SCORE_T_VICTORY,   // guess
    GLUE_SCORE_P_DEFEAT,    // guess
    GLUE_SCORE_P_VICTORY,   // guess
    GLUE_MODEM,             // guess
    GLUE_DIRECT,            // guess
    GLUE_CAMPAIGN2,         // guess
    GLUE_GAME_MODE,         // guess
    GLUE_GAME_MODE2,        // guess
    GLUE_GENERIC            // guess
  */

  static u32 *BWDATA_g_LocalHumanID = (u32*) 0x00512688;

  //----------------------------------------- FUNCTION LEVEL -------------------------------------------------
  static Unit **BWDATA_ClientSelectionGroup = (Unit**) 0x00597208;
  static u8   *BWDATA_ClientSelectionCount  = (u8*)    0x0059723D;
  static u32  BWDATA_PlayerSelection        =          0x006284E0;

  static int *BWDATA_NextLogicFrameData = (int*) 0x006509C4;
  static u32 BWFXN_NextLogicFrame       =        0x004D974E;

  static void (*BWDATA_DSoundDestroy)()  = (void (*)()) 0x004BC180;

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl *BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;
  static u32            *BWDATA_MoveToX                  = (u32*)             0x0062848C;
  static u32            *BWDATA_MoveToY                  = (u32*)             0x006284A8;
  static Position       *BWDATA_MoveToTile               = (Position*)       0x0057F1D0;

  static POINT          *BWDATA_Mouse                    = (POINT*) 0x006CDDC4;
  static u32            *BWDATA_ScreenX                  = (u32*)   0x00628448;
  static u32            *BWDATA_ScreenY                  = (u32*)   0x00628470;

  static Position       *BWDATA_MapSize                  = (Position*) 0x0057F1D4;

  //--------------------------------------------- STRINGS ----------------------------------------------------
  static char           *BWDATA_menuMapFileName          = (char*) 0x0059BC88;
  static char           *BWDATA_menuMapRelativePath      = (char*) 0x0059B850;
  static int            *BWDATA_menuStuff                = (int*)  0x0057EEE4;
  static char           *BWDATA_CurrentMapFileName       = (char*) 0x0057FD3C;
  static char           *BWDATA_CurrentMapName           = (char*) 0x0057FE40;
  static char           *BWDATA_SaveGameFile             = (char*) 0x00685148;
  const char            *GetStatString(int index);

  extern char           *BWDATA_StringTableOff;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct Supplies
  {
    /** Supply available, used, and maximum for all players and every race */
    struct SuppliesPerRace
    {
      s32 available[PLAYER_COUNT];
      s32 used[PLAYER_COUNT];
      s32 max[PLAYER_COUNT];
    };
    SuppliesPerRace race[RACE_COUNT];
  };
  static Supplies *BWDATA_Supplies = (Supplies*) 0x00582144;

  //---------------------------------------------- UNIT DATA -------------------------------------------------
  static u8  *BWDATA_UnitGraphics         = (u8*)  unitsDat[0].address;
  static u16 *BWDATA_SubUnit1             = (u16*) unitsDat[1].address;
  static u32 *BWDATA_ConstructionGraphics = (u32*) unitsDat[4].address;
  static u8  *BWDATA_ShieldsEnabled       = (u8*)  unitsDat[6].address;
  static u16 *BWDATA_MaxShieldPoints      = (u16*) unitsDat[7].address;
  static s32 *BWDATA_MaxHitPoints         = (s32*) unitsDat[8].address;
  static u8  *BWDATA_Elevation            = (u8*)  unitsDat[9].address;
  static u8  *BWDATA_UnitGroundWeapon     = (u8*)  unitsDat[17].address;
  static u8  *BWDATA_MaxGroundHits        = (u8*)  unitsDat[18].address;
  static u8  *BWDATA_UnitAirWeapon        = (u8*)  unitsDat[19].address;
  static u8  *BWDATA_MaxAirHits           = (u8*)  unitsDat[20].address;
  static u8  *BWDATA_UnitSeekRange        = (u8*)  unitsDat[23].address;
  static u8  *BWDATA_UnitSightRange       = (u8*)  unitsDat[24].address;
  static u8  *BWDATA_UnitUpgrade          = (u8*)  unitsDat[25].address;
  static u8  *BWDATA_UnitSize             = (u8*)  unitsDat[26].address;
  static u8  *BWDATA_Armor                = (u8*)  unitsDat[27].address;
  static u8  *BWDATA_RightClickAction     = (u8*)  unitsDat[28].address;
  static u16 *BWDATA_MineralPrices        = (u16*) unitsDat[40].address;
  static u16 *BWDATA_GasPrices            = (u16*) unitsDat[41].address;
  static u16 *BWDATA_BuildTime            = (u16*) unitsDat[42].address;
  static u8  *BWDATA_SupplyProvided       = (u8*)  unitsDat[45].address;
  static u8  *BWDATA_SupplyRequired       = (u8*)  unitsDat[46].address;
  static u8  *BWDATA_SpaceRequired        = (u8*)  unitsDat[47].address;
  static u8  *BWDATA_SpaceProvided        = (u8*)  unitsDat[48].address;
  static u16 *BWDATA_BuildScore           = (u16*) unitsDat[49].address;
  static u16 *BWDATA_DestroyScore         = (u16*) unitsDat[50].address;
  static u16 *BWDATA_BroodwarOnly         = (u16*) unitsDat[52].address;
  static u16 *BWDATA_AvailabilityFlags    = (u16*) unitsDat[53].address;

  static Util::BitMask<u32> *BWDATA_UnitPrototypeFlags  = (Util::BitMask<u32>*) unitsDat[22].address;
  static Util::BitMask<u8>  *BWDATA_PrototypeGroupFlags = (Util::BitMask<u8>*)  unitsDat[44].address;

  /** Unit Placement Size */
  struct UnitPlacement_type
  {
    struct Placement_Internal_type
    {
      u16 height;
      u16 width;
    } unitType[UNIT_TYPE_COUNT];
  };
  static UnitPlacement_type *BWDATA_UnitPlacement = (UnitPlacement_type*) unitsDat[36].address;

  /** Unit Dimensions; The distance from the 1px 'center' of unit to each border */
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
  static UnitsDimensions_type *BWDATA_UnitDimensions = (UnitsDimensions_type*) unitsDat[38].address;

  //-------------------------------------------- FLINGY DATA -------------------------------------------------
  static u8  *BWDATA_FlingyMovementControl = (u8*)  flingyDat[6].address;
  static u32 *BWDATA_FlingyHaltDistance    = (u32*) flingyDat[3].address;
  static u16 *BWDATA_FlingyAcceleration    = (u16*) flingyDat[2].address;
  static u8  *BWDATA_FlingyTurnRadius      = (u8*)  flingyDat[4].address;
  static u32 *BWDATA_FlingyTopSpeed        = (u32*) flingyDat[1].address;
  static u16 *BWDATA_FlingySpriteFile      = (u16*) flingyDat[0].address;

  //-------------------------------------------- WEAPON DATA -------------------------------------------------
  static u16 *BWDATA_WeaponLabel              = (u16*) weaponsDat[0].address;
  static u32 *BWDATA_WeaponGraphics           = (u32*) weaponsDat[1].address;
  static u32 *BWDATA_WeaponMinRange           = (u32*) weaponsDat[4].address;
  static u32 *BWDATA_WeaponMaxRange           = (u32*) weaponsDat[5].address;
  static u8  *BWDATA_WeaponUpgrade            = (u8*)  weaponsDat[6].address;
  static u8  *BWDATA_WeaponDamageType         = (u8*)  weaponsDat[7].address;
  static u8  *BWDATA_WeaponGraphicalBehavior  = (u8*)  weaponsDat[8].address;
  static u8  *BWDATA_WeaponRemoveAfter        = (u8*)  weaponsDat[9].address;
  static u8  *BWDATA_WeaponExplosionType      = (u8*)  weaponsDat[10].address;
  static u16 *BWDATA_WeaponInnerSplashRadius  = (u16*) weaponsDat[11].address;
  static u16 *BWDATA_WeaponMedianSplashRadius = (u16*) weaponsDat[12].address;
  static u16 *BWDATA_WeaponOuterSplashRadius  = (u16*) weaponsDat[13].address;
  static u16 *BWDATA_WeaponDamageAmount       = (u16*) weaponsDat[14].address;
  static u16 *BWDATA_WeaponDamageBonus        = (u16*) weaponsDat[15].address;
  static u8  *BWDATA_WeaponDamageCooldown     = (u8*)  weaponsDat[16].address;
  static u8  *BWDATA_WeaponDamageFactor       = (u8*)  weaponsDat[17].address;
  static u8  *BWDATA_WeaponAttackDirection    = (u8*)  weaponsDat[18].address;
  static u8  *BWDATA_WeaponLaunchSpin         = (u8*)  weaponsDat[19].address;
  static u8  *BWDATA_WeaponXOffset            = (u8*)  weaponsDat[20].address;
  static u8  *BWDATA_WeaponYOffset            = (u8*)  weaponsDat[21].address;

  static Util::BitMask<u16> *BWDATA_WeaponTargetFlags = (Util::BitMask<u16>*) weaponsDat[3].address;

  //------------------------------------------- UPGRADE DATA -------------------------------------------------
  static u16            *BWDATA_UpgradeLabelIndex        = (u16*)  upgradesDat[8].address;
  static u8             *BWDATA_UpgradeMax               = (u8*)   upgradesDat[10].address;
  static u16            *BWDATA_UpgradeMineralCostBase   = (u16*)  upgradesDat[0].address;
  static u16            *BWDATA_UpgradeMineralCostFactor = (u16*)  upgradesDat[1].address;
  static u16            *BWDATA_UpgradeGasCostBase       = (u16*)  upgradesDat[2].address;
  static u16            *BWDATA_UpgradeGasCostFactor     = (u16*)  upgradesDat[3].address;
  static u16            *BWDATA_UpgradeTimeCostBase      = (u16*)  upgradesDat[4].address;
  static u16            *BWDATA_UpgradeTimeCostFactor    = (u16*)  upgradesDat[5].address;
  static u8             *BWDATA_UpgradeRace              = (u8*)   upgradesDat[9].address;

  struct _scUpgrs {    u8 level[PLAYER_COUNT][46];  };
  struct _bwUpgrs {    u8 level[PLAYER_COUNT][15];  };
  static _scUpgrs *BWDATA_UpgradeLevelSC = (_scUpgrs*)0x0058D2B0;
  static _bwUpgrs *BWDATA_UpgradeLevelBW = (_bwUpgrs*)0x0058F32C;

  static Util::BitMask<u64> *BWDATA_UpgradeProgress = (Util::BitMask<u64>*) 0x0058F3E0;

  //--------------------------------------------- TECH DATA --------------------------------------------------
  static u16 *BWDATA_TechMineralCost  = (u16*) techdataDat[0].address;
  static u16 *BWDATA_TechGasCost      = (u16*) techdataDat[1].address;
  static u16 *BWDATA_TechResearchTime = (u16*) techdataDat[2].address;
  static u16 *BWDATA_TechEnergyCost   = (u16*) techdataDat[3].address;
  static u16 *BWDATA_TechLabelIndex   = (u16*) techdataDat[7].address;

  struct _scTechs {    u8 enabled[PLAYER_COUNT][24];  };
  struct _bwTechs {    u8 enabled[PLAYER_COUNT][20];  };
  static _scTechs *BWDATA_TechResearchSC = (_scTechs*)0x0058CF44;
  static _bwTechs *BWDATA_TechResearchBW = (_bwTechs*)0x0058F140;

  static u32 BWDATA_ResearchProgress  = 0x0058F230;

  //------------------------------------------------ MAPPING -------------------------------------------------
  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  extern TileID     *BWDATA_ZergCreepArray;
  extern TileID     *BWDATA_MapTileArray;
  extern TileType   *BWDATA_TileSet;

  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      Util::BitMask<u16> miniTile[16];
    };
    MiniTileFlagArray tile[0x10000];
  };
  extern MiniTileMaps_type *BWDATA_MiniTileFlags;
  extern u32               *BWDATA_MapFogOfWar;
  extern SAI_Paths         *BWDATA_SAIPathing;
};
#pragma pack()
