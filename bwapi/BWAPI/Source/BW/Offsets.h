#pragma once
#pragma pack(1)

#include <windows.h>
#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include "UnitPrototypeFlags.h"
#include "WeaponTargetFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"
#include "PlayerType.h"
#include "Race.h"
#include "Sprite.h"
#include "Dialog.h"

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */
namespace BW
{
  static const u8  PLAYER_COUNT          =  12;
  static const u8  PLAYABLE_PLAYER_COUNT =   8;
  static const u8  RACE_COUNT            =   3;
  static const u8  UNIT_TYPE_COUNT       = 228;
  static const u8  TECH_TYPE_COUNT       =  44;
  static const u8  UPGRADE_TYPE_COUNT    =  61;
  static const u8  WEAPON_TYPE_COUNT     = 130;
  static const u8  DAMAGE_TYPE_COUNT     =   5;
  static const u8  EXPLOSION_TYPE_COUNT  =  25;
  static const u8  FLINGY_TYPE_COUNT     = 209;

  static const int TILE_SIZE          =  32;

  struct Unit;
  struct UnitArray;
  struct Bullet;
  struct BulletArray;
  class TileType;
  class DoodatType;
  class dialog;
  struct dlgEvent;
  struct rect;
  struct bitmap;
  struct fntHead;
  //-------------------------------------------- PLAYER RESOURCES --------------------------------------------
  /** Direct mapping of players resource amount in the bw memory */
  struct PlayerResources
  {
    /** Direct mapping of resource value of some player (gas or minerals) */
    struct PlayerResource
    {
      s32 player[PLAYER_COUNT];
    };
    PlayerResource minerals;
    PlayerResource gas;
    PlayerResource cumulativeGas;
    PlayerResource cumulativeMinerals;
  };
  static PlayerResources* BWDATA_PlayerResources = (PlayerResources*) 0x0057F0F0;
  struct PlayerVictory
  {
    u8 player[8];
  };
  static PlayerVictory* BWDATA_PlayerVictory               = (PlayerVictory*) 0x00650974;

  //----------------------------------------------- NAMES TYPE -----------------------------------------------
  /** Direct mapping of players info in bw memory */
  struct Players
  {
    /** Direct mapping of player info in bw memory */
    struct PlayerInfo
    {
      u32  dwPlayerID;
      u32  dwStormId;
      u8   nType;
      u8   nRace;
      u8   nTeam;
      char szName[25];
    };
    PlayerInfo  player[PLAYER_COUNT];
  };

  struct Positions
  {
    u16   x;
    u16   y;
  };

  struct ForceName
  {
    char  name[30];
  };

  struct DatLoad
  {
    u32   address;
    u32   length;
    u32   entries;
  };

  struct PlayerAlliance
  {
    struct Alliances
    {
      u8 player[PLAYER_COUNT];
    };
    Alliances alliance[PLAYER_COUNT];
  };


  static DatLoad*       upgradesDat       = (DatLoad*)        0x005136E0;  // 1.15.3, 1.16, 1.16.1
  static DatLoad*       techdataDat       = (DatLoad*)        0x005137D8;
  static DatLoad*       weaponsDat        = (DatLoad*)        0x00513868;
  static DatLoad*       unitsDat          = (DatLoad*)        0x00513C30;
  static DatLoad*       flingyDat         = (DatLoad*)        0x00515A38;

  static Positions*      BWDATA_startPositions   = (Positions*)      0x0058D720;  // 1.16.1
  static ForceName*      BWDATA_ForceNames       = (ForceName*)      0x0058D5BC;  // 1.16.1
  static Players*        BWDATA_Players          = (Players*)        0x0057EEE0;  // 1.16.1
  static PlayerAlliance* BWDATA_Alliance         = (PlayerAlliance*) 0x0058D634;  // 1.16.1

