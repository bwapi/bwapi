#pragma once
#pragma pack(1)

#include <windows.h>
#include <Util/Types.h>
#include "../Storm/storm.h"

#include "Position.h"
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
  class  Unit;
  struct Bullet;
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
  static DatLoad *upgradesDat = (DatLoad*)0x005136E0;  /** 1.15.3, 1.16, 1.16.1 */
  static DatLoad *techdataDat = (DatLoad*)0x005137D8;
  static DatLoad *weaponsDat  = (DatLoad*)0x00513868;
  static DatLoad *unitsDat    = (DatLoad*)0x00513C30;
  static DatLoad *flingyDat   = (DatLoad*)0x00515A38;

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

  /** Code Patches */
  static u32 BWDATA_MenuLoadHack      = 0x004DE392;
  static u32 BWDATA_MenuInHack        = 0x004DD76E;
  static u32 BWDATA_MenuOutHack       = 0x004DD162;
  static u32 BWDATA_MultiplayerHack   = 0x004DD5A2;
  static u32 BWDATA_MultiplayerHack2  = 0x004DD5C9;
  static u32 BWDATA_OpponentStartHack = 0x004B995D;

  /** QueueGameCommand data */
  static u32    *BWDATA_sgdwBytesInCmdQueue = (u32*)      0x00654AA0;
  static u8     *BWDATA_TurnBuffer          = (u8*)       0x00654880;
  static void  (*BWFXN_sendTurn)()          = (void(*)()) 0x00485A40;
  static u32    BWFXN_QueueCommand          =             0x00485BD0;

  /** Speed & Latency */
  static u32     OriginalSpeedModifiers[7]  =        { 167, 111, 83, 67, 56, 48, 42};
  static u32     *BWDATA_GameSpeedModifiers = (u32*) 0x005124D8;
  static u32     *BWDATA_LatencyFrames      = (u32*) 0x0051CE70;
  static u32     *BWDATA_GameSpeed          = (u32*) 0x006CDFD4;
  static int     *BWDATA_FrameSkip          = (int*) 0x005124D4;
  static u8      *BWDATA_Latency            = (u8*)  0x006556e4;
  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  /** Dialog and drawing offsets */
  static bool (__fastcall **BWDATA_GenericDlgInteractFxns)(dialog*,dlgEvent*)   = (bool (__fastcall**)(dialog*,dlgEvent*))    0x005014AC;
  static void (__fastcall **BWDATA_GenericDlgUpdateFxns)(dialog*,int,int,rect*) = (void (__fastcall**)(dialog*,int,int,rect*))0x00501504;

  static dialog  **BWDATA_DialogList       = (dialog**) 0x006D5E34;
  static fntHead **BWDATA_FontBase         = (fntHead**)0x006CE0F4;
  static bitmap  *BWDATA_GameScreenBuffer  = (bitmap*)  0x006CEFF0;
  static bitmap  *BWDATA_GameScreenConsole = (bitmap*)  0x00597240;
  
  static PALETTEENTRY        *BWDATA_GamePalette    = (PALETTEENTRY*)       0x006CE320;
  static LPDIRECTDRAWSURFACE *BWDATA_PrimarySurface = (LPDIRECTDRAWSURFACE*)0x006D5E00;
  static LPDIRECTDRAW        *BWDATA_DDInterface    = (LPDIRECTDRAW*)       0x006D5E08;
  static LPDIRECTDRAWPALETTE *BWDATA_PrimaryPalette = (LPDIRECTDRAWPALETTE*)0x006D5E0C;
  static LPDIRECTDRAWSURFACE *BWDATA_BackSurface    = (LPDIRECTDRAWSURFACE*)0x006D5E10;

  struct bltMask
  {
    bltMask *prev;
    bltMask *next;
    HANDLE  hTrans;
    RECT    info;
    DWORD   dwReserved;
  };
  static bltMask **BWDATA_MainBltMask = (bltMask**)0x00597238;

  static void (__cdecl *BWFXN_DDrawDestroy)()       = (void(__cdecl*)())0x0041D8B0;
  static void (__cdecl *BWFXN_UpdateBltMasks)()     = (void(__cdecl*)())0x0041D470;

  static u8 *BWDATA_RefreshRegions = (u8*)0x006CEFF8;
  static u8 *BWDATA_PlayerColors   = (u8*)0x00581DD6;

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
  static Unit **BWDATA_UnitNodeList_VisibleUnit_First  = (Unit**)0x00628430;
  static Unit **BWDATA_UnitNodeList_HiddenUnit_First   = (Unit**)0x006283EC;
  static Unit **BWDATA_UnitNodeList_ScannerSweep_First = (Unit**)0x006283F4;
  static Unit *BWDATA_UnitNodeTable                    = (Unit*) 0x0059CCA8;

  static Bullet **BWDATA_BulletNodeTable_FirstElement = (Bullet**)0x0064DEAC;
  static Bullet **BWDATA_BulletNodeTable_LastElement  = (Bullet**)0x0064DEC4;
  static Bullet *BWDATA_BulletNodeTable               = (Bullet*) 0x0064B2E8;

  //------------------------------------------- DATA LEVEL ---------------------------------------------------
  /* Mode Stuff */
  static u8             *BWDATA_gameType       = (u8*)  0x00596820;
  static u32            *BWDATA_InGame         = (u32*) 0x006556E0;
  static u32            *BWDATA_InReplay       = (u32*) 0x006D0F14;
  static int            *BWDATA_NetMode        = (int*) 0x0059688C;
  static u8             *BWDATA_IsRunning      = (u8*)  0x0051CE6C;

  // note: unaligned because it's part of a structure that we're not using
  static u32            *BWDATA_ReplayFrames   = (u32*) 0x006D0F31;

  static u8             *BWDATA_GameState      = (u8*)  0x006D11EC;
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

  static u32 BWFXN_NetSelectReturnMenu   = 0x004DC5B0;

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl *BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;
  static u32            *BWDATA_MoveToX                  = (u32*)             0x0062848C;
  static u32            *BWDATA_MoveToY                  = (u32*)             0x006284A8;
  static Position       *BWDATA_MoveToTile               = (Position*)        0x0057F1D0;

  static POINT          *BWDATA_Mouse                    = (POINT*) 0x006CDDC4; // @TODO: No-Offset method of getting this
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
  static AllScores *BWDATA_AllScores = (AllScores*) 0x00581DE4;

  //---------------------------------------------- UNIT DATA -------------------------------------------------
  static u8  *BWDATA_Unit_Graphic             = (u8*)  unitsDat[0].address;
  static u16 *BWDATA_Unit_SubUnit             = (u16*) unitsDat[1].address;
  static u32 *BWDATA_Unit_ConstructionGraphic = (u32*) unitsDat[4].address;
  static u8  *BWDATA_Unit_ShieldsEnabled      = (u8*)  unitsDat[6].address;
  static u16 *BWDATA_Unit_MaxShieldPoints     = (u16*) unitsDat[7].address;
  static s32 *BWDATA_Unit_MaxHitPoints        = (s32*) unitsDat[8].address;
  static u8  *BWDATA_Unit_Elevation           = (u8*)  unitsDat[9].address;
  static u8  *BWDATA_Unit_GroundWeapon        = (u8*)  unitsDat[17].address;
  static u8  *BWDATA_Unit_MaxGroundHits       = (u8*)  unitsDat[18].address;
  static u8  *BWDATA_Unit_AirWeapon           = (u8*)  unitsDat[19].address;
  static u8  *BWDATA_Unit_MaxAirHits          = (u8*)  unitsDat[20].address;
  static u32 *BWDATA_Unit_PrototypeFlags      = (u32*) unitsDat[22].address;
  static u8  *BWDATA_Unit_SeekRange           = (u8*)  unitsDat[23].address;
  static u8  *BWDATA_Unit_SightRange          = (u8*)  unitsDat[24].address;
  static u8  *BWDATA_Unit_ArmorUpgrade        = (u8*)  unitsDat[25].address;
  static u8  *BWDATA_Unit_SizeType            = (u8*)  unitsDat[26].address;
  static u8  *BWDATA_Unit_ArmorAmount         = (u8*)  unitsDat[27].address;
  static u16 *BWDATA_Unit_MineralCost         = (u16*) unitsDat[40].address;
  static u16 *BWDATA_Unit_GasCost             = (u16*) unitsDat[41].address;
  static u16 *BWDATA_Unit_TimeCost            = (u16*) unitsDat[42].address;
  static u8  *BWDATA_Unit_GroupFlags          = (u8*)  unitsDat[44].address;
  static u8  *BWDATA_Unit_SupplyProvided      = (u8*)  unitsDat[45].address;
  static u8  *BWDATA_Unit_SupplyRequired      = (u8*)  unitsDat[46].address;
  static u8  *BWDATA_Unit_SpaceRequired       = (u8*)  unitsDat[47].address;
  static u8  *BWDATA_Unit_SpaceProvided       = (u8*)  unitsDat[48].address;
  static u16 *BWDATA_Unit_BuildScore          = (u16*) unitsDat[49].address;
  static u16 *BWDATA_Unit_DestroyScore        = (u16*) unitsDat[50].address;
  static u16 *BWDATA_Unit_BroodwarOnly        = (u16*) unitsDat[52].address;

  /** Unit Placement Size */
  struct UnitPlacement_type
  {
    struct Placement_Internal_type
    {
      u16 height;
      u16 width;
    } unitType[UNIT_TYPE_COUNT];
  };
  static UnitPlacement_type *BWDATA_Unit_Placement = (UnitPlacement_type*) unitsDat[36].address;

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
  static UnitsDimensions_type *BWDATA_Unit_Dimensions = (UnitsDimensions_type*) unitsDat[38].address;

  //-------------------------------------------- FLINGY DATA -------------------------------------------------
  static u16 *BWDATA_Flingy_SpriteID        = (u16*) flingyDat[0].address;
  static u32 *BWDATA_Flingy_TopSpeed        = (u32*) flingyDat[1].address;
  static u16 *BWDATA_Flingy_Acceleration    = (u16*) flingyDat[2].address;
  static u32 *BWDATA_Flingy_HaltDistance    = (u32*) flingyDat[3].address;
  static u8  *BWDATA_Flingy_TurnRadius      = (u8*)  flingyDat[4].address;
  static u8  *BWDATA_Flingy_MovementControl = (u8*)  flingyDat[6].address;

  //-------------------------------------------- WEAPON DATA -------------------------------------------------
  static u16 *BWDATA_Weapon_Label              = (u16*) weaponsDat[0].address;
  static u32 *BWDATA_Weapon_Graphic            = (u32*) weaponsDat[1].address;
  static u16 *BWDATA_Weapon_TargetFlags        = (u16*) weaponsDat[3].address;
  static u32 *BWDATA_Weapon_MinRange           = (u32*) weaponsDat[4].address;
  static u32 *BWDATA_Weapon_MaxRange           = (u32*) weaponsDat[5].address;
  static u8  *BWDATA_Weapon_Upgrade            = (u8*)  weaponsDat[6].address;
  static u8  *BWDATA_Weapon_DamageType         = (u8*)  weaponsDat[7].address;
  static u8  *BWDATA_Weapon_Behavior           = (u8*)  weaponsDat[8].address;
  static u8  *BWDATA_Weapon_RemoveAfter        = (u8*)  weaponsDat[9].address;
  static u8  *BWDATA_Weapon_ExplosionType      = (u8*)  weaponsDat[10].address;
  static u16 *BWDATA_Weapon_InnerSplashRadius  = (u16*) weaponsDat[11].address;
  static u16 *BWDATA_Weapon_MedianSplashRadius = (u16*) weaponsDat[12].address;
  static u16 *BWDATA_Weapon_OuterSplashRadius  = (u16*) weaponsDat[13].address;
  static u16 *BWDATA_Weapon_DamageAmount       = (u16*) weaponsDat[14].address;
  static u16 *BWDATA_Weapon_DamageBonus        = (u16*) weaponsDat[15].address;
  static u8  *BWDATA_Weapon_DamageCooldown     = (u8*)  weaponsDat[16].address;
  static u8  *BWDATA_Weapon_DamageFactor       = (u8*)  weaponsDat[17].address;
  static u8  *BWDATA_Weapon_AttackDirection    = (u8*)  weaponsDat[18].address;
  static u8  *BWDATA_Weapon_LaunchSpin         = (u8*)  weaponsDat[19].address;
  static u8  *BWDATA_Weapon_XOffset            = (u8*)  weaponsDat[20].address;
  static u8  *BWDATA_Weapon_YOffset            = (u8*)  weaponsDat[21].address;

  //------------------------------------------- UPGRADE DATA -------------------------------------------------
  static u16            *BWDATA_Upgrade_MineralCostBase   = (u16*)  upgradesDat[0].address;
  static u16            *BWDATA_Upgrade_MineralCostFactor = (u16*)  upgradesDat[1].address;
  static u16            *BWDATA_Upgrade_GasCostBase       = (u16*)  upgradesDat[2].address;
  static u16            *BWDATA_Upgrade_GasCostFactor     = (u16*)  upgradesDat[3].address;
  static u16            *BWDATA_Upgrade_TimeCostBase      = (u16*)  upgradesDat[4].address;
  static u16            *BWDATA_Upgrade_TimeCostFactor    = (u16*)  upgradesDat[5].address;
  static u16            *BWDATA_Upgrade_Label             = (u16*)  upgradesDat[8].address;
  static u8             *BWDATA_Upgrade_Race              = (u8*)   upgradesDat[9].address;
  static u8             *BWDATA_Upgrade_MaxRepeats        = (u8*)   upgradesDat[10].address;

  struct _scUpgrs {    u8 level[PLAYER_COUNT][46];  };
  struct _bwUpgrs {    u8 level[PLAYER_COUNT][15];  };
  static _scUpgrs *BWDATA_UpgradeLevelSC = (_scUpgrs*)0x0058D2B0;
  static _bwUpgrs *BWDATA_UpgradeLevelBW = (_bwUpgrs*)0x0058F32C;

  static u32 BWDATA_UpgradeProgress = 0x0058F3E0;

  //--------------------------------------------- TECH DATA --------------------------------------------------
  static u16 *BWDATA_Tech_MineralCost  = (u16*) techdataDat[0].address;
  static u16 *BWDATA_Tech_GasCost      = (u16*) techdataDat[1].address;
  static u16 *BWDATA_Tech_TimeCost     = (u16*) techdataDat[2].address;
  static u16 *BWDATA_Tech_EnergyCost   = (u16*) techdataDat[3].address;
  static u16 *BWDATA_Tech_LabelIndex   = (u16*) techdataDat[7].address;

  struct _scTechs {    u8 enabled[PLAYER_COUNT][24];  };
  struct _bwTechs {    u8 enabled[PLAYER_COUNT][20];  };
  static _scTechs *BWDATA_TechResearchSC = (_scTechs*)0x0058CF44;
  static _bwTechs *BWDATA_TechResearchBW = (_bwTechs*)0x0058F140;

  static u32 BWDATA_ResearchProgress  = 0x0058F230;

  //------------------------------------------------ MAPPING -------------------------------------------------
  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  extern TileID   *BWDATA_ZergCreepArray;
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
  extern u32               *BWDATA_ActiveTileArray;
  extern SAI_Paths         *BWDATA_SAIPathing;
};
#pragma pack()
