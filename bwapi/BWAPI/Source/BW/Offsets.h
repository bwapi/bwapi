#pragma once
#pragma pack(1)

#include <Util/Bitmask.h>
#include <Util/Types.h>
#include "../StaticAssert.h"

#include "UnitPrototypeFlags.h"
#include "MiniTileFlags.h"
#include "GroupFlags.h"
#include "PlayerType.h"
#include "Race.h"

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
  static const u8  UPGRADE_TYPE_COUNT    =  66;

  static const int TILE_SIZE          =  32;
 
  struct Unit;
  struct UnitArray;
  class TileType;
  class DoodatType;
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
  };
  static PlayerResources* BWDATA_PlayerResources = (PlayerResources*) 0x0057F0D0; 

  //----------------------------------------------- NAMES TYPE -----------------------------------------------
  /** Direct mapping of players info in bw memory */
  struct Players
  {
    /** Direct mapping of player info in bw memory */
    struct PlayerInfo
    {
      u32              id;
      u32              actions; // unused; FF FF FF FF if not a human player
      PlayerType::Enum type;
      Race::Enum       race;
      u8               force;
      char             name[25];
    };
    PlayerInfo   player[PLAYER_COUNT];
  };

  struct Positions
  {
    u16   x;
    u16   y;
  };

  struct ForceName
  {
    char           name[30];
  };

  struct DatLoad
  {
    u32            address;
    u32            length;
    u32            entries;
  };

  struct PlayerAlliance
  {
    struct Alliances
    {
      u8 player[PLAYER_COUNT];
    };
    Alliances alliance[PLAYER_COUNT];
  };

  static DatLoad* upgradesDat = (DatLoad*) 0x005136E0; // 1.15.3 AND 1.16
  static DatLoad* techdataDat = (DatLoad*) 0x005137D8;
  static DatLoad* weaponsDat = (DatLoad*) 0x00513868;
  static DatLoad* unitsDat = (DatLoad*) 0x00513C30;

  static Positions* startPositions = (Positions*) 0x058D700;
  static ForceName* ForceNames = (ForceName*) 0x0058D59C;
  static Players* BWDATA_Players = (Players*) 0x0057EEC0;
  static PlayerAlliance* BWDATA_Alliance = (PlayerAlliance*) 0x0058D614;

  //----------------------------------------------- UNIT STATS -----------------------------------------------
  /** Direct mapping of bw tables used for unti stats like count, completed, kills, deaths */
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
  
  static Counts* BWDATA_Counts = (Counts*) 0x00582304;
  static u8* BWDATA_gameType = (u8*) 0x00596800;

  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  static TileID**       BWDATA_MapTileArray = (TileID**) 0x005993A4;
  static TileType**     BWDATA_TileSet = (TileType**) 0x006D5EA0;  /**< Index  0-1023 */
  static DoodatType**   BWDATA_DoodatSet = (DoodatType**) 0x006D5EA0; /**< Index 1024 + */
  static u16*           BWDATA_MapSizeX = (u16*) 0x0057F1B4;
  static u16*           BWDATA_MapSizeY = ((u16*) BWDATA_MapSizeX) + 1;
  static Unit**         BWDATA_UnitNodeTable_FirstElement = (Unit**)0x00628410;
  static UnitArray*     BWDATA_UnitNodeTable = (UnitArray*) 0x0059CB38; // starts properly at 0x0059CC88
  const  u32            UNIT_ARRAY_MAX_LENGTH = 1701; // should be 1700
  
  static void (_stdcall* selectUnits)(int count, BW::Unit ** unitsToSelect) = (void (_stdcall*) (int, BW::Unit * *)) 0x004C0750; 
  static void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit **, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool)) 0x0049AEE0;
  static u32            BWFXN_IssueCommand = 0x00485AC0; 
  static u32*           BWDATA_InGame = (u32*) 0x006556C0;
  static u8*            BWDATA_InReplay = (u8*) 0x006D0EF4;
  static u32            BWDATA_CountDownTimer = 0x0058D6D4;

  static u32            BWFXN_PrintText = 0x0048D0B0;
  
  static u16*           BWDATA_SendTextRequired = (u16*) 0x0057F1BA;
  static u16            BWDATA_FullMask = 0xFFFF;
  static u8*            BWDATA_CurrentPlayer = (u8*) 0x0057F090;

  static u32            BWFXN_SendPublicCall = 0x004F3107; 
  static u32            BWFXN_SendPublicCallBack = BWFXN_SendPublicCall + 5;
  static u32            BWFXN_SendPublicCallTarget = 0x004C2310;

  static u32            BWFXN_SendLobbyCall = 0x004B97CE;
  static u32            BWFXN_SendLobbyCallBack = BWFXN_SendLobbyCall + 5;
  static u32            BWFXN_SendLobbyCallTarget = 0x00470C70;

  static Unit*          BWDATA_CurrentPlayerSelectionGroup = (Unit*) 0x005971E8;

  static u32            BWFXN_GameStart = 0x004C9941;
  static u32            BWFXN_GameStartBack = BW::BWFXN_GameStart + 5;
  static u32            BWFXN_GameStartTarget = 0x00417C10;

  static u32            BWFXN_GameEnd = 0x004EE7D3;
  static u32            BWFXN_GameEndBack = BWFXN_GameEnd + 5;
  static u32            BWFXN_GameEndTarget = 0x00416D80;

  static u32            BWFXN_NextFrameHelperFunction = 0x004D977D; 
  static u32            BWFXN_NextFrameHelperFunctionBack = BWFXN_NextFrameHelperFunction + 5;
  static u32            BWFXN_NextFrameHelperFunctionTarget = 0x004D13B0;

  static u32            BWFXN_Refresh = 0x0041DDE0;

  static u32            BWFXN_CancelTrainByClickInTheQueue = 0x004573D9; 
  static u32            BWFXN_CancelTrainByClickInTheQueueBack = BWFXN_CancelTrainByClickInTheQueue + 5;
  static u32            BWFXN_CancelTrainByClickInTheQueueTarget = BWFXN_IssueCommand;

  static u32            BWFXN_CancelTrainByEscape = 0x00423494; 
  static u32            BWFXN_CancelTrainByEscapeBack = BWFXN_CancelTrainByEscape + 5;
  static u32            BWFXN_CancelTrainByEscapeTarget = BWFXN_IssueCommand;

  static u32            BWFXN_RemoveUnit = 0x004EC354; 
  static u32            BWFXN_RemoveUnitBack = BWFXN_RemoveUnit + 5;
  static u32            BWFXN_RemoveUnitTarget = 0x00479920;

  static u32            BWFXN_Redraw = 0x004D97F3; 
  static u32            BWFXN_RedrawBack = BWFXN_Redraw + 5;
  static u32            BWFXN_RedrawTarget = 0x0041C9F0;

  static u32            BWFXN_NextLogicFrame = 0x004D960E; 
  static u32            BWFXN_NextLogicFrameBack = BWFXN_NextLogicFrame + 5;
  static u32            BWFXN_NextLogicFrameTarget = 0x00488670;

  static u32            BWDATA_UpgradeLevelSC = 0x0058D290;
  static u32            BWDATA_UpgradeLevelBW = 0x0058F30C;
  static u32            BWDATA_TechResearchSC = 0x0058CF24;
  static u32            BWDATA_TechResearchBW = 0x0058F120;
  static u32            BWDATA_PlayerSelection = 0x006284C0;
  static u32            BWDATA_Mouse  = 0x006CDDA4; 
  static int*           BWDATA_MouseX = (int*) BWDATA_Mouse;
  static int*           BWDATA_MouseY = (int*) (BWDATA_Mouse + 4);
  static int*           BWDATA_ScreenX = (int*) 0x00628428; //  @todo: Find better screen x/y location (as a pair, one address)
  static int*           BWDATA_ScreenY = (int*) 0x00628450; 
  static char*          BWDATA_CurrentMapFileName = (char*) 0x0057FD1C;
  static u16*           BWDATA_TechLabelIndex = (u16*) techdataDat[7].address;
  static u16*           BWDATA_UpgradeLabelIndex = (u16*) upgradesDat[8].address;
  static u8*            BWDATA_UpgradeMax = (u8*) upgradesDat[10].address;
  static u16*           BWDATA_UpgradeMineralCostBase = (u16*) upgradesDat[0].address;
  static u16*           BWDATA_UpgradeMineralCostFactor = (u16*) upgradesDat[1].address;
  static u16*           BWDATA_UpgradeGasCostBase = (u16*) upgradesDat[2].address;
  static u16*           BWDATA_UpgradeGasCostFactor = (u16*) upgradesDat[3].address;
  static u16*           BWDATA_UpgradeTimeCostBase = (u16*) upgradesDat[4].address;
  static u16*           BWDATA_UpgradeTimeCostFactor = (u16*) upgradesDat[5].address;
  static Race::Enum*    BWDATA_UpgradeRace = (Race::Enum*) upgradesDat[9].address;
  static u32            BWDATA_StringTableOff = 0x006D1218;
  static u16**          BWDATA_StringTableIndex = (u16**) BWDATA_StringTableOff;
  static char**         BWDATA_StringTable = (char**) BWDATA_StringTableOff;
  static u32**          BWDATA_MapFogOfWar = (u32**) 0x006D1240;

  struct UpgradeProgress
  {
    Util::BitMask<u64> player[PLAYER_COUNT];
  };
  static UpgradeProgress* BWDATA_UpgradeProgress = (UpgradeProgress*) 0x0058F3C0;
  static u32 BWDATA_ResearchProgress = 0x0058F210; 

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
  static Supplies* BWDATA_Supplies    = (Supplies*) 0x00582124;
  
  
  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  /** Direct mapping of unit types mineral prices. */
  struct MineralPrices_type
  {
    u16 mineralPrice[UNIT_TYPE_COUNT];
  };
  static MineralPrices_type* BWDATA_MineralPrices = (MineralPrices_type*) unitsDat[40].address;

  //------------------------------------------ UNIT MINERAL PRICES -------------------------------------------
  /** Direct mapping of unit types gas prices. */
  struct GasPrices_type
  {
    u16 gasPrice[UNIT_TYPE_COUNT];
  };
  static GasPrices_type* BWDATA_GasPrices = (GasPrices_type*) unitsDat[41].address;
  //------------------------------------------ UNIT SUPPLY DEMANDS -------------------------------------------
  /** Direct mapping of unit supply demands. */
  struct SupplyDemands_type
  {
    u8 supplyDemand[UNIT_TYPE_COUNT];
  };
  static SupplyDemands_type* BWDATA_SupplyDemands = (SupplyDemands_type*) unitsDat[46].address;
  
  //------------------------------------------ UNIT SUPPLY PRODUCED ------------------------------------------
  /** Direct mapping of unit supply production. */
  struct SupplyProduced_type
  {
    u8 unitType[UNIT_TYPE_COUNT];
  };
  static SupplyProduced_type* BWDATA_SupplyProduced = (SupplyProduced_type*) unitsDat[45].address;

  //---------------------------------------------- UNIT MAX HP -----------------------------------------------
  /** Direct mapping of unit unit type (Max Health Points)/(Not Attackable)/(Requirable) specification. */
  struct MaxHealthPoints_NotAttackable_Repairable_type
  {
    /** mapping of the Max Health Points)/(Not Attackable)/(Requirable) for single unit type. */
    struct MaxHealthPoints_NotAttackable_Repairable_Internal_type
    {
      u16 maxHealthPoints;
      u8 notAttackable;
      u8 repairable;
    };
    MaxHealthPoints_NotAttackable_Repairable_Internal_type raw[UNIT_TYPE_COUNT];
  };
  static MaxHealthPoints_NotAttackable_Repairable_type* BWDATA_MaxHealthPoints_NotAttackable_Repairable = (MaxHealthPoints_NotAttackable_Repairable_type*) unitsDat[8].address;
  //-------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit unit type Shield points */
  struct MaxShieldPoints_type
  {
    u16 maxShieldPoints[UNIT_TYPE_COUNT];
  };
  static MaxShieldPoints_type* BWDATA_MaxShieldPoints = (MaxShieldPoints_type*) unitsDat[7].address;
   //------------------------------------------- UNIT MAX SHIELDS --------------------------------------------
  /** Direct mapping of unit type armor */
  struct Armor_type
  {
    u8 armor[UNIT_TYPE_COUNT];
  };
  static Armor_type* BWDATA_Armor = (Armor_type*) unitsDat[27].address;

    //----------------------------------------- UNIT MAX BUILD TIME ------------------------------------------
  /** Direct mapping of unit build time*/
  struct BuildTime_type
  {
   u16 buildTime[UNIT_TYPE_COUNT];
  };
  static BuildTime_type* BWDATA_BuildTime = (BuildTime_type*) unitsDat[42].address;

  //-------------------------------------------- UNIT DIRECTIONS ---------------------------------------------
  /** Direct mapping of unit unit type armor */
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
  const u8 NoWeapon = 130;
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit weapon type */
  struct UnitsGroundWeapon_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsGroundWeapon_type* BWDATA_UnitGroundWeapon = (UnitsGroundWeapon_type*) unitsDat[17].address;
  
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit sight range */
  struct UnitsSightRange_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsSightRange_type* BWDATA_UnitSightRange = (UnitsSightRange_type*) unitsDat[24].address;
  
  //-------------------------------------------- GROUND WEAPONS  ---------------------------------------------
  /** Direct mapping of unit unit sight range */
  struct UnitsSeekRange_type
  {
    u8 unit[UNIT_TYPE_COUNT];
  };
  static UnitsSeekRange_type* BWDATA_UnitSeekRange = (UnitsSeekRange_type*) unitsDat[23].address;
  
  const int weaponTypeCount = 130;
  //------------------------------------------ WEAPON DAMAGE FACTOR ------------------------------------------
  /** Direct mapping of weapon type damage factor*/
  struct WeaponsDamageFactor_type
  {
    u8 weapon[weaponTypeCount];
  };
  static WeaponsDamageFactor_type* BWDATA_WeaponDamageFactor = (WeaponsDamageFactor_type*) weaponsDat[17].address;

  //------------------------------------------- WEAPON DAMAGE TYPE -------------------------------------------
  /** Direct mapping of weapon type damage */
  struct WeaponsDamage_type
  {
    u16 weapon[weaponTypeCount];
  };
  static WeaponsDamage_type* BWDATA_WeaponDamage = (WeaponsDamage_type*) weaponsDat[14].address;
  
  //------------------------------------------- WEAPON RANGE TYPE --------------------------------------------
  /** Direct mapping of unit unit type armor */
  struct WeaponsRange_type
  {
    u32 weapon[weaponTypeCount];
  };
  static WeaponsRange_type* BWDATA_WeaponRange = (WeaponsRange_type*) weaponsDat[5].address;

  //------------------------------------------------- FLAGS --------------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeFlags_type
  {
    Util::BitMask<UnitPrototypeFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeFlags_type* BWDATA_UnitPrototypeFlags = (PrototypeFlags_type*) unitsDat[22].address;

  //---------------------------------------------- GROUP FLAGS -----------------------------------------------
  /** Direct mapping of unit flags data */
  struct PrototypeGroupFlags_type
  {
    Util::BitMask<GroupFlags::Enum> unit[UNIT_TYPE_COUNT];
  };
  static PrototypeGroupFlags_type* BWDATA_PrototypeGroupFlags = (PrototypeGroupFlags_type*) unitsDat[44].address;
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
      Util::BitMask<MiniTileFlags::Enum> miniTile[16];
    };
    MiniTileFlagArray tile[tileTypeCount];
  };

  static MiniTileMaps_type** BWDATA_MiniTileFlags = (MiniTileMaps_type**) 0x005993B0;
};
#pragma pack()