  //----------------------------------------------- UNIT STATS -----------------------------------------------
  /** Direct mapping of BW tables used for unit stats like count, completed, kills, deaths */
  struct Counts
  {
    struct UnitStats
    {
      struct PlayerStats
      {
        s32 player[PLAYER_COUNT];
      };
      PlayerStats unit[BW::UNIT_TYPE_COUNT];
    };
    UnitStats all;
    UnitStats completed;
    UnitStats killed;
    UnitStats dead;
  };

//  static int *listcount = (int*)0x64DEBC;
  static Bullet**       BWDATA_BulletNodeTable_FirstElement = (Bullet**)     0x0064DEAC;
  static Bullet**       BWDATA_BulletNodeTable_LastElement  = (Bullet**)     0x0064DEC4;
  static BulletArray*   BWDATA_BulletNodeTable              = (BulletArray*) 0x0064B2E8;
  const  u32            BULLET_ARRAY_MAX_LENGTH             = 100;

  static Counts* BWDATA_Counts             = (Counts*) 0x00582324;    // 1.16.1
  static u8*     BWDATA_gameType           = (u8*)     0x00596820;    // 1.16.1
  static u32     BWDATA_LoadedUnits        = 0x004F4B58;

  /* Speed Hacks */
  static u32     BWDATA_MenuLoadHack       =           0x004DE392;
  static u32     BWDATA_MenuInHack         =           0x004DD76E;
  static u32     BWDATA_MenuOutHack        =           0x004DD162;
  static u32     BWDATA_MultiplayerHack    =           0x004DD5A2;
  static u32     BWDATA_MultiplayerHack2   =           0x004DD5C9;
  static u32*    BWDATA_GameSpeedModifiers = (u32*)    0x005124F4;
  static u32     BWDATA_OpponentStartHack  =           0x004B995D;
  static dialog  **BWDATA_DialogList       = (dialog**)0x006D5E34;

  static bool (__fastcall **BWDATA_GenericDlgInteractFxns)(dialog*,dlgEvent*)   = (bool (__fastcall**)(dialog*,dlgEvent*))0x005014AC;
  static void (__fastcall **BWDATA_GenericDlgUpdateFxns)(dialog*,int,int,rect*) = (void (__fastcall**)(dialog*,int,int,rect*))0x00501504;

  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  extern TileID     *BWDATA_ZergCreepArray;
  extern TileID     *BWDATA_MapTileArray;
  extern TileType   *BWDATA_TileSet;
  extern DoodatType *BWDATA_DoodatSet;

  static Positions      *BWDATA_MapSize                   = (Positions*)   0x0057F1D4;
  static Unit**         BWDATA_UnitNodeTable_FirstElement = (Unit**)       0x00628430;               // @TODO: Verify; old: 0x00628410;
  static Unit**         BWDATA_UnitNodeTable_PlayerFirstUnit = (Unit**)    0x0062843C;               // Haven't found the right offset yet. Should point to the first unit of the first player (player 1).
  static UnitArray*     BWDATA_UnitNodeTable              = (UnitArray*)   0x0059CCA8;
  const  u32            UNIT_ARRAY_MAX_LENGTH             = 1700;

  static u8*            BWDATA_Latency                    = (u8*)          0x006556e4;
  static void (_stdcall* selectUnits)(int count, BW::Unit** unitsToSelect) = (void (_stdcall*)(int,BW::Unit**))0x004C0860;
  static u32            BWFXN_OldIssueCommand                     =         0x00485BD0;
  static u32*           BWDATA_InGame                             = (u32*)  0x006556E0;
  static u32*           BWDATA_InReplay                           = (u32*)  0x006D0F14;
  static u8*            BWDATA_IsMultiplayer                      = (u8*)   0x0065fbf0;
  static u8*            BWDATA_IsBattleNet                        = (u8*)   0x006D5ED0;
  static u8*            BWDATA_IsNotPaused                        = (u8*)   0x0051CE6C;

  static u8*            BWDATA_GameState                          = (u8*)  0x006D11EC;
  static u16*           BWDATA_GamePosition                       = (u16*) 0x0051CE90;

  static u16*           BWDATA_gwGameMode                         = (u16*) 0x00596904;
  /*
    GAME_INTRO          = 0,  // guessed

    GAME_RUN            = 3,  // official -- begins game
    GAME_GLUES          = 4,  // official -- uses glGluesMode

    GAME_CREDITS        = 8,  // guessed
    GAME_EPILOG         = 9,  // guessed
    GAME_CINEMATIC      = 10  // guessed
  */
  static u32*           BWDATA_glGluesMode                        = (u32*) 0x006D11BC;
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

  extern BOOL (__stdcall *SNetReceiveMessage)(int *senderplayerid, u8 **data, int *databytes);
  extern BOOL (__stdcall *SCodeDelete)(HANDLE handle);
  extern int  (__stdcall *SStrCopy)(char *dest, const char *source, size_t size);
  extern void* (__stdcall *SMemAlloc)(int amount, char *logfilename, int logline, int defaultValue);
  extern BOOL (__stdcall *SFileOpenFileEx)(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);

  static u32            BWFXN_PrintText                           = 0x0048D1C0;
  static u16*           BWDATA_SendTextFilter                     = (u16*)  0x0057F1DA;
  static char*          BWDATA_CurrentPlayer                      = (char*) 0x0057EE9C;            // 1.16.1

  static u32            BWFXN_SendPublicCallTarget                = 0x004C2420;
  static u32            BWFXN_SendLobbyCallTarget                 = 0x004707D0;

  static Unit*          BWDATA_CurrentPlayerSelectionGroup        = (Unit*) 0x00597208;

  static u32            BWFXN_NextFrameHelperFunction             = 0x004D98BD;
  static u32            BWFXN_NextFrameHelperFunctionBack         = BWFXN_NextFrameHelperFunction + 5;
  static u32            BWFXN_NextFrameHelperFunctionTarget       = 0x004D14D0;

  static u32            BWFXN_PrintXY                             = 0x004200D0;
  static u32*           BWDATA_PrintXY_PositionX                  = (u32*) 0x006CE108;
  static u32*           BWDATA_PrintXY_PositionY                  = (u32*) 0x006CE0E0;
  static void**         BWDATA_PrintXY_Current_Font               = (void**) 0x006D5DDC;

  struct fontMemStruct
  {
    u32 tFontData;
    u32 tFontUnknown;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
  };

  static fontMemStruct  *BWDATA_PrintXY_Font                      = (fontMemStruct*) 0x006CE0C0;
  static u8             *BWDATA_PrintXY_Size                      = (u8*)            0x006CE110;

  static u32            BWDATA_FontData                           =             0x006CE028;
  static fntHead        **BWDATA_FontBase                         = (fntHead**) 0x006CE0F4;
/*
  static u32            BWFXN_Refresh                             = 0x0041E26B;
  static u32            BWFXN_RefreshTarget                       = 0x0041E0D0;
  static u32            BWFXN_RefreshBack                         = BWFXN_Refresh + 5;
*/
  static u32            BWFXN_DrawHigh                            = 0x004BD614;
  static u32            BWFXN_DrawHighBack                        = BWFXN_DrawHigh + 5;
  static u32            BWFXN_DrawHighTarget                      = 0x0048CF60;

  static u32            BWFXN_NewIssueCommand                     = 0x00485BD9;

  static void(__thiscall *BWFXN_PlayIscript)(Image::CImage *_this, char *header,int unk1,int unk2)    = (void(__thiscall*)(Image::CImage*,char*,int,int))0x004D74C0;
  static u32            BWFXN_IscriptHook                         = 0x004D84F3;

  static int*           BWDATA_NextLogicFrameData                 = (int*)0x006509C4;
  static u32            BWFXN_NextLogicFrame                      = 0x004D974E;

  static void (__stdcall *BWFXN_videoLoop)(int flag)              = (void(__stdcall*)(int))0x004D1BF0;
  static u32            BWFXN_NextMenuFrame                       = 0x0041A0D3;

  static u32            BWDATA_SaveGameFile                       = 0x00685148;

  static u32            BWDATA_UpgradeLevelSC                     = 0x0058D2B0;
  static u32            BWDATA_UpgradeLevelBW                     = 0x0058F32C;
  static u32            BWDATA_TechResearchSC                     = 0x0058CF44;
  static u32            BWDATA_TechResearchBW                     = 0x0058F140;
  static u32            BWDATA_PlayerSelection                    = 0x006284E0;

  static void (__cdecl *BWFXN_UpdateScreenPosition)()             = (void(__cdecl*)())0x0049BFD0;
  static u32*           BWDATA_MoveToX                            = (u32*)0x0062848C;
  static u32*           BWDATA_MoveToY                            = (u32*)0x006284A8;
  static Positions      *BWDATA_MoveToTile                        = (Positions*)0x0057F1D0;

  static POINT          *BWDATA_Mouse                             = (POINT*)0x006CDDC4;
  static u32*           BWDATA_ScreenX                            = (u32*)0x00628448;
  static u32*           BWDATA_ScreenY                            = (u32*)0x00628470;

  static char*          BWDATA_menuMapFileName                    = (char*)       0x0059BC88;
  static char*          BWDATA_menuMapRelativePath                = (char*)       0x0059B850;
  static int*           BWDATA_menuStuff                          = (int*)        0x0057EEE4;
  static char*          BWDATA_CurrentMapFileName                 = (char*)       0x0057FD3C;
  static char*          BWDATA_CurrentMapName                     = (char*)       0x0057FE40;
  static u16*           BWDATA_TechLabelIndex                     = (u16*)        techdataDat[7].address;
  static u16*           BWDATA_UpgradeLabelIndex                  = (u16*)        upgradesDat[8].address;
  static u8*            BWDATA_UpgradeMax                         = (u8*)         upgradesDat[10].address;
  static u16*           BWDATA_UpgradeMineralCostBase             = (u16*)        upgradesDat[0].address;
  static u16*           BWDATA_UpgradeMineralCostFactor           = (u16*)        upgradesDat[1].address;
  static u16*           BWDATA_UpgradeGasCostBase                 = (u16*)        upgradesDat[2].address;
  static u16*           BWDATA_UpgradeGasCostFactor               = (u16*)        upgradesDat[3].address;
  static u16*           BWDATA_UpgradeTimeCostBase                = (u16*)        upgradesDat[4].address;
  static u16*           BWDATA_UpgradeTimeCostFactor              = (u16*)        upgradesDat[5].address;
  static u8*            BWDATA_UpgradeRace                        = (u8*)         upgradesDat[9].address;

  const char *GetStatString(int index);

  extern char *BWDATA_StringTableOff;
  extern u32  *BWDATA_MapFogOfWar;

  struct UpgradeProgress
  {
    Util::BitMask<u64> player[PLAYER_COUNT];
  };
  static UpgradeProgress* BWDATA_UpgradeProgress  = (UpgradeProgress*) 0x0058F3E0;
  static u32 BWDATA_ResearchProgress              =                    0x0058F230;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct Supplies
  {
    /** Direct mapping of available/Used/Max supplies for one race and all players */
    struct SuppliesPerRace
    {
      /** Direct mapping of some supply vale for all players*/
      struct SupplyValues
      {
        s32 player[PLAYER_COUNT];
      };
      SupplyValues available;
      SupplyValues used;
      SupplyValues max;
    };
    SuppliesPerRace race[RACE_COUNT];
  };
  static Supplies* BWDATA_Supplies    = (Supplies*) 0x00582144;

  struct unitsDat_u8_type
  {
    u8 unitType[UNIT_TYPE_COUNT];
  };
  struct unitsDat_u16_type
  {
    u16 unitType[UNIT_TYPE_COUNT];
  };
  struct unitsDat_u32_type
  {
    u32 unitType[UNIT_TYPE_COUNT];
  };
  struct unitsDat_s32_type
  {
    s32 unitType[UNIT_TYPE_COUNT];
  };


  //---------------------------------------------- UNIT GRAPHICS ---------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitGraphics = (unitsDat_u8_type*) unitsDat[0].address;
  //----------------------------------------------- SUB UNIT 1 -----------------------------------------------
  static unitsDat_u16_type* BWDATA_SubUnit1 = (unitsDat_u16_type*) unitsDat[1].address;
  //----------------------------------------------- SUB UNIT 2 -----------------------------------------------
  static unitsDat_u16_type* BWDATA_SubUnit2 = (unitsDat_u16_type*) unitsDat[2].address;


  //--------------------------------------- UNIT CONSTRUCTION GRAPHICS ---------------------------------------
  static unitsDat_u32_type* BWDATA_ConstructionGraphics = (unitsDat_u32_type*) unitsDat[4].address;
  //----------------------------------------------- DIRECTION ------------------------------------------------
  static unitsDat_u8_type* BWDATA_Direction = (unitsDat_u8_type*) unitsDat[5].address;
  //--------------------------------------------- SHIELDS ENABLED --------------------------------------------
  static unitsDat_u8_type* BWDATA_ShieldsEnabled = (unitsDat_u8_type*) unitsDat[6].address;
  //-------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  static unitsDat_u16_type* BWDATA_MaxShieldPoints = (unitsDat_u16_type*) unitsDat[7].address;
  //---------------------------------------------- UNIT MAX HP -----------------------------------------------
  static unitsDat_s32_type* BWDATA_MaxHitPoints = (unitsDat_s32_type*) unitsDat[8].address;
  //--------------------------------------------- ELEVATION ----------------------------------------------
  static unitsDat_u8_type* BWDATA_Elevation = (unitsDat_u8_type*) unitsDat[9].address;


  //--------------------------------------------- UNIT SUBLABEL ----------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitSubLabel = (unitsDat_u8_type*) unitsDat[11].address;


  //------------------------------------------ UNIT GROUND WEAPON --------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitGroundWeapon = (unitsDat_u8_type*) unitsDat[17].address;
  //-------------------------------------------- MAX GROUND HITS ---------------------------------------------
  static unitsDat_u8_type* BWDATA_MaxGroundHits = (unitsDat_u8_type*) unitsDat[18].address;
  //------------------------------------------- UNIT AIR WEAPON ----------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitAirWeapon = (unitsDat_u8_type*) unitsDat[19].address;
  //---------------------------------------------- MAX AIR HITS ----------------------------------------------
  static unitsDat_u8_type* BWDATA_MaxAirHits = (unitsDat_u8_type*) unitsDat[20].address;
  //-------------------------------------------- UNIT AI INTERVAL --------------------------------------------
  static unitsDat_u8_type* BWDATA_AIInterval = (unitsDat_u8_type*) unitsDat[21].address;
  //------------------------------------------------- FLAGS --------------------------------------------------
  struct PrototypeFlags_type
  {
    Util::BitMask<u32> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeFlags_type* BWDATA_UnitPrototypeFlags = (PrototypeFlags_type*) unitsDat[22].address;
  //-------------------------------------------- UNIT SEEK RANGE ---------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitSeekRange = (unitsDat_u8_type*) unitsDat[23].address;
  //-------------------------------------------- UNIT SIGHT RANGE --------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitSightRange = (unitsDat_u8_type*) unitsDat[24].address;
  //---------------------------------------------- UNIT UPGRADE ----------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitUpgrade = (unitsDat_u8_type*) unitsDat[25].address;
  //----------------------------------------------- UNIT SIZE ------------------------------------------------
  static unitsDat_u8_type* BWDATA_UnitSize = (unitsDat_u8_type*) unitsDat[26].address;
  //---------------------------------------------- UNIT ARMOR ------------------------------------------------
  static unitsDat_u8_type* BWDATA_Armor = (unitsDat_u8_type*) unitsDat[27].address;
  //----------------------------------------- UNIT RIGHT CLICK ACTION ----------------------------------------
  static unitsDat_u8_type* BWDATA_RightClickAction = (unitsDat_u8_type*) unitsDat[28].address;
  //-------------------------------------------- UNIT READY SOUND --------------------------------------------
  static unitsDat_u16_type* BWDATA_ReadySound = (unitsDat_u16_type*) unitsDat[29].address;
  //------------------------------------------ UNIT FIRST WHAT SOUND -----------------------------------------
  static unitsDat_u16_type* BWDATA_FirstWhatSound = (unitsDat_u16_type*) unitsDat[30].address;
  //------------------------------------------ UNIT LAST WHAT SOUND ------------------------------------------
  static unitsDat_u16_type* BWDATA_LastWhatSound = (unitsDat_u16_type*) unitsDat[31].address;
  //---------------------------------------- UNIT FIRST ANNOYED SOUND ----------------------------------------
  static unitsDat_u16_type* BWDATA_FirstAnnoyedSound = (unitsDat_u16_type*) unitsDat[32].address;
  //----------------------------------------- UNIT LAST ANNOYED SOUND ----------------------------------------
  static unitsDat_u16_type* BWDATA_LastAnnoyedSound = (unitsDat_u16_type*) unitsDat[33].address;
  //------------------------------------------- UNIT FIRST YES SOUND -----------------------------------------
  static unitsDat_u16_type* BWDATA_FirstYesSound = (unitsDat_u16_type*) unitsDat[34].address;
  //------------------------------------------- UNIT LAST YES SOUND ------------------------------------------
  static unitsDat_u16_type* BWDATA_LastYesSound = (unitsDat_u16_type*) unitsDat[35].address;
  //--------------------------------------------- UNIT PLACEMENT ---------------------------------------------
  struct UnitPlacement_type
  {
    struct Placement_Internal_type
    {
      u16 height;
      u16 width;
    };
    Placement_Internal_type unitType[UNIT_TYPE_COUNT];
  };
  static UnitPlacement_type* BWDATA_UnitPlacement = (UnitPlacement_type*) unitsDat[36].address;


  //-------------------------------------------- UNIT DIMENSIONS ---------------------------------------------
  struct UnitsDimensions_type
  {
    /** Dimensions of unit, it's the distance from the 'center' of unit to each border */
    struct UnitDimensions
    {
      u16 left;
      u16 up;
      u16 right;
      u16 down;
    };
    UnitDimensions units[UNIT_TYPE_COUNT];
  };
  static UnitsDimensions_type* BWDATA_UnitDimensions = (UnitsDimensions_type*) unitsDat[38].address;
  //------------------------------------------- UNIT IDLE PORTRAIT -------------------------------------------
  static unitsDat_u16_type* BWDATA_IdlePortrait = (unitsDat_u16_type*) unitsDat[39].address;
  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  static unitsDat_u16_type* BWDATA_MineralPrices = (unitsDat_u16_type*) unitsDat[40].address;
  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  static unitsDat_u16_type* BWDATA_GasPrices = (unitsDat_u16_type*) unitsDat[41].address;
  //-------------------------------------------- UNIT BUILD TIME ---------------------------------------------
  static unitsDat_u16_type* BWDATA_BuildTime = (unitsDat_u16_type*) unitsDat[42].address;


  //---------------------------------------------- GROUP FLAGS -----------------------------------------------
  struct PrototypeGroupFlags_type
  {
    Util::BitMask<u8> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeGroupFlags_type* BWDATA_PrototypeGroupFlags = (PrototypeGroupFlags_type*) unitsDat[44].address;
  //------------------------------------------ UNIT SUPPLY PROVIDED ------------------------------------------
  static unitsDat_u8_type* BWDATA_SupplyProvided = (unitsDat_u8_type*) unitsDat[45].address;
  //------------------------------------------ UNIT SUPPLY REQUIRED ------------------------------------------
  static unitsDat_u8_type* BWDATA_SupplyRequired = (unitsDat_u8_type*) unitsDat[46].address;
  //------------------------------------------ UNIT SPACE REQUIRED -------------------------------------------
  static unitsDat_u8_type* BWDATA_SpaceRequired = (unitsDat_u8_type*) unitsDat[47].address;
  //------------------------------------------ UNIT SPACE PRODUCED -------------------------------------------
  static unitsDat_u8_type* BWDATA_SpaceProvided = (unitsDat_u8_type*) unitsDat[48].address;
  //------------------------------------------- UNIT BUILD SCORE ---------------------------------------------
  static unitsDat_u16_type* BWDATA_BuildScore = (unitsDat_u16_type*) unitsDat[49].address;
  //------------------------------------------- UNIT DESTROY SCORE -------------------------------------------
  static unitsDat_u16_type* BWDATA_DestroyScore = (unitsDat_u16_type*) unitsDat[50].address;


  //--------------------------------------------- BROODWAR ONLY ----------------------------------------------
  static unitsDat_u16_type* BWDATA_BroodwarOnly = (unitsDat_u16_type*) unitsDat[52].address;
  //------------------------------------------- AVAILABILITY FLAGS -------------------------------------------
  static unitsDat_u16_type* BWDATA_AvailabilityFlags = (unitsDat_u16_type*) unitsDat[53].address;


  struct flingyDat_u8_type
  {
    u8 flingyType[FLINGY_TYPE_COUNT];
  };
  struct flingyDat_u16_type
  {
    u16 flingyType[FLINGY_TYPE_COUNT];
  };
  struct flingyDat_u32_type
  {
    u32 flingyType[FLINGY_TYPE_COUNT];
  };
  //----------------------------------------- FLINGY MOVEMENT CONTROL ----------------------------------------
  static flingyDat_u8_type* BWDATA_FlingyMovementControl = (flingyDat_u8_type*) flingyDat[6].address;
  //------------------------------------------ FLINGY HALT DISTANCE ------------------------------------------
  static flingyDat_u32_type* BWDATA_FlingyHaltDistance = (flingyDat_u32_type*) flingyDat[3].address;
  //------------------------------------------ FLINGY ACCELERATION -------------------------------------------
  static flingyDat_u16_type* BWDATA_FlingyAcceleration = (flingyDat_u16_type*) flingyDat[2].address;
  //------------------------------------------- FLINGY TURN RADIUS -------------------------------------------
  static flingyDat_u8_type* BWDATA_FlingyTurnRadius = (flingyDat_u8_type*) flingyDat[4].address;
  //-------------------------------------------- FLINGY TOP SPEED --------------------------------------------
  static flingyDat_u32_type* BWDATA_FlingyTopSpeed = (flingyDat_u32_type*) flingyDat[1].address;
  //--------------------------------------------- FLINGY UNUSED ----------------------------------------------
  static flingyDat_u8_type* BWDATA_FlingyUnused = (flingyDat_u8_type*) flingyDat[5].address;
  //------------------------------------------- FLINGY SPRITE FILE -------------------------------------------
  static flingyDat_u16_type* BWDATA_FlingySpriteFile = (flingyDat_u16_type*) flingyDat[0].address;

  struct weaponsDat_u8_type
  {
    u8 weaponType[WEAPON_TYPE_COUNT];
  };
  struct weaponsDat_u16_type
  {
    u16 weaponType[WEAPON_TYPE_COUNT];
  };
  struct weaponsDat_u32_type
  {
    u32 weaponType[WEAPON_TYPE_COUNT];
  };

  //---------------------------------------------- WEAPON LABEL ----------------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponLabel = (weaponsDat_u16_type*) weaponsDat[0].address;
  //--------------------------------------------- WEAPON GRAPHICS --------------------------------------------
  static weaponsDat_u32_type* BWDATA_WeaponGraphics = (weaponsDat_u32_type*) weaponsDat[1].address;
  //--------------------------------------- WEAPON UNUSED TECH PROPERTY --------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponUnusedTechProperty = (weaponsDat_u8_type*) weaponsDat[2].address;
  //------------------------------------------- WEAPON TARGET FLAGS ------------------------------------------
  struct TargetFlags_type
  {
    Util::BitMask<u16> weaponType[WEAPON_TYPE_COUNT];
  };
  static TargetFlags_type* BWDATA_WeaponTargetFlags = (TargetFlags_type*) weaponsDat[3].address;
  //-------------------------------------------- WEAPON MIN RANGE --------------------------------------------
  static weaponsDat_u32_type* BWDATA_WeaponMinRange = (weaponsDat_u32_type*) weaponsDat[4].address;
  //-------------------------------------------- WEAPON MAX RANGE --------------------------------------------
  static weaponsDat_u32_type* BWDATA_WeaponMaxRange = (weaponsDat_u32_type*) weaponsDat[5].address;
  //--------------------------------------------- WEAPON UPGRADE ---------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponUpgrade = (weaponsDat_u8_type*) weaponsDat[6].address;
  //------------------------------------------- WEAPON DAMAGE TYPE -------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponDamageType = (weaponsDat_u8_type*) weaponsDat[7].address;
  //---------------------------------------- WEAPON GRAPHICAL BEHAVIOR ---------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponGraphicalBehavior = (weaponsDat_u8_type*) weaponsDat[8].address;
  //------------------------------------------- WEAPON REMOVE AFTER ------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponRemoveAfter = (weaponsDat_u8_type*) weaponsDat[9].address;
  //------------------------------------------ WEAPON EXPLOSION TYPE -----------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponExplosionType = (weaponsDat_u8_type*) weaponsDat[10].address;
  //--------------------------------------- WEAPON INNER SPLASH RADIUS ---------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponInnerSplashRadius = (weaponsDat_u16_type*) weaponsDat[11].address;
  //--------------------------------------- WEAPON MEDIAN SPLASH RADIUS --------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponMedianSplashRadius = (weaponsDat_u16_type*) weaponsDat[12].address;
  //--------------------------------------- WEAPON OUTER SPLASH RADIUS ---------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponOuterSplashRadius = (weaponsDat_u16_type*) weaponsDat[13].address;
  //------------------------------------------ WEAPON DAMAGE AMOUNT ------------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponDamageAmount = (weaponsDat_u16_type*) weaponsDat[14].address;
  //------------------------------------------ WEAPON DAMAGE BONUS -------------------------------------------
  static weaponsDat_u16_type* BWDATA_WeaponDamageBonus = (weaponsDat_u16_type*) weaponsDat[15].address;
  //----------------------------------------- WEAPON DAMAGE COOLDOWN -----------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponDamageCooldown = (weaponsDat_u8_type*) weaponsDat[16].address;
  //------------------------------------------ WEAPON DAMAGE FACTOR ------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponDamageFactor = (weaponsDat_u8_type*) weaponsDat[17].address;
  //----------------------------------------- WEAPON ATTACK DIRECTION ----------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponAttackDirection = (weaponsDat_u8_type*) weaponsDat[18].address;
  //------------------------------------------- WEAPON LAUNCH SPIN -------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponLaunchSpin = (weaponsDat_u8_type*) weaponsDat[19].address;
  //-------------------------------------------- WEAPON X OFFSET ---------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponXOffset = (weaponsDat_u8_type*) weaponsDat[20].address;
  //-------------------------------------------- WEAPON Y OFFSET ---------------------------------------------
  static weaponsDat_u8_type* BWDATA_WeaponYOffset = (weaponsDat_u8_type*) weaponsDat[21].address;



  //------------------------------------------- TECH MINERAL COST --------------------------------------------
  struct TechCost
  {
    u16 tech[TECH_TYPE_COUNT];
  };
  static TechCost* BWDATA_TechMineralCost = (TechCost*) techdataDat[0].address;
  static TechCost* BWDATA_TechGasCost = (TechCost*) techdataDat[1].address;
  static TechCost* BWDATA_TechEnergyCost = (TechCost*) techdataDat[3].address;
  //------------------------------------------------ MAPPING -------------------------------------------------
  const u16 tileTypeCount = 65535;
  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      Util::BitMask<u16> miniTile[16];
    };
    MiniTileFlagArray tile[tileTypeCount];
  };

  extern MiniTileMaps_type* BWDATA_MiniTileFlags;
};
#pragma pack()
